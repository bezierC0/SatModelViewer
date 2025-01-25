#include "pch.h"
#include "AcisLib.h"
#include "avro_fr_utility.hpp"
#include "setting.h"

// ACIS includes
#include "acis.hxx"
#include "kernapi.hxx"
#include "lists.hxx"
#include "acistype.hxx"
#include "fileinfo.hxx"
#include "license.hxx"
#include "spa_unlock_result.hxx"
#include "cstrapi.hxx"
#include "sweepapi.hxx"
#include "ckoutcom.hxx"
#include "boolapi.hxx"
#include "kernapi.hxx"
#include "rnd_api.hxx"
#include "surf_utl.hxx"
#include <geom_utl.hxx>
#include <queryapi.hxx>
#include <ptentrel.hxx>
#include <ptentrel.hxx>
#include <at_name.hxx>
#include <ga_api.hxx>
#include <at_str.hxx>
#include <af_api.hxx>
#include <surface.hxx>
#include <curve.hxx>
#include <surdef.hxx>
#include <strdef.hxx>
#include <elldef.hxx>
#include <heldef.hxx>
#include <intdef.hxx>
#include <curdef.hxx>
#include <condef.hxx>
#include <pladef.hxx>
#include <sphdef.hxx>
#include <spldef.hxx>
#include <tordef.hxx>
#include <getowner.hxx>

#pragma comment(lib, "comctl32.lib")

void unlock_license();

static void get_triangles_from_faceted_face(
    FACE* face,
    std::vector<float>& coords,
    std::vector<int>& triangles,
    std::vector<float>& normalCoords)
{
    af_serializable_mesh* sm = GetSerializableMesh(face);
    if (NULL == sm)
    {
        // Application decision: do we throw for unfaceted faces?
        return;
    }
    SPAtransf tr = get_owner_transf(face);

    const int nv = sm->number_of_vertices();
    int ntri = sm->number_of_polygons();

    coords.resize(3 * nv);
    sm->serialize_positions(coords.data()); // if std::vector::data is not available, &(coords[0]) will also work.
    if (!tr.identity())
    {
        for (int ii = 0; ii < nv; ii++)
        {
            int jj = 3 * ii;
            SPAposition pos(coords[jj], coords[jj + 1], coords[jj + 2]);
            pos *= tr;
            coords[jj] = (float)pos.x();
            coords[jj + 1] = (float)pos.y();
            coords[jj + 2] = (float)pos.z();
        }
    }

    bool const has_normals = sm->has_normals() == TRUE;
    if (has_normals)
    {
        normalCoords.resize(3 * nv);
        sm->serialize_normals(normalCoords.data());
    }

    triangles.resize(3 * ntri);
    int ntri_actual = sm->serialize_triangles(triangles.data());
    while (ntri_actual < ntri)
    {
        triangles.pop_back();
        ntri_actual = static_cast<int>(triangles.size());
    }
}

static void get_triangles_from_faceted_faces(
    ENTITY_LIST& faces,
    std::vector<AcisMesh::FaceMesh>& faceData,
    std::vector<float>& coords,
    std::vector<int>& triangles,
    std::vector<float>& normalCoords,
    ENTITY_LIST& selectedList,
    std::vector<COGLJText*>& pmiList
    )
{
    int nF = 0;
    int nV = 0;
    int nI = 0;
    int numFaces = faces.iteration_count();
    assert(numFaces == faceData.size());
    for (ENTITY* ent = faces.first(); ent; ent = faces.next())
    {
        assert(nF < numFaces);
        assert(is_FACE(ent));
        if (!is_FACE(ent)) { continue; }

        FACE* face = (FACE*)ent;
        std::vector<float> temp_coords;
        std::vector<int> temp_triangles;
        std::vector<float> temp_normalCoords;
        get_triangles_from_faceted_face(face, temp_coords, temp_triangles, temp_normalCoords);
        {
            int nCoordsStart = (int)coords.size() / 3;
            int nCoords = (int)temp_coords.size();
            for (int ii = 0; ii < nCoords; ii++)
            {
                coords.push_back(temp_coords[ii]);
                normalCoords.push_back(temp_normalCoords[ii]);
            }
            int nTri = (int)temp_triangles.size();
            for (int jj = 0; jj < nTri; jj++)
            {
                triangles.push_back(temp_triangles[jj] + nCoordsStart);
            }
        }
        logical found = FALSE;
        outcome out = api_rh_get_entity_rgb(ent, faceData[nF].color, TRUE, found);
        if (!out.ok() || !found)
        {
            faceData[nF].color = rgb_color(1., 1., 1.);
        }

        for (int si = 0; si < selectedList.count(); ++si)
        {
            ENTITY* selectedItem = selectedList[si];
            if (is_FACE(selectedItem))
            {
                if (selectedItem && selectedItem == ent)
                {
                    faceData[nF].color = rgb_color(HIGH_LIGHT_COLOR[0], HIGH_LIGHT_COLOR[1], HIGH_LIGHT_COLOR[2]);
                }
            }
        }

        faceData[nF].numIndices = (unsigned int)temp_triangles.size();
        faceData[nF].baseIndex = (unsigned int)nI;
        faceData[nF].baseVertex = (unsigned int)nV;

        nI += (unsigned int)temp_triangles.size();
        nV += (unsigned int)temp_coords.size();
        nF++;
    }
}


static void get_polylines_from_faceted_edges(
    ENTITY_LIST& edges,
    std::vector<AcisMesh::EdgeMesh>& edgeData,
    std::vector<float>& coords,
    ENTITY_LIST& selectedList,
    std::vector<COGLJText*>& pmiList
)
{
    int nE = 0;
    int nV = 0;
    int numEdges = edges.iteration_count();
    assert(numEdges == edgeData.size());
    for (ENTITY* ent = edges.first(); ent; ent = edges.next())
    {
        assert(nE < numEdges);
        assert(is_EDGE(ent));
        if (!is_EDGE(ent)) { continue; }

        SPAtransf tr = get_owner_transf(ent);
        EDGE* edge = (EDGE*)ent;
        SPAposition* pos = NULL;

        int nP = 0;
        outcome out = api_get_facet_edge_points(edge, pos, nP);
        if (!out.ok())
        {
            ACIS_DELETE[] pos;
            continue;
        }
        for (int ii = 0; ii < nP; ii++)
        {
            pos[ii] *= tr;
            coords.push_back((float)pos[ii].x());
            coords.push_back((float)pos[ii].y());
            coords.push_back((float)pos[ii].z());
        }
        ACIS_DELETE[] pos;
        pos = NULL;
        logical found = FALSE;

        edgeData[nE].color = rgb_color(Setting::COLOR_EDGE[0], Setting::COLOR_EDGE[1], Setting::COLOR_EDGE[2]);

        for (int si = 0; si < selectedList.count(); ++si)
        {
            ENTITY* selectedItem = selectedList[si];
            if (is_EDGE(selectedItem))
            {
                if (selectedItem && selectedItem == ent)
                {
                    edgeData[nE].color = rgb_color(HIGH_LIGHT_COLOR[0], HIGH_LIGHT_COLOR[1], HIGH_LIGHT_COLOR[2]);
                }
            }
        }
        
        edgeData[nE].numIndices = 3 * nP;
        edgeData[nE].baseVertex = nV;
        nV += 3 * nP;
        nE++;

    }
}

static void get_polylines_from_faceted_vertices(
    ENTITY_LIST& vertices,
    std::vector<AcisMesh::EdgeVertex>& verticesData,
    ENTITY_LIST& selectedList,
    std::vector<COGLJText*>& pmiList
)
{
    std::vector<ENTITY*> selectedVertexList;
    std::vector<ENTITY*> selectedEdgeList;
    for (int ii = 0; ii < selectedList.count(); ++ii)
    {
        if (is_EDGE(selectedList[ii]))
        {
            selectedEdgeList.emplace_back(selectedList[ii]);
        }
        else if (is_VERTEX(selectedList[ii]))
        {
            selectedVertexList.emplace_back(selectedList[ii]);
        }
    }

    int numVertices = vertices.count();
    assert(numVertices == verticesData.size());
    for (int i = 0; i < numVertices; ++i)
    {
        double rgb[3] = { 0.0, 0.0, 0.0 };
        ENTITY* ent = vertices[i];
        assert(is_VERTEX(ent));
        if (!is_VERTEX(ent)) { continue; }
        VERTEX* vertex = (VERTEX*)ent;
        verticesData[i].x = (float)vertex->geometry()->coords().x();
        verticesData[i].y = (float)vertex->geometry()->coords().y();
        verticesData[i].z = (float)vertex->geometry()->coords().z();

        if (!selectedVertexList.empty())
        {
            for (int si = 0; si < selectedVertexList.size(); ++si)
            {
                ENTITY* selectedItem = selectedVertexList[si];
                if (selectedItem && selectedItem == ent)
                {
                    rgb[0] = HIGH_LIGHT_COLOR[0];
                    rgb[1] = HIGH_LIGHT_COLOR[1];
                    rgb[2] = HIGH_LIGHT_COLOR[2];
                }
            }
        }
        else
        {
            for (int si = 0; si < selectedEdgeList.size(); ++si)
            {
                EDGE* selectedEdgeItem = (EDGE*)selectedEdgeList[si];
                if (selectedEdgeItem->start() == ent)
                {
                    // 始点Red
                    rgb[0] = 1.0;
                    rgb[1] = 0.0;
                    rgb[2] = 0.0;
                }
                else if (selectedEdgeItem->end() == ent)
                {
                    // 始点Blue
                    rgb[0] = 0.0;
                    rgb[1] = 0.0;
                    rgb[2] = 1.0;
                }
                
            }
        }

        verticesData[i].color = rgb_color(rgb[0], rgb[1], rgb[2]);
    }
}


int initialize_acis()
{
    outcome result;

    int level = 0;
    // Base initalization will among other things
    //  create thread-local storage and a thread ID.
    if (initialize_base())
    {
        ++level;
        outcome result = api_start_modeller(0);
        if (result.ok())
        {
            ++level;
            // Add common ACIS state settings here.
        }
    }
    FileInfo fileinfo;
    fileinfo.set_units(1.0);
    fileinfo.set_product_id("ACIS (c) SPATIAL");
    result = api_set_file_info(3, fileinfo);

    if (2 > level)
    {
        MessageBox(NULL, L"Could not initialize ACIS", L"Error!", NULL);
    }
    else
    {
        unlock_license();
    }
    return level;
}

void terminate_acis(int level)
{
    // terminate ACIS
    if (level > 1)
    {
        // This outcome object must destruct before terminate base is called,
        //  because thread-local storage is accessed.
        outcome result = api_stop_modeller();
        if (result.ok() != FALSE)
        {
            --level;
        }
    }

    if (level > 0)
    {
        if (terminate_base())
        {
            --level;
        }
    }
}

bool CreateMeshFromEntityList(
    ENTITY_LIST& el, 
    AcisMesh::DisplayData& dd, 
    ENTITY_LIST& selectedList, 
    std::vector<COGLJText*>& pmiList
)
{
    int numEnt = el.iteration_count();
    if (0 == numEnt) { return false; }
    {
        for (ENTITY* ent = el.first(); ent; ent = el.next())
        {
            outcome out = api_facet_entity(ent);
            if (!out.ok())
            {
                return false;
            }
        }
    }
    ENTITY_LIST faces;
    {
        for (ENTITY* ent = el.first(); ent; ent = el.next())
        {
            if (is_EDGE(ent))
            {
                continue;
            }
            outcome out = api_get_faces(ent, faces);
            if (!out.ok()) { return false; }
        }
    }
    int numFaces = faces.iteration_count();
    dd.faceMesh.resize(numFaces);

    ENTITY_LIST edges;
    {
        for (ENTITY* ent = el.first(); ent; ent = el.next())
        {
            outcome out = api_get_edges(ent, edges);
            if (!out.ok()) { return false; }
        }
    }
    int numEdges = edges.iteration_count();
    dd.edgeMesh.resize(numEdges);

    ENTITY_LIST vertices;
    {
        for (ENTITY* ent = el.first(); ent; ent = el.next())
        {
            outcome out = api_get_vertices(ent, vertices);
            if (!out.ok()) { return false; }
        }
    }
    int numVertices = vertices.iteration_count();
    dd.vertices.resize(numVertices);

    if (0 == numEdges + numFaces) { return false; }

    // FACEに含むEdgeもselectedListに追加、ハイライト
    ENTITY_LIST selected_face_edges;
    for (int si = 0; si < selectedList.count(); ++si)
    {
        ENTITY* selectedItem = selectedList[si];
        if (is_FACE(selectedItem))
        {
            ENTITY_LIST face_edges;
            api_get_edges(selectedItem, face_edges);
            for (int ii = 0; ii < face_edges.count(); ++ii)
            {
                selected_face_edges.add(face_edges[ii]);
            }
        }
    }
    for (int ii = 0; ii < selected_face_edges.count(); ++ii)
    {
        selectedList.add(selected_face_edges[ii]);
    }

    if (Setting::IS_DISPALY_FACE)
    {
        get_triangles_from_faceted_faces(faces, dd.faceMesh, dd.faceCoords, dd.triangles, dd.normalCoords, selectedList, pmiList);
    }
    
    get_polylines_from_faceted_edges(edges, dd.edgeMesh, dd.edgeCoords, selectedList, pmiList);
    get_polylines_from_faceted_vertices(vertices, dd.vertices, selectedList, pmiList);

    return true;
}

int SaveSAT(const char* file_name, const ENTITY_LIST* opEntities)
{
    FileInfo info;
    info.set_product_id("Meviy feature recognition");
    info.set_units(1.0);
    check_outcome(api_set_file_info((FileIdent | FileUnits), info));
    check_outcome(api_save_version(22, 0));

    FILE* fp = NULL;
    fopen_s(&fp, file_name, "wb");
    if (fp == nullptr)
    {
        return 1;
    }
    else
    {
        check_outcome(api_save_entity_list(fp, TRUE, *opEntities));
    }
    fclose(fp);
    return 0;
}

int RestoreSAT(const char* file_name, ENTITY_LIST& elist)
{
    // Open a file for reading, restore the model from the file, and close the file.
    FILE* save_file = nullptr;
    fopen_s(&save_file, file_name, "rb");
    check_outcome(api_restore_entity_list(save_file, TRUE, elist));
    if (save_file)
    {
        fclose(save_file);
        save_file = nullptr;
    }
    
    return 0;
}

std::string find_attribute(ENTITY* ent, const char* name)
{
    ATTRIB_GEN_NAME* attr_uid = nullptr;
    api_find_named_attribute(ent, name, attr_uid);
    const auto uid_attr = dynamic_cast<ATTRIB_GEN_STRING*>(attr_uid);
    if (!uid_attr)
    {
        return std::string("");
    }

    return std::string(uid_attr->value());
}

std::string get_entity_type(ENTITY* ent)
{
    std::string type("");
    if(is_FACE(ent))
    {
        type = get_face_type(ent);
    }
    else if(is_EDGE(ent))
    {
        type = get_edge_type(ent);
    }
    return type;
}

std::string get_face_type(ENTITY* ent)
{
    std::string type("");
    if (is_planar_face(ent))
    {
        type = "PLANE\r\n";
    }
    else if (is_cylindrical_face(ent))
    {
        type = "cylinder\r\n";
    }
    else if (is_conical_face(ent))
    {
        type = "CONE\r\n";
    }
    else if (is_spherical_face(ent))
    {
        type = "SPHERE\r\n"; 
    }
    else if (is_toroidal_face(ent))
    {
        type = "TORUS\r\n";
    }
    else if (is_spline_face(ent))
    {
        type = "SPLINE\r\n";
    }
    else if (is_analytic_face(ent))
    {
        type = "analytic\r\n";
    }
    else 
    {
        type = "null\r\n";
    }
    return type;
}

std::string get_edge_type(ENTITY* ent)
{
    std::string type("");
    if (is_curve_edge(ent))
    {
        type = "curve\r\n";
    }
    if (is_linear_edge(ent))
    {
        type = "straight\r\n";
    }
    else if (is_elliptical_edge(ent))
    {
        type = "ellipse\r\n";
    }
    else if (is_helical_edge(ent))
    {
        type = "helix\r\n";
    }
    else if (is_intcurve_edge(ent))
    {
        type = "intcurve\r\n";
    }
    else 
    {
        type = "null\r\n";
    }
    if (is_circular_edge(ent))
    {
        type = "circular\r\n";
    }
    return type;
}

std::string get_entity_info(ENTITY* ent)
{
    std::string info("");
    if (is_BODY(ent))
    {
        info = "BODY \r\n";
        // 面積
        double  area, accuracy = .001;
        api_ent_area(ent, accuracy, area, accuracy);
        info += "area : " + std::to_string(area) + " mm2\r\n";
    }
    else if (is_WIRE(ent))
    {
        info = "WIRE  \r\n";
    }
    else if (is_LUMP(ent))
    {
        info = "LUMP  \r\n";
    }
    else if (is_SHELL(ent))
    {
        info = "SHELL  \r\n";
    }
    else if (is_SUBSHELL(ent))
    {
        info = "SUBSHELL \r\n";
    }
    else if (is_FACE(ent))
    {
        FACE* face = dynamic_cast<FACE*>(ent);
        if(face)
        {
            outcome result;
            SURFACE* surf = face->geometry();
            if (surf)
            {
                const surface& surfdef = surf->equation();
                if (is_plane(&surfdef))
                {
                    const plane* face_surface = dynamic_cast<const plane*>(&surfdef);
                    info += "plane \r\n";
                    info += "root_point : " + std::to_string(face_surface->root_point.x()) + ","
                        + std::to_string(face_surface->root_point.y()) + ","
                        + std::to_string(face_surface->root_point.z())
                        + "\r\n";
                    info += "normal : " + std::to_string(face_surface->normal.x()) + ","
                        + std::to_string(face_surface->normal.y()) + ","
                        + std::to_string(face_surface->normal.z())
                        + "\r\n";
                    info += "reverse_v : " + std::to_string(face_surface->reverse_v) 
                        + "\r\n";
                }
                else if (is_cone(&surfdef))
                {
                    const cone* face_surface = dynamic_cast<const cone*>(&surfdef);
                    info += "cone \r\n";
                    info += "sine_angle : " + std::to_string(face_surface->sine_angle) + "\r\n";
                    info += "cosine_angle : " + std::to_string(face_surface->cosine_angle) + "\r\n";

                    const ellipse* edge_curve = dynamic_cast<const ellipse*>(&face_surface->base);
                    if (is_circular(&face_surface->base))
                    {
                        info += "ellipse \r\n";
                    }
                    else
                    {
                        info += "circle \r\n";
                    }

                    info += "centre : " + std::to_string(edge_curve->centre.x()) + ","
                        + std::to_string(edge_curve->centre.y()) + ","
                        + std::to_string(edge_curve->centre.z())
                        + "\r\n";
                    info += "normal : " + std::to_string(edge_curve->normal.x()) + ","
                        + std::to_string(edge_curve->normal.y()) + ","
                        + std::to_string(edge_curve->normal.z())
                        + "\r\n";
                    info += "major_axis : " + std::to_string(edge_curve->major_axis.x()) + ","
                        + std::to_string(edge_curve->major_axis.y()) + ","
                        + std::to_string(edge_curve->major_axis.z())
                        + "\r\n";
                    info += "major_axis_length : " + std::to_string(edge_curve->major_axis_length)
                        + "\r\n";
                    info += "radius_ratio : "
                        + std::to_string(edge_curve->radius_ratio)
                        + "\r\n";
                    info += "param_range : " + std::to_string(edge_curve->param_range().start_pt()) + ","
                        + std::to_string(edge_curve->param_range().mid_pt()) + ","
                        + std::to_string(edge_curve->param_range().end_pt())
                        + "\r\n";
                }
                else if (is_sphere(&surfdef))
                {
                    const sphere* face_surface = dynamic_cast<const sphere*>(&surfdef);
                    if (is_cylinder(&surfdef))
                    {
                        info += "cylinder \r\n";
                    }
                    else
                    {
                        info += "sphere \r\n";
                    }

                    info += "centre : " + std::to_string(face_surface->centre.x()) + ","
                        + std::to_string(face_surface->centre.y()) + ","
                        + std::to_string(face_surface->centre.z())
                        + "\r\n";

                    info += "radius : " + std::to_string(face_surface->radius) + ","
                        + "\r\n";

                    info += "uv_oridir : " + std::to_string(face_surface->uv_oridir.x()) + ","
                        + std::to_string(face_surface->uv_oridir.y()) + ","
                        + std::to_string(face_surface->uv_oridir.z())
                        + "\r\n";

                    info += "pole_dir : " + std::to_string(face_surface->pole_dir.x()) + ","
                        + std::to_string(face_surface->pole_dir.y()) + ","
                        + std::to_string(face_surface->pole_dir.z())
                        + "\r\n";
                }
                else if (is_torus(&surfdef))
                {
                    const torus* face_surface = dynamic_cast<const torus*>(&surfdef);
                    info += "torus \r\n";
                }
                else if (is_spline(&surfdef))
                {
                    const spline* face_surface = dynamic_cast<const spline*>(&surfdef);
                    info += "spline \r\n";

                    //info += "discontinuities_u : " + std::to_string(face_surface->discontinuities_u()) + "\r\n";
                    //info += "discontinuities_u : " + std::to_string(face_surface->discontinuities_u()) + "\r\n";
                }
            }
            // 面積
            double  area, accuracy = .001;
            result = api_ent_area(face, accuracy, area, accuracy);
            info += "area : " + std::to_string(area) + " mm2\r\n";
        }
    }
    else if (is_EDGE(ent))
    {
        EDGE* edge = dynamic_cast<EDGE*>(ent);
        if (edge)
        {
            CURVE* cur = edge->geometry();
            if (cur)
            {
                const double pi = 3.14;
                const curve& curdef = cur->equation();
                if(is_straight(&curdef))
                {
                    const straight* edge_curve = dynamic_cast<const straight*>(&curdef);
                    info += "straight \r\n";
                    info += "param_range : " + std::to_string(edge_curve->param_range().start_pt()) + ","
                        + std::to_string(edge_curve->param_range().mid_pt()) + ","
                        + std::to_string(edge_curve->param_range().end_pt())
                        + "\r\n";
                }
                else if(is_ellipse(&curdef))
                {
                    const ellipse* edge_curve = dynamic_cast<const ellipse*>(&curdef);
                    if (is_circular(&curdef))
                    {
                        info += "ellipse \r\n";
                    }
                    else
                    {
                        info += "circle \r\n";
                    }

                    info += "centre : " + std::to_string(edge_curve->centre.x()) + ","
                        + std::to_string(edge_curve->centre.y()) + ","
                        + std::to_string(edge_curve->centre.z())
                        + "\r\n";
                    info += "normal : " + std::to_string(edge_curve->normal.x()) + ","
                        + std::to_string(edge_curve->normal.y()) + ","
                        + std::to_string(edge_curve->normal.z())
                        + "\r\n";
                    info += "major_axis : " + std::to_string(edge_curve->major_axis.x()) + ","
                        + std::to_string(edge_curve->major_axis.y()) + ","
                        + std::to_string(edge_curve->major_axis.z())
                        + "\r\n";
                    info += "major_axis_length : " + std::to_string(edge_curve->major_axis_length)
                        + "\r\n";
                    info += "radius_ratio : " 
                        + std::to_string(edge_curve->radius_ratio)
                        + "\r\n";
                    info += "param_range : " + std::to_string(edge_curve->param_range().start_pt() / pi) + ","
                        + std::to_string(edge_curve->param_range().mid_pt() / pi) + ","
                        + std::to_string(edge_curve->param_range().end_pt() / pi)
                        + "\r\n";

                }

                else if (is_helix(&curdef))
                {
                    const helix* edge_curve = dynamic_cast<const helix*>(&curdef);
                    info += "helix \r\n";
                }
                else if (is_intcurve(&curdef))
                {
                    const intcurve* edge_curve = dynamic_cast<const intcurve*>(&curdef);
                    info += "intcurve \r\n";
                }
                else if (is_composite_curve(&curdef))
                {
                    info += "composite_curve \r\n";
                }
            }

            info += "length : " + std::to_string(edge->length()) + " mm\r\n";
            info += "start_pos : " + std::to_string(edge->start_pos().x()) + ","
                + std::to_string(edge->start_pos().y()) + ","
                + std::to_string(edge->start_pos().z())
                + "\r\n";
            info += "end_pos : " + std::to_string(edge->end_pos().x()) + ","
                + std::to_string(edge->end_pos().y()) + ","
                + std::to_string(edge->end_pos().z())
                + "\r\n";
        }
    }
    return info;
}

FACE* create_shape_face_cone(
    const SPAposition& center,
    const SPAunit_vector& normal_axis,
    const SPAvector& major_axis,
    double radius_ratio,
    double sint,
    double cost,
    double st_ang,
    double end_ang,
    double height
)
{

    FACE* result_face = nullptr;
    outcome result = api_make_cnface(
        center,
        normal_axis,
        major_axis,
        radius_ratio,
        sint,
        cost,
        st_ang,
        end_ang,
        height,
        result_face
    );

    if (!result.ok() || !result_face)
    {
        return nullptr;
    }

    return result_face;
}



boost::optional<std::vector<EDGE*>> convert_welding_edge_to_acis_edge(const fr::recognize_welding::Edge* edge)
{
    bool is_reversed = false;
    boost::optional<std::vector<EDGE*>> calc_target = boost::none;
    auto curve = edge->curve;
    if (avro_fr::union_util::is_same_type<fr::recognize_welding::Circle>(curve) ||
        avro_fr::union_util::is_same_type<fr::recognize_welding::Ellipse>(curve))
    {
        const auto& circle = curve.get_Circle();

        SPAposition center(circle.center.x, circle.center.y, circle.center.z);
        SPAvector normal(circle.normal.x, circle.normal.y, circle.normal.z);
        SPAunit_vector unit_vec = normalise(normal);
        SPAvector major_axis(circle.majorAxis.x, circle.majorAxis.y, circle.majorAxis.z);

        double start_angle = edge->paramRange.start.get_double();
        double end_angle = edge->paramRange.end.get_double();

        EDGE* arc = NULL;
        auto result = api_mk_ed_ellipse(center, unit_vec, major_axis, circle.radiusRatio, start_angle, end_angle, arc);
        if (!result.ok())
        {
            err_mess_type err_no = result.error_number();
            //BOOST_LOG_TRIVIAL(error) << __FUNCTION__ << " : cannot convert welding circle/arc to ACIS edge. " << err_no << ": " << find_err_mess(err_no);
            return calc_target; // error
        }

        if (is_reversed)
        {
            major_axis = arc->end_pos() - center;
            api_del_entity(arc);
            unit_vec = -unit_vec;
            auto result = api_mk_ed_ellipse(center, unit_vec, major_axis, circle.radiusRatio, start_angle, end_angle, arc);
            if (!result.ok())
            {
                err_mess_type err_no = result.error_number();
                //BOOST_LOG_TRIVIAL(error) << __FUNCTION__ << " : cannot convert welding circle/arc to ACIS edge. " << err_no << ": " << find_err_mess(err_no);
                return calc_target; // error
            }
        }

        std::vector<EDGE*> calc_target_;
        calc_target_.push_back(arc);
        calc_target = calc_target_;
    }
    else if (avro_fr::union_util::is_same_type<fr::recognize_welding::Polyline>(curve))
    {
        const auto& polyline = curve.get_Polyline();

        // 関数：各頂点が同じ直線にある
        auto is_on_the_same_line = [](const std::vector<fr::recognize_welding::Point3D>& points)
        {
            SPAunit_vector vec_unit(0.0, 0.0, 0.0);
            for (int i = 0; i < points.size() - 1; i++)
            {
                SPAposition pnt1(points[i].x, points[i].y, points[i].z);
                SPAposition pnt2(points[i + 1].x, points[i + 1].y, points[i + 1].z);
                SPAunit_vector pnt1_2 = normalise(pnt2 - pnt1);
                if (vec_unit.is_zero())
                {
                    vec_unit = pnt1_2;
                }
                else
                {
                    if (!biparallel(vec_unit, pnt1_2))
                    {
                        return false;
                    }
                }
            }
            return true;
        };

        // 同じ直線チェック
        if (is_on_the_same_line(polyline.points))
        {
            auto get_max_min = [](
                const std::vector<fr::recognize_welding::Point3D>& points,
                fr::recognize_welding::Point3D& min_point,
                fr::recognize_welding::Point3D& max_point
                )
            {
                if (!points.empty())
                {
                    min_point = points[0];
                    max_point = points[0];
                    for (int i = 1; i < points.size(); i++)
                    {
                        if (max_point.x <= points[i].x &&
                            max_point.y <= points[i].y &&
                            max_point.z <= points[i].z
                            )
                        {
                            max_point = points[i];
                        }
                        if (min_point.x >= points[i].x &&
                            min_point.y >= points[i].y &&
                            min_point.z >= points[i].z
                            )
                        {
                            min_point = points[i];
                        }
                    }
                }

            };
            fr::recognize_welding::Point3D min_point;
            fr::recognize_welding::Point3D max_point;
            get_max_min(polyline.points, min_point, max_point);
            SPAposition pnt1(min_point.x, min_point.y, min_point.z);
            SPAposition pnt2(max_point.x, max_point.y, max_point.z);
            EDGE* line;
            auto result = api_curve_line(pnt1, pnt2, line);
            if (!result.ok())
            {
                err_mess_type err_no = result.error_number();
                //BOOST_LOG_TRIVIAL(error) << __FUNCTION__ << " : cannot convert welding polyline to ACIS edge. " << err_no << ": " << find_err_mess(err_no);
                return calc_target; // error
            }
            std::vector<EDGE*> calc_target_;
            calc_target_.push_back(line);
            calc_target = calc_target_;
        }
        else
        {
            SPAposition pnt_prev;
            auto i = 0;
            for (const auto& point : polyline.points)
            {
                SPAposition pnt(point.x, point.y, point.z);
                if (i != 0)
                {
                    EDGE* line;
                    auto result = api_curve_line(pnt_prev, pnt, line);
                    if (!result.ok())
                    {
                        err_mess_type err_no = result.error_number();
                        //BOOST_LOG_TRIVIAL(error) << __FUNCTION__ << " : cannot convert welding polyline to ACIS edge. " << err_no << ": " << find_err_mess(err_no);
                        return calc_target; // error
                    }
                    std::vector<EDGE*> calc_target_;
                    calc_target_.push_back(line);
                    calc_target = calc_target_;
                }
                pnt_prev = pnt;
                ++i;
            }
        }
    }
    else if (avro_fr::union_util::is_same_type<fr::recognize_welding::Straight>(curve))
    {
        const auto& straight = curve.get_Straight();
        // start point(virtual vertex) =paramRange * sense * scale * root 計算仕様
        SPAposition stt(straight.rootPoint.x + straight.direction.x * straight.paramScale * edge->paramRange.start.get_double(),
            straight.rootPoint.y + straight.direction.y * straight.paramScale * edge->paramRange.start.get_double(),
            straight.rootPoint.z + straight.direction.z * straight.paramScale * edge->paramRange.start.get_double());
        SPAposition end(
            straight.rootPoint.x + straight.direction.x * straight.paramScale * edge->paramRange.end.get_double(),
            straight.rootPoint.y + straight.direction.y * straight.paramScale * edge->paramRange.end.get_double(),
            straight.rootPoint.z + straight.direction.z * straight.paramScale * edge->paramRange.end.get_double());

        if (is_reversed)
        {
            std::swap(stt, end);
        }

        EDGE* line;
        auto result = api_curve_line(stt, end, line);
        if (!result.ok())
        {
            err_mess_type err_no = result.error_number();
            //BOOST_LOG_TRIVIAL(error) << __FUNCTION__ << " : cannot convert welding line to ACIS edge. " << err_no << ": " << find_err_mess(err_no);
            return calc_target; // error
        }
        std::vector<EDGE*> calc_target_;
        calc_target_.push_back(line);
        calc_target = calc_target_;
    }
    return calc_target;

}