#include "pch.h"
#include <stdio.h>
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "ModelViewer.h"
#endif

#include "ModelViewerDoc.h"
#include "ModelViewerView.h"
#include "MainFrm.h"
#include "CAvroView.h"
#include "ViewTreeEntity.h"
#include "ViewTreeAvro.h"
#include "CDialogAvro.h"
#include "CDialogLookAt.h"
#include "CDialogShapePoint.h"
#include "CDialogShapeEdgeStraight.h"
#include "CDialogShapeFaceCone.h"
#include "CDialogShapeFacePlane.h"

#include "LoadStlFile.h"
#include "setting.h"
#include "utility/json/json11util.hpp"

// START FR
//#include "recognized_welding_result.h"
#include "avro_fr_utility.hpp"
// END FR

// START ACIS
#include "ga_api.hxx"
#include "cstrapi.hxx"
#include "kernapi.hxx"
#include "acistype.hxx"
#include "rnd_api.hxx"
#include "rgbcolor.hxx"
#include "edge.hxx"
#include "vertex.hxx"
#include "point.hxx"
#include "unitvec.hxx"
#include "boolapi.hxx"
// END ACIS

#include <boost/filesystem.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CDialogApiSubtract.h"


// CModelViewerView

IMPLEMENT_DYNCREATE(CModelViewerView, COpenGLPickView)

BEGIN_MESSAGE_MAP(CModelViewerView, COpenGLPickView)
    // 標準印刷コマンド
    //ON_COMMAND(ID_FILE_PRINT, &COpenGLPickView::OnFilePrint)
    //ON_COMMAND(ID_FILE_PRINT_DIRECT, &COpenGLPickView::OnFilePrint)
    //ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CModelViewerView::OnFilePrintPreview)
    //ON_WM_CONTEXTMENU()
    //ON_WM_RBUTTONUP()
    ON_COMMAND(ID_VIEW_ROTATION, &CModelViewerView::OnViewRotation)
    ON_COMMAND(ID_VIEW_PAN, &CModelViewerView::OnViewPan)
    ON_COMMAND(ID_VIEW_ZOOM, &CModelViewerView::OnViewZoom)
    ON_UPDATE_COMMAND_UI(ID_VIEW_ROTATION, &CModelViewerView::OnUpdateViewRotation)
    ON_UPDATE_COMMAND_UI(ID_VIEW_PAN, &CModelViewerView::OnUpdateViewPan)
    ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM, &CModelViewerView::OnUpdateViewZoom)
    ON_COMMAND(ID_VIEW_RENDERPOINT, &CModelViewerView::OnViewRenderpoint)
    ON_COMMAND(ID_VIEW_RENDERLINE, &CModelViewerView::OnViewRenderline)
    ON_COMMAND(ID_VIEW_RENDERFACE, &CModelViewerView::OnViewRenderface)
    ON_UPDATE_COMMAND_UI(ID_VIEW_RENDERPOINT, &CModelViewerView::OnUpdateViewRenderpoint)
    ON_UPDATE_COMMAND_UI(ID_VIEW_RENDERLINE, &CModelViewerView::OnUpdateViewRenderline)
    ON_UPDATE_COMMAND_UI(ID_VIEW_RENDERFACE, &CModelViewerView::OnUpdateViewRenderface)
    ON_COMMAND(ID_VIEW_PICKPOINT, &CModelViewerView::OnViewPickpoint)
    ON_COMMAND(ID_VIEW_PICKLINE, &CModelViewerView::OnViewPickline)
    ON_COMMAND(ID_VIEW_PICKFACE, &CModelViewerView::OnViewPickface)
    ON_UPDATE_COMMAND_UI(ID_VIEW_PICKPOINT, &CModelViewerView::OnUpdateViewPickpoint)
    ON_UPDATE_COMMAND_UI(ID_VIEW_PICKLINE, &CModelViewerView::OnUpdateViewPickline)
    ON_UPDATE_COMMAND_UI(ID_VIEW_PICKFACE, &CModelViewerView::OnUpdateViewPickface)
    //ON_COMMAND(ID_FILE_OPEN, &CModelViewerView::OnFileOpen)
    ON_COMMAND(ID_FILE_NEW, &CModelViewerView::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, &CModelViewerView::OnFileOpenSat)
    ON_COMMAND(ID_FILE_OPEN_ADD, &CModelViewerView::OnFileOpenAddSat)
    ON_COMMAND(ID_FILE_OPEN_AVRO, &CModelViewerView::OnFileOpenAvro)
    ON_COMMAND(ID_FILE_OPEN_JSON, &CModelViewerView::OnFileOpenJson)
    ON_COMMAND(ID_FILE_SAVE_AS, &CModelViewerView::OnFileSaveAs)
    ON_COMMAND(ID_LOOKAT_CENTER, &CModelViewerView::OnSetLookAtCenter)

    ON_COMMAND(ID_SHAPE_POINT, &CModelViewerView::OnShapePoint)
    ON_COMMAND(ID_SHAPE_EDGE_STRAIGHT, &CModelViewerView::OnShapeEdgeStraight)
    ON_COMMAND(ID_SHAPE_EDGE_ELLIPSE, &CModelViewerView::OnShapeEdgeEllipse)

    ON_COMMAND(ID_SHAPE_FACE_CONE, &CModelViewerView::OnShapeFaceCone)
    ON_COMMAND(ID_SHAPE_FACE_PLANE, &CModelViewerView::OnShapeFacePlane)
    ON_COMMAND(ID_SHAPE_FACE_SPHERE, &CModelViewerView::OnShapeFaceSphere)
    ON_COMMAND(ID_SHAPE_FACE_SPLINE, &CModelViewerView::OnShapeFaceSpline)
    ON_COMMAND(ID_SHAPE_FACE_TORUS, &CModelViewerView::OnShapeFaceTorus)

    ON_COMMAND(ID_SHAPE_SOLID_BLOCK, &CModelViewerView::OnShapeSolidBlock)
    ON_COMMAND(ID_SHAPE_SOLID_CYLINDER, &CModelViewerView::OnShapeSolidCylinder)
    ON_COMMAND(ID_SHAPE_SOLID_SPHERE, &CModelViewerView::OnShapeSolidSphere)
    ON_COMMAND(ID_SHAPE_SOLID_TORUS, &CModelViewerView::OnShapeSolidTorus)

    ON_COMMAND(ID_API_SUBTRACT, &CModelViewerView::OnApiSubtract)
    ON_COMMAND(ID_API_UNITE, &CModelViewerView::OnApiUnite)
    ON_WM_KEYDOWN()
    ON_WM_KEYUP()
    ON_BN_CLICKED(IDOK, &CModelViewerView::OnApiSubtractBnClickedOk)
END_MESSAGE_MAP()

// CModelViewerView コンストラクション/デストラクション
CModelViewerView::CModelViewerView() :
    m_bRotation(TRUE),
    m_bPan(TRUE),
    m_bZoom(TRUE),
    m_bRenderPoint(TRUE),
    m_bRenderLine(TRUE),
    m_bRenderFace(TRUE),
    m_bPickPoint(TRUE),
    m_bPickLine(TRUE),
    m_bPickFace(TRUE)
{
    m_fLineWidth = Setting::Line_Width;    // Lineサイズ
    m_fPointSize = Setting::Point_Size;    // 頂点サイズ
    m_fFaceAlpha = Setting::Face_Alpha;    // Face透明度
    m_fLineAlpha = Setting::Line_Alpha;    // Line透明度
    m_fPointAlpha = Setting::Point_Alpha;    // 頂点透明度
}

CModelViewerView::~CModelViewerView()
{

    if (m_display_data)
    {
        delete m_display_data;
        m_display_data = nullptr;
    }
    for (int i=0;i< m_pmi_list.size();++i)
    {
        if (m_pmi_list[i])
        {
            delete m_pmi_list[i];
        }
    }
}

BOOL CModelViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
    //  修正してください。

    return COpenGLPickView::PreCreateWindow(cs);
}

// CModelViewerView 描画
//void CModelViewerView::OnDraw(CDC* /*pDC*/)
//{
//    CModelViewerDoc* pDoc = GetDocument();
//    ASSERT_VALID(pDoc);
//    if (!pDoc)
//        return;
//
//    // TODO: この場所にネイティブ データ用の描画コードを追加します。
//}

// CModelViewerView の印刷
void CModelViewerView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
    AFXPrintPreview(this);
#endif
}

BOOL CModelViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // 既定の印刷準備
    return DoPreparePrinting(pInfo);
}

void CModelViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: 印刷前の特別な初期化処理を追加してください。
}

void CModelViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: 印刷後の後処理を追加してください。
}

void CModelViewerView::ReadSatFile(bool is_add)
{
    CFileDialog file_open_dialog(TRUE);
    file_open_dialog.m_ofn.lpstrFilter = _T("SAT ファイル (*.sat)\0*.sat\0")
        _T("All Files (*.*)\0*.*\0\0");
    file_open_dialog.m_ofn.nFilterIndex = 1;
    file_open_dialog.m_ofn.Flags |= OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    INT_PTR iResponse = file_open_dialog.DoModal();
    if (iResponse == IDOK)
    {
        char* sat_file_name;
        size_t length = wcslen(file_open_dialog.GetPathName().GetBuffer()) + 1;
        sat_file_name = (LPSTR)malloc(length * sizeof(WCHAR));
        WideCharToMultiByte(CP_ACP,    // CODE PAGE: ANSI code page
            0,
            file_open_dialog.GetPathName().GetBuffer(),
            -1,
            sat_file_name,
            (int)length * sizeof(WCHAR),
            NULL,
            NULL);

        ENTITY_LIST read_sat_file; // 読み込むSATファイルENTITY_LIST
        // SAT読み込み処理
        RestoreSAT(sat_file_name, read_sat_file);

        // 追加読み込む処理チェック
        if (!is_add) 
        {
            m_entity_list.clear();
        }
        m_entity_list.add(read_sat_file);

        if (m_display_data)
        {
            delete m_display_data;
            m_display_data = nullptr;
        }

        m_display_data = new AcisMesh::DisplayData;
        if (CreateMeshFromEntityList(m_entity_list, *m_display_data, m_selected_list, m_pmi_list))
        {
            boost::filesystem::path path(sat_file_name);
            CreateEnitiyTreeItems(read_sat_file, path.filename().string(), is_add);
        }

        SetEntityMap(m_entity_list);
    }
}

void CModelViewerView::SetEntityMap(ENTITY_LIST& entity_list)
{
    m_uid_to_face_map.clear();

    for (int i = 0; i < m_entity_list.count(); ++i)
    {
        ENTITY_LIST faces;
        api_get_faces(m_entity_list[i], faces);
        for (int j = 0; j < faces.count(); ++j)
        {
            std::string str_uid = find_attribute(faces[j], "uid");
            if (!str_uid.empty())
            {
                m_uid_to_face_map.insert(std::make_pair(str_uid, faces[j]));
            }
        }
    }

    m_uid_to_edge_map.clear();

    for (int i = 0; i < m_entity_list.count(); ++i)
    {
        ENTITY_LIST edges;
        api_get_edges(m_entity_list[i], edges);
        for (int j = 0; j < edges.count(); ++j)
        {
            std::string str_uid = find_attribute(edges[j], "uid");
            if (!str_uid.empty())
            {
                m_uid_to_edge_map.insert(std::make_pair(str_uid, edges[j]));
            }
        }
    }

    m_uid_to_vertex_map.clear();
    for (int i = 0; i < m_entity_list.count(); ++i)
    {
        ENTITY_LIST vertices;
        api_get_vertices(m_entity_list[i], vertices);
        for (int j = 0; j < vertices.count(); ++j)
        {
            std::string str_uid = find_attribute(vertices[j], "uid");
            if (!str_uid.empty())
            {
                m_uid_to_vertex_map.insert(std::make_pair(str_uid, vertices[j]));
            }
        }
    }
}

COGLJText* CModelViewerView::CreatePMI(AcisMesh::EdgeVertex edgeVertex)
{
    COGLJText* point_text = new COGLJText(_T("Meiryo UI"), 8, COGLJText::OP_LEFTTOP);
    point_text->Format(_T("%6.3f,%6.3f,%6.3f"), edgeVertex.x, edgeVertex.y, edgeVertex.z);
    return point_text;
}

COGLJText* CModelViewerView::CreateEdgePMI(ENTITY* edge)
{
    COGLJText* point_text = nullptr;
    if (is_EDGE(edge))
    {
        const float fFontSize = 14.0f;
        point_text = new COGLJText(_T("Meiryo UI"), fFontSize, COGLJText::OP_LEFTTOP);
        point_text->Format(_T("%6.3f"), ((EDGE*)edge)->length());
    }
    return point_text;
}

COGLJText* CModelViewerView::CreateVertexPMI(double vertex[3])
{
    COGLJText* point_text = nullptr;
    const float fFontSize = 14.0f;
    point_text = new COGLJText(_T("Meiryo UI"), fFontSize, COGLJText::OP_LEFTTOP);
    point_text->Format(_T("%6.3f,%6.3f,%6.3f"), vertex[0], vertex[1], vertex[2]);
    return point_text;
}

static void CheckChild(CTreeCtrl* pTree, HTREEITEM hParent, BOOL bCheck)
{
    HTREEITEM hItem = pTree->GetChildItem(hParent);
    while (hItem != NULL)
    {
        pTree->SetCheck(hItem, bCheck);
        CheckChild(pTree, hItem, bCheck);
        hItem = pTree->GetNextItem(hItem, TVGN_NEXT);
    }
}

void CModelViewerView::CreateEnitiyTreeItems(const ENTITY_LIST& entity_list, std::string file_name, bool is_add)
{
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    if (pFrame)
    {
        // 追加読み込みチェック
        if (!is_add)
        {
            pFrame->m_wndEntityView.m_wndEntityView.DeleteAllItems();
        }

        auto get_tree_item_name = [](ENTITY* ent, const int ent_index, const int children_size)
        {
            std::string entity_name = "ENTITY";
            if (is_BODY(ent))
            {
                entity_name = "BODY";
            }
            else if (is_FACE(ent))
            {
                entity_name = "FACE";
            }
            else if (is_EDGE(ent))
            {
                entity_name = "EDGE";
            }
            else if (is_VERTEX(ent))
            {
                entity_name = "VERTEX";
            }
            char str[255] = "\0";
            std::string uid = find_attribute(ent, "uid");
            if (strlen(uid.c_str()))
            {
                sprintf_s(str, "%s%03d %s(%d)", entity_name.c_str(), ent_index, uid.c_str(), children_size);
            }
            else
            {
                sprintf_s(str, "%s%03d(%d)", entity_name.c_str(), ent_index, children_size);
            }
            return std::string(str);
        };

        const unsigned int data_size = 255;
        wchar_t w_str[data_size + 1] = { 0x00 };
        char str[data_size] = "\0";
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, file_name.c_str(), data_size, w_str, data_size + 1);
        HTREEITEM hRoot = pFrame->m_wndEntityView.m_wndEntityView.InsertItem(w_str, 0, 0);
        pFrame->m_wndEntityView.m_wndEntityView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

        std::string entity_name;

        // entity階層
        for (int i = 0; i < entity_list.count(); ++i)
        {
            HTREEITEM hBody;
            if (is_BODY(entity_list[i]))
            {
                sprintf_s(str, "BODY");

            }
            else
            {
                sprintf_s(str, "NonBODY");
            }
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            hBody = pFrame->m_wndEntityView.m_wndEntityView.InsertItem(w_str, 1, 1, hRoot);

            ENTITY_LIST faces;
            api_get_faces(entity_list[i], faces);
            int faces_count = faces.count();
            if (faces_count > 0)
            {
                // face子階層
                for (int j = 0; j < faces_count; ++j)
                {
                    ENTITY_LIST edges;
                    api_get_edges(faces[j], edges);
                    int edges_count = edges.count();
                    entity_name = get_tree_item_name(faces[j], j, edges_count);
                    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, entity_name.c_str(), data_size, w_str, data_size + 1);
                    HTREEITEM face_root = pFrame->m_wndEntityView.m_wndEntityView.InsertItem(w_str, 1, 1, hBody);
                    pFrame->m_wndEntityView.m_wndEntityView.SetItemData(face_root, DWORD_PTR(faces[j]));

                    CreateEnitiyTreeFaceItems(face_root, faces[j]);
                }
            }
            else 
            {
                sprintf_s(str, "NonFACE");
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM non_root = pFrame->m_wndEntityView.m_wndEntityView.InsertItem(w_str, 1, 1, hBody);

                CreateEnitiyTreeFaceItems(non_root, entity_list[i]);
            }
        }

        pFrame->m_wndEntityView.m_wndEntityView.SetCheck(hRoot);
        CheckChild(&pFrame->m_wndEntityView.m_wndEntityView, hRoot, TRUE);
    }
}

void CModelViewerView::CreateEnitiyTreeFaceItems(HTREEITEM& root, ENTITY* face)
{
    const unsigned int data_size = 255;
    wchar_t w_str[data_size + 1] = { 0x00 };
    char str[data_size] = "\0";
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    std::string entity_name;
    if (pFrame)
    {
        auto get_tree_item_name = [](ENTITY* ent, const int ent_index, const int children_size)
        {
            std::string entity_name = "ENTITY";
            if (is_BODY(ent))
            {
                entity_name = "BODY";
            }
            else if (is_FACE(ent))
            {
                entity_name = "FACE";
            }
            else if (is_EDGE(ent))
            {
                entity_name = "EDGE";
            }
            else if (is_VERTEX(ent))
            {
                entity_name = "VERTEX";
            }
            char str[255] = "\0";
            std::string uid = find_attribute(ent, "uid");
            if (strlen(uid.c_str()))
            {
                sprintf_s(str, "%s%03d %s(%d)", entity_name.c_str(), ent_index, uid.c_str(), children_size);
            }
            else
            {
                sprintf_s(str, "%s%03d(%d)", entity_name.c_str(), ent_index, children_size);
            }
            return std::string(str);
        };

        ENTITY_LIST edges;
        api_get_edges(face, edges);
        int edges_count = edges.count();

        // edge子階層
        for (int k = 0; k < edges_count; ++k)
        {
            EDGE* edge_edge = (EDGE*)edges[k];
            ENTITY_LIST vertices;
            api_get_vertices(edges[k], vertices);
            int vertices_count = vertices.count();
            entity_name = get_tree_item_name(edges[k], k, vertices_count);
            int ret = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, entity_name.c_str(), data_size, w_str, data_size + 1);
            HTREEITEM edge_root = pFrame->m_wndEntityView.m_wndEntityView.InsertItem(w_str, 1, 1, root);
            pFrame->m_wndEntityView.m_wndEntityView.SetItemData(edge_root, DWORD_PTR(edges[k]));

            CreateEnitiyTreeEdgeItems(edge_root, const_cast<const EDGE*&>(edge_edge));
        }
    }
}

void CModelViewerView::CreateEnitiyTreeEdgeItems(HTREEITEM& edge_root, const EDGE*& edge_edge)
{
    const unsigned int data_size = 255;
    wchar_t w_str[data_size + 1] = { 0x00 };
    char str[data_size] = "\0";
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    if (pFrame)
    {
        // 始点
        SPAposition start_pos = edge_edge->start()->geometry()->coords();
        sprintf_s(str, "Vertex(%6.3f, %6.3f, %6.3f)", start_pos.x(), start_pos.y(), start_pos.z());
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM start_pos_root = pFrame->m_wndEntityView.m_wndEntityView.InsertItem(w_str, 1, 1, edge_root);
        pFrame->m_wndEntityView.m_wndEntityView.SetItemData(start_pos_root, DWORD_PTR(edge_edge->start()));

        // 終点
        SPAposition end_pos = edge_edge->end()->geometry()->coords();
        sprintf_s(str, "Vertex(%6.3f, %6.3f, %6.3f)", end_pos.x(), end_pos.y(), end_pos.z());
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM end_pos_root = pFrame->m_wndEntityView.m_wndEntityView.InsertItem(w_str, 1, 1, edge_root);
        pFrame->m_wndEntityView.m_wndEntityView.SetItemData(end_pos_root, DWORD_PTR(edge_edge->end()));

        //// sense
        //std::string sense = "FORWARD";
        //if (edge_edge->sense() == FORWARD) // #define FORWARD FALSE
        //{
        //    sense = "FORWARD";
        //}
        //else if (edge_edge->sense() == REVERSED)// #define REVERSED TRUE
        //{
        //    sense = "REVERSED";
        //}
        //sprintf_s(str, "sense:%s", sense.c_str());
        //MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        //auto tree_item = pFrame->m_wndEntityView.m_wndEntityView.InsertItem(w_str, 1, 1, edge_root);
    }
}

void CModelViewerView::ReadWeldingResultRaw(const std::string str_welding_result, const std::string schema_file_path, bool is_export_edge)
{
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    if (!pFrame)
    {
        return;
    }

    CViewTreeAvro* avroTreeView = &pFrame->m_wndAvroView.m_wndAvroTreeView;
    if (!avroTreeView)
    {
        return;
    }
    
    try
    {
        avro_fr::serialization::read_avro_datafile(str_welding_result, schema_file_path, m_welding_result_raw);

        // メンバー変数セット
        SetFeatures(m_welding_result_raw);
        SetVirtualTopologies(m_welding_result_raw);

        boost::filesystem::path welding_result(str_welding_result);
        // tree生成
        CreateViewTreeAvroItems(m_welding_result_raw, welding_result.filename().string());

        // AVROからSATへの変換
        if (is_export_edge)
        {
            std::string str_result;
            size_t found = str_welding_result.find_last_of("/\\");
            std::string welding_result_directory = str_welding_result.substr(0, found + 1);
            
            ENTITY_LIST save_entity_list;
            
            for (const auto& welding_edge : m_id_to_virtual_edges)
            {
                auto acis_edges = convert_welding_edge_to_acis_edge(welding_edge.second);
                if (acis_edges)
                {
                    ENTITY_LIST save_entity;
                    if (acis_edges.get().size() != 1)
                    {
                        continue;
                    }
                    auto acis_edge = acis_edges.get()[0];

                    std::string solid_id;
                    api_add_generic_named_attribute(acis_edge, "uid", welding_edge.first.c_str());
                    save_entity_list.add(acis_edge);
                    save_entity.add(acis_edge);
                    std::string save_topology_file_path = welding_result_directory + welding_edge.first + ".sat";
                    SaveSAT(save_topology_file_path.c_str(), &save_entity);
                    str_result += welding_edge.first + ".sat" + "\n";
                }
            }
            std::string save_topology_file_path_total = (welding_result_directory + "welding_edge_to_acis_edge.sat");
            SaveSAT(save_topology_file_path_total.c_str(), &save_entity_list);

            //m_entity_list.add(save_entity_list);

            str_result += "welding_edge_to_acis_edge.sat\n\n";
            str_result += "OutputDirectory:" + welding_result_directory;

            const unsigned int data_size = 1024 * 10;
            wchar_t w_str[data_size + 1] = { 0x00 };
            char str[data_size] = "\0";
            sprintf_s(str, "%s", str_result.c_str());
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            AfxMessageBox(w_str);
        }
    }
    catch (...)
    {
        AfxMessageBox(_T("Read avro file error!"));
    }
}

void CModelViewerView::CreateViewTreeAvroItems(const fr::recognize_welding::RecognizedWeldingResultRaw& welding_result_raw, std::string file_name)
{
    const unsigned int data_size = 255;
    wchar_t w_str[data_size + 1] = { 0x00 };
    char str[data_size] = "\0";
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    if (pFrame)
    {
        pFrame->m_wndAvroView.m_wndAvroTreeView.DeleteAllItems();
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, file_name.c_str(), data_size, w_str, data_size + 1);
        HTREEITEM avroRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 0, 0);

        CreateViewTreeFeatures(avroRoot, welding_result_raw);

        CreateViewTreeVirtualTopologies(avroRoot, welding_result_raw);

        CreateViewAssemblyTree(avroRoot, welding_result_raw);
    }
}

void CModelViewerView::SetFeatures(const fr::recognize_welding::RecognizedWeldingResultRaw& welding_result_raw)
{
    // LineWeldingPartのマップ
    for (const auto& feature : welding_result_raw.features)
    {
        std::string part_id = "";
        if (avro_fr::union_util::is_same_type<fr::recognize_welding::SheetAssembly>(feature))
        {
            auto part = feature.get_SheetAssembly();
            part_id = part.id;
            m_id_to_sheet_assemblies.insert(std::make_pair(part_id, part));
        }
        else if (avro_fr::union_util::is_same_type<fr::recognize_welding::SideWelding>(feature))
        {
            auto part = feature.get_SideWelding();
            part_id = part.id;
            m_id_to_side_weldings.insert(std::make_pair(part_id, part));
        }
        else if (avro_fr::union_util::is_same_type<fr::recognize_welding::SurfaceWelding>(feature))
        {
            auto part = feature.get_SurfaceWelding();
            part_id = part.id;
            m_id_to_surface_weldings.insert(std::make_pair(part_id, part));
        }
        else if (avro_fr::union_util::is_same_type<fr::recognize_welding::LineWeldingPart>(feature))
        {
            auto part = feature.get_LineWeldingPart();
            part_id = part.id;
            m_id_to_line_welding_parts.insert(std::make_pair(part_id, part));
        }
        else if (avro_fr::union_util::is_same_type<fr::recognize_welding::NonWeldingPart>(feature))
        {
            auto part = feature.get_NonWeldingPart();
            part_id = part.id;
            m_id_to_non_welding_parts.insert(std::make_pair(part_id, part));
        }
        else if (avro_fr::union_util::is_same_type<fr::recognize_welding::SurfaceWeldingPart>(feature))
        {
            auto part = feature.get_SurfaceWeldingPart();
            part_id = part.id;
            m_id_to_suface_welding_parts.insert(std::make_pair(part_id, part));
        }
        m_id_to_features.insert(std::make_pair(part_id, feature));
    }
}

void CModelViewerView::SetVirtualTopologies(const fr::recognize_welding::RecognizedWeldingResultRaw& welding_result_raw)
{
    for (const auto& topology : welding_result_raw.virtualTopologies.faces)
    {
        m_id_to_virtual_faces.insert(std::make_pair(topology.id, &topology));
    }

    for (const auto& topology : welding_result_raw.virtualTopologies.loops)
    {
        m_id_to_virtual_loops.insert(std::make_pair(topology.id, &topology));
    }

    for (const auto& topology : welding_result_raw.virtualTopologies.coedges)
    {
        m_id_to_virtual_coedges.insert(std::make_pair(topology.id, &topology));
    }

    // 仮想edgeリスト取得し、マップにセット
    for (const auto& edge : welding_result_raw.virtualTopologies.edges)
    {
        m_id_to_virtual_edges.insert(std::make_pair(edge.id, &edge));
    }

    // 仮想vertexリスト取得し、マップにセット
    for (const auto& vertex : welding_result_raw.virtualTopologies.vertices)
    {
        m_id_to_virtual_vertices.insert(std::make_pair(vertex.id, &vertex));
    }
}

void CModelViewerView::CreateViewTreeFeatures(HTREEITEM& avroTreeRoot, const fr::recognize_welding::RecognizedWeldingResultRaw& welding_result_raw)
{
    const unsigned int data_size = 255;
    wchar_t w_str[data_size + 1] = { 0x00 };
    char str[data_size] = "\0";
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    if (pFrame)
    {
        int count_size = (int)welding_result_raw.features.size();
        sprintf_s(str, "Features(%d)", count_size);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM featureRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, avroTreeRoot);
        for (int i = 0; i < count_size; ++i)
        {
            auto& feature = welding_result_raw.features[i];
            if (avro_fr::union_util::is_same_type<fr::recognize_welding::SheetAssembly>(feature)) // SheetAssembly
            {
                auto part = feature.get_SheetAssembly();
                sprintf_s(str, "SheetAssembly%03d %s", i,part.id.c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM partRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, featureRoot);

                // id
                sprintf_s(str, "Id:%s", part.id.c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM idRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, partRoot);
            }
            else if (avro_fr::union_util::is_same_type<fr::recognize_welding::SideWelding>(feature)) // SideWelding
            {
                auto part = feature.get_SideWelding();
                sprintf_s(str, "SideWelding%03d %s", i, part.id.c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM partRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, featureRoot);

                // id
                sprintf_s(str, "Id:%s", part.id.c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM idRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, partRoot);

                // primaryFeature
                sprintf_s(str, "primaryFeature:%s", part.primaryFeature.c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM primaryFeatureRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, partRoot);

                // secondaryFeature
                sprintf_s(str, "secondaryFeature:%s", part.secondaryFeature.c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM secondaryFeatureRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, partRoot);

                // topPart
                sprintf_s(str, "topPart:%s", part.topPart.c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM topPartRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, partRoot);

                // bottomPart
                sprintf_s(str, "bottomPart:%s", part.bottomPart.c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM bottomPartRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, partRoot);
            }
            else if (avro_fr::union_util::is_same_type<fr::recognize_welding::SurfaceWelding>(feature)) // SurfaceWelding
            {
                auto part = feature.get_SurfaceWelding();
                sprintf_s(str, "SurfaceWelding%03d %s", i, part.id.c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM partRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, featureRoot);

                // id
                sprintf_s(str, "Id:%s", part.id.c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM idRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, partRoot);

                // primaryFeature
                sprintf_s(str, "primaryFeature:%s", part.primaryFeature.c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM primaryFeatureRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, partRoot);

                // secondaryFeature
                sprintf_s(str, "secondaryFeature:%s", part.secondaryFeature.c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM secondaryFeatureRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, partRoot);

                // parts
                int partsSize = (int)part.parts.size();
                sprintf_s(str, "Parts(%d)", partsSize);
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM partsRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, partRoot);
                for (int pi = 0; pi < partsSize; ++pi)
                {
                    sprintf_s(str, "%s", part.parts.at(pi).c_str());
                    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                    HTREEITEM partRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, partsRoot);
                }
            }
            else if (avro_fr::union_util::is_same_type<fr::recognize_welding::LineWeldingPart>(feature)) // LineWeldingPart
            {
                auto& part = feature.get_LineWeldingPart();
                sprintf_s(str, "LineWeldingPart%03d %s", i, part.id.c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM partRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, featureRoot);
                CreateViewLineWeldingPart(partRoot, part);
            }
            else if (avro_fr::union_util::is_same_type<fr::recognize_welding::SurfaceWeldingPart>(feature)) // SurfaceWeldingPart
            {
                auto part = feature.get_SurfaceWeldingPart();
                sprintf_s(str, "SurfaceWeldingPart%03d %s", i, part.id.c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM partRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, featureRoot);

                // id
                sprintf_s(str, "Id:%s", part.id.c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM idRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, partRoot);
            }
            else if (avro_fr::union_util::is_same_type<fr::recognize_welding::NonWeldingPart>(feature)) // NonWeldingPart
            {
                auto part = feature.get_NonWeldingPart();
                sprintf_s(str, "NonWeldingPart%03d %s", i, part.id.c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM partRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, featureRoot);
            }
        }
    }
}

void CModelViewerView::CreateViewTreeVirtualTopologies(HTREEITEM& avroTreeRoot, const fr::recognize_welding::RecognizedWeldingResultRaw& welding_result_raw)
{
    const unsigned int data_size = 255;
    wchar_t w_str[data_size + 1] = { 0x00 };
    char str[data_size] = "\0";
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    if (pFrame)
    {
        sprintf_s(str, "VirtualTopologies");
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM virtualTopologiesRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, avroTreeRoot);

        // faces
        //int count_size = (int)welding_result_raw.virtualTopologies.faces.size();
        int count_size = (int)m_id_to_virtual_faces.size();
        sprintf_s(str, "Faces(%d)", count_size);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM facesRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, virtualTopologiesRoot);
        for (int ii = 0; ii < count_size; ++ii)
        {
            fr::recognize_welding::Face face = welding_result_raw.virtualTopologies.faces[ii];

            sprintf_s(str, "Face%03d %s", ii, face.id.c_str());
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            HTREEITEM faceRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, facesRoot);
            pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(faceRoot, DWORD_PTR(&welding_result_raw.virtualTopologies.faces[ii].id));
            CreateViewVirtualFace(faceRoot, face);
        }

        // loops
        //count_size = (int)welding_result_raw.virtualTopologies.loops.size();
        count_size = (int)m_id_to_virtual_loops.size();
        sprintf_s(str, "Loops(%d)", count_size);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM loopsRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, virtualTopologiesRoot);
        for (int ii = 0; ii < count_size; ++ii)
        {
            fr::recognize_welding::Loop loop = welding_result_raw.virtualTopologies.loops[ii];
            sprintf_s(str, "Loop%03d %s", ii, loop.id.c_str());
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            HTREEITEM loopRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, loopsRoot);
            pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(loopRoot, DWORD_PTR(&welding_result_raw.virtualTopologies.loops[ii].id));
            CreateViewVirtualLoop(loopRoot, loop);
        }

        // coedges
        //count_size = (int)welding_result_raw.virtualTopologies.coedges.size();
        count_size = (int)m_id_to_virtual_coedges.size();
        sprintf_s(str, "Coedges(%d)", count_size);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM coedgesRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, virtualTopologiesRoot);
        for (int ii = 0; ii < count_size; ++ii)
        {
            fr::recognize_welding::Coedge coedge = welding_result_raw.virtualTopologies.coedges[ii];
            sprintf_s(str, "Coedge%03d %s", ii, coedge.id.c_str());
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            HTREEITEM coedgeRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, coedgesRoot);
            pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(coedgeRoot, DWORD_PTR(&welding_result_raw.virtualTopologies.coedges[ii].id));
            CreateViewVirtualCoedge(coedgeRoot, coedge);
        }

        // edges
        //count_size = (int)welding_result_raw.virtualTopologies.edges.size();
        count_size = (int)m_id_to_virtual_edges.size();
        sprintf_s(str, "Edges(%d)", count_size);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM edgesRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, virtualTopologiesRoot);
        for (int ii = 0; ii < count_size; ++ii)
        {
            fr::recognize_welding::Edge edge = welding_result_raw.virtualTopologies.edges[ii];
            std::string curve_type = "Edge";
            // curve
            auto curve = edge.curve;
            if (avro_fr::union_util::is_same_type<fr::recognize_welding::Straight>(curve))
            {
                curve_type = "Straight";
            }
            else if (avro_fr::union_util::is_same_type<fr::recognize_welding::Polyline>(curve))
            {
                curve_type = "Polyline";
            }
            else if (avro_fr::union_util::is_same_type<fr::recognize_welding::Ellipse>(curve))
            {
                curve_type = "Ellipse";
            }
            else if (avro_fr::union_util::is_same_type<fr::recognize_welding::Circle>(curve))
            {
                curve_type = "Circle";
            }

            sprintf_s(str, "%s%03d %s", curve_type.c_str(), ii, edge.id.c_str());
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            HTREEITEM edgeRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, edgesRoot);
            pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(edgeRoot, DWORD_PTR(&welding_result_raw.virtualTopologies.edges[ii].id));
            CreateViewVirtualEdge(edgeRoot, edge);
        }

        // vertices
        //count_size = (int)welding_result_raw.virtualTopologies.vertices.size();
        count_size = (int)m_id_to_virtual_vertices.size();
        sprintf_s(str, "Vertices(%d)", count_size);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM verticesRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, virtualTopologiesRoot);

        for (int ii = 0; ii < count_size; ++ii)
        {
            fr::recognize_welding::Vertex vertex = welding_result_raw.virtualTopologies.vertices[ii];
            sprintf_s(str, "Vertex%03d %s", ii, vertex.id.c_str());
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            HTREEITEM vertexRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, verticesRoot);
            pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(vertexRoot, DWORD_PTR(&welding_result_raw.virtualTopologies.vertices[ii].id));
            CreateViewVirtualVertex(vertexRoot, vertex);
        }
    }
}

void CModelViewerView::CreateViewAssemblyTree(HTREEITEM& avroTreeRoot, const fr::recognize_welding::RecognizedWeldingResultRaw& welding_result_raw)
{
    const unsigned int data_size = 255;
    wchar_t w_str[data_size + 1] = { 0x00 };
    char str[data_size] = "\0";
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    if (pFrame)
    {
        int count_size = 0;
        int i = 0;
        for (auto sheet_assembly : m_id_to_sheet_assemblies)
        {
            sprintf_s(str, "SheetAssembly %s", sheet_assembly.second.id.c_str());
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            HTREEITEM sheetAssemblyRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, avroTreeRoot);

            for (auto& welding_id : sheet_assembly.second.weldings)
            {
                auto side_welding_find_id = m_id_to_side_weldings.find(welding_id);
                if (side_welding_find_id != m_id_to_side_weldings.end())
                {
                    auto& welding = side_welding_find_id->second;
                    sprintf_s(str, "SideWelding %s", welding.id.c_str());
                    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                    HTREEITEM weldingRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, sheetAssemblyRoot);
                    pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(weldingRoot, DWORD_PTR(&welding.id));

                    auto& primary_feature_id = welding.primaryFeature;
                    sprintf_s(str, "PrimaryFeature %s", primary_feature_id.c_str());
                    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                    HTREEITEM primaryFeatureRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, weldingRoot);
                    pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(primaryFeatureRoot, DWORD_PTR(&primary_feature_id));

                    auto& secondary_feature_id = welding.secondaryFeature;
                    sprintf_s(str, "SecondaryFeature %s", secondary_feature_id.c_str());
                    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                    HTREEITEM secondaryFeatureRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, weldingRoot);
                    pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(secondaryFeatureRoot, DWORD_PTR(&secondary_feature_id));

                    sprintf_s(str, "TopPart %s", welding.topPart.c_str());
                    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                    HTREEITEM topPartRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, weldingRoot);
                    pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(topPartRoot, DWORD_PTR(&welding.topPart));
                    auto find_it = m_id_to_line_welding_parts.find(welding.topPart);
                    if (find_it != m_id_to_line_welding_parts.end())
                    {
                        auto& part = find_it->second;
                        sprintf_s(str, "LineWeldingPart %s", part.id.c_str());
                        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                        HTREEITEM partRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, topPartRoot);
                        pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(partRoot, DWORD_PTR(&part.id));
                        CreateViewLineWeldingPart(partRoot, find_it->second);
                    }
                    else 
                    {
                        auto non_find_it = m_id_to_non_welding_parts.find(welding.topPart);
                        if (non_find_it != m_id_to_non_welding_parts.end())
                        {
                            auto& part = non_find_it->second;
                            sprintf_s(str, "NonWeldingPart %s", part.id.c_str());
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                            HTREEITEM partRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, topPartRoot);
                            pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(partRoot, DWORD_PTR(&part.id));
                        }
                    }

                    sprintf_s(str, "BottomPart %s", welding.bottomPart.c_str());
                    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                    HTREEITEM bottomPartRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, weldingRoot);
                    pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(bottomPartRoot, DWORD_PTR(&welding.bottomPart));
                    find_it = m_id_to_line_welding_parts.find(welding.bottomPart);
                    if (find_it != m_id_to_line_welding_parts.end())
                    {
                        auto& part = find_it->second;
                        sprintf_s(str, "LineWeldingPart %s", part.id.c_str());
                        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                        HTREEITEM partRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, bottomPartRoot);
                        pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(partRoot, DWORD_PTR(&part.id));
                        CreateViewLineWeldingPart(partRoot, find_it->second);
                    }
                    else
                    {
                        auto non_find_it = m_id_to_non_welding_parts.find(welding.bottomPart);
                        if (non_find_it != m_id_to_non_welding_parts.end())
                        {
                            auto& part = non_find_it->second;
                            sprintf_s(str, "NonWeldingPart %s", part.id.c_str());
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                            HTREEITEM partRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, bottomPartRoot);
                            pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(partRoot, DWORD_PTR(&part.id));
                        }
                    }

                    sprintf_s(str, "SideWeldingType:%s", welding.sideWeldingType.c_str());
                    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                    HTREEITEM sideWeldingTypeRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, weldingRoot);

                    sprintf_s(str, "IsBothSideOnly:%d", (int)welding.isBothSideOnly);
                    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                    HTREEITEM isBothSideOnlyRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, weldingRoot);
                }
                // SurfaceWelding
                auto surface_welding_find_id = m_id_to_surface_weldings.find(welding_id);
                if (surface_welding_find_id != m_id_to_surface_weldings.end())
                {
                    auto& welding = surface_welding_find_id->second;
                    sprintf_s(str, "SurfaceWelding %s",  welding.id.c_str());
                    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                    HTREEITEM weldingRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, sheetAssemblyRoot);
                    pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(weldingRoot, DWORD_PTR(&welding.id));

                    auto& primary_feature_id = welding.primaryFeature;
                    sprintf_s(str, "PrimaryFeature %s", primary_feature_id.c_str());
                    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                    HTREEITEM primaryFeatureRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, weldingRoot);
                    pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(primaryFeatureRoot, DWORD_PTR(&primary_feature_id));

                    auto& secondary_feature_id = welding.secondaryFeature;
                    sprintf_s(str, "SecondaryFeature %s", secondary_feature_id.c_str());
                    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                    HTREEITEM secondaryFeatureRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, weldingRoot);
                    pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(secondaryFeatureRoot, DWORD_PTR(&secondary_feature_id));

                    int count_size = (int)welding.parts.size();
                    sprintf_s(str, "Parts(%d)", count_size);
                    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                    HTREEITEM partsRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, weldingRoot);
                    for (int ii = 0; ii < count_size; ++ii)
                    {
                        auto suface_welding_part_find_it = m_id_to_suface_welding_parts.find(welding.parts[ii]);
                        if (suface_welding_part_find_it != m_id_to_suface_welding_parts.end())
                        {
                            auto& part = suface_welding_part_find_it->second;
                            sprintf_s(str, "SurfaceWeldingPart%03d %s", ii, part.id.c_str());
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                            HTREEITEM surfaceWeldingPartRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, partsRoot);
                            pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(surfaceWeldingPartRoot, DWORD_PTR(&part.id));
                            CreateViewSurfaceWeldingPart(surfaceWeldingPartRoot, part);
                        }

                        auto line_welding_find_it = m_id_to_line_welding_parts.find(welding.parts[ii]);
                        if (line_welding_find_it != m_id_to_line_welding_parts.end())
                        {
                            auto& part = line_welding_find_it->second;
                            sprintf_s(str, "LineWeldingPart%03d %s", ii, part.id.c_str());
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                            HTREEITEM partRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, partsRoot);
                            pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(partRoot, DWORD_PTR(&part.id));
                            CreateViewLineWeldingPart(partRoot, part);
                        }
                    }
                }
            }
        }
    }
}

void CModelViewerView::CreateViewSurfaceWeldingPart(HTREEITEM& root, const fr::recognize_welding::SurfaceWeldingPart& part)
{
    const unsigned int data_size = 255;
    wchar_t w_str[data_size + 1] = { 0x00 };
    char str[data_size] = "\0";
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    if (pFrame)
    {
        sprintf_s(str, "PrimaryFace %s", part.primaryFace.c_str());
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM primaryFaceRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
        pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(primaryFaceRoot, DWORD_PTR(&part.primaryFace));

        sprintf_s(str, "SecondaryFace %s", part.secondaryFace.c_str());
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM secondaryFaceRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
        pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(secondaryFaceRoot, DWORD_PTR(&part.secondaryFace));
    }
}

void CModelViewerView::CreateViewLineWeldingPart(HTREEITEM& root, const fr::recognize_welding::LineWeldingPart& part)
{
    const unsigned int data_size = 255;
    wchar_t w_str[data_size + 1] = { 0x00 };
    char str[data_size] = "\0";
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    if (pFrame)
    {
        sprintf_s(str, "Primary");
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM primaryRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
        {
            // Edge
            sprintf_s(str, "Edge %s", part.primaryEdge.c_str());
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            HTREEITEM primaryEdgeRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, primaryRoot);
            pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(primaryEdgeRoot, DWORD_PTR(&part.primaryEdge));

            auto edge_find_it = m_id_to_virtual_edges.find(part.primaryEdge);
            if (edge_find_it != m_id_to_virtual_edges.end())
            {
                const fr::recognize_welding::Edge* edge = edge_find_it->second;
                CreateViewVirtualEdge(primaryEdgeRoot, *edge);
            }

            // Faces
            int primaryFacesSize = (int)part.primaryFaces.size();
            sprintf_s(str, "Faces(%d)", primaryFacesSize);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            HTREEITEM primaryFacesRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, primaryRoot);
            for (int fi = 0; fi < primaryFacesSize; ++fi)
            {
                sprintf_s(str, "%s", part.primaryFaces.at(fi).c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM primaryFaceRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, primaryFacesRoot);
                pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(primaryFaceRoot, DWORD_PTR(&part.primaryFaces.at(fi)));
            }

            // ProximityFaces
            int facesProximityToPrimaryEdgeSize = (int)part.facesProximityToPrimaryEdge.size();
            sprintf_s(str, "ProximityFaces(%d)", facesProximityToPrimaryEdgeSize);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            HTREEITEM facesProximityToPrimaryEdgeRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, primaryRoot);
            for (int fi = 0; fi < facesProximityToPrimaryEdgeSize; ++fi)
            {
                sprintf_s(str, "%s", part.facesProximityToPrimaryEdge.at(fi).c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM primaryProximityFaceRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, facesProximityToPrimaryEdgeRoot);
                pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(primaryProximityFaceRoot, DWORD_PTR(&part.facesProximityToPrimaryEdge.at(fi)));
            }
        }

        sprintf_s(str, "Secondary");
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM secondaryRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
        {
            // secondaryEdge
            sprintf_s(str, "Edge %s", part.secondaryEdge.c_str());
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            HTREEITEM secondaryEdgeRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, secondaryRoot);
            auto edge_find_it = m_id_to_virtual_edges.find(part.secondaryEdge);
            if(edge_find_it != m_id_to_virtual_edges.end())
            {
                const fr::recognize_welding::Edge* edge = edge_find_it->second;
                CreateViewVirtualEdge(secondaryEdgeRoot, *edge);
            }

            // secondaryFaces
            int secondaryFacesRootSize = (int)part.secondaryFaces.size();
            sprintf_s(str, "Faces(%d)", secondaryFacesRootSize);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            HTREEITEM secondaryFacesRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, secondaryRoot);
            for (int fi = 0; fi < secondaryFacesRootSize; ++fi)
            {
                sprintf_s(str, "%s", part.secondaryFaces.at(fi).c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM secondaryFaceRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, secondaryFacesRoot);
                pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(secondaryFaceRoot, DWORD_PTR(&part.secondaryFaces.at(fi)));
            }

            // facesProximityToSecondaryEdge
            int facesProximityToSecondaryEdgeSize = (int)part.facesProximityToSecondaryEdge.size();
            sprintf_s(str, "ProximityFaces(%d)", facesProximityToSecondaryEdgeSize);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            HTREEITEM facesProximityToSecondaryEdgeRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, secondaryRoot);
            for (int fi = 0; fi < facesProximityToSecondaryEdgeSize; ++fi)
            {
                sprintf_s(str, "%s", part.facesProximityToSecondaryEdge.at(fi).c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM secondaryProximityFaceRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, facesProximityToSecondaryEdgeRoot);
                pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(secondaryProximityFaceRoot, DWORD_PTR(&part.facesProximityToSecondaryEdge.at(fi)));
            }
        }

        // length
        sprintf_s(str, "Length:%f", part.length);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM lengthRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

        // holeFeatureId
        sprintf_s(str, "holeFeatureId %s", part.holeFeatureId.c_str());
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM holeFeatureIdRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

        // facesRelatedToEdge
        int facesRelatedToEdgeSize = (int)part.facesRelatedToEdge.size();
        sprintf_s(str, "FacesRelatedToEdge(%d)", facesRelatedToEdgeSize);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM facesRelatedToEdgeRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
        for (int pi = 0; pi < facesRelatedToEdgeSize; ++pi)
        {
            sprintf_s(str, "%s", part.facesRelatedToEdge.at(pi).c_str());
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            HTREEITEM facesRelatedToEdge = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, facesRelatedToEdgeRoot);
            pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(facesRelatedToEdge, DWORD_PTR(&part.facesRelatedToEdge.at(pi)));
        }

        // isEnabledGrooveWelding
        sprintf_s(str, "IsEnabledGrooveWelding:%d", part.isEnabledGrooveWelding);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM isEnabledGrooveWeldingRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

        // isEnabledGrooveWelding
        sprintf_s(str, "IgnorableReasons");
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM ignorableReasonsRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
        for (int pi = 0; pi < part.ignorableReasons.size(); ++pi)
        {
            sprintf_s(str, "%s", part.ignorableReasons.at(pi).c_str());
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            HTREEITEM ignorableReasonRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, ignorableReasonsRoot);
        }
    }
}

void CModelViewerView::CreateViewPrimaryOrSecondary(HTREEITEM& root, const fr::recognize_welding::LineWeldingPart& part)
{
    const unsigned int data_size = 255;
    wchar_t w_str[data_size + 1] = { 0x00 };
    char str[data_size] = "\0";
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    if (pFrame)
    {
        // Edge
        sprintf_s(str, "Edge %s", part.primaryEdge.c_str());
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM primaryEdgeRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
        pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(primaryEdgeRoot, DWORD_PTR(&part.primaryEdge));

        auto edge_find_it = m_id_to_virtual_edges.find(part.primaryEdge);
        if (edge_find_it != m_id_to_virtual_edges.end())
        {
            const fr::recognize_welding::Edge* edge = edge_find_it->second;
            CreateViewVirtualEdge(primaryEdgeRoot, *edge);
        }

        // Faces
        int primaryFacesSize = (int)part.primaryFaces.size();
        sprintf_s(str, "Faces(%d)", primaryFacesSize);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM primaryFacesRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
        for (int fi = 0; fi < primaryFacesSize; ++fi)
        {
            sprintf_s(str, "%s", part.primaryFaces.at(fi).c_str());
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            HTREEITEM primaryFaceRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, primaryFacesRoot);
            pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(primaryFaceRoot, DWORD_PTR(&part.primaryFaces.at(fi)));
        }

        // ProximityFaces
        int facesProximityToPrimaryEdgeSize = (int)part.facesProximityToPrimaryEdge.size();
        sprintf_s(str, "ProximityFaces(%d)", facesProximityToPrimaryEdgeSize);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM facesProximityToPrimaryEdgeRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
        for (int fi = 0; fi < facesProximityToPrimaryEdgeSize; ++fi)
        {
            sprintf_s(str, "%s", part.facesProximityToPrimaryEdge.at(fi).c_str());
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            HTREEITEM primaryProximityFaceRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, facesProximityToPrimaryEdgeRoot);
            pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(primaryProximityFaceRoot, DWORD_PTR(&part.facesProximityToPrimaryEdge.at(fi)));
        }
    }

}

void CModelViewerView::CreateViewVirtualFace(HTREEITEM& root, const fr::recognize_welding::Face& face)
{
    const unsigned int data_size = 255;
    wchar_t w_str[data_size + 1] = { 0x00 };
    char str[data_size] = "\0";
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    int count_size = 0;
    if (pFrame)
    {
        // surface
        sprintf_s(str, "Surface");
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM surfaceRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
        
        // loops
        count_size = (int)face.loops.size();
        sprintf_s(str, "Loops(%d)", count_size);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM loopsRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
        {
            for (int ii = 0; ii < count_size; ++ii)
            {
                auto loop = face.loops.at(ii);
                sprintf_s(str, "Loop%03d %s", ii, loop.c_str());
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                HTREEITEM loopRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, loopsRoot);
                auto find_it = m_id_to_virtual_loops.find(loop);
                if (find_it != m_id_to_virtual_loops.end())
                {
                    CreateViewVirtualLoop(loopRoot, *find_it->second);
                }
            }
        }

        // area
        sprintf_s(str, "Area:%06.3f", face.area);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM areaRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

        // sense
        sprintf_s(str, "Sense:%d", face.sense);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM senseRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

    }
}

void CModelViewerView::CreateViewVirtualLoop(HTREEITEM& root, const fr::recognize_welding::Loop& loop)
{
    const unsigned int data_size = 255;
    wchar_t w_str[data_size + 1] = { 0x00 };
    char str[data_size] = "\0";
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    int count_size = 0;
    if (pFrame)
    {
        // face
        sprintf_s(str, "Face:%s", loop.face.c_str());
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM faceRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

        // loops
        count_size = (int)loop.coedges.size();
        sprintf_s(str, "Coedges(%d)", count_size);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM coedgesRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

        for (int ii = 0; ii < count_size; ++ii)
        {
            auto coedge = loop.coedges.at(ii);
            sprintf_s(str, "Coedge%03d %s", ii , coedge.c_str());
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            HTREEITEM coedgeRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, coedgesRoot);
            auto find_it = m_id_to_virtual_coedges.find(coedge);
            if (find_it != m_id_to_virtual_coedges.end())
            {
                CreateViewVirtualCoedge(coedgeRoot, *find_it->second);
            }
        }
    }
}

void CModelViewerView::CreateViewVirtualCoedge(HTREEITEM& root, const fr::recognize_welding::Coedge& coedge)
{
    const unsigned int data_size = 255;
    wchar_t w_str[data_size + 1] = { 0x00 };
    char str[data_size] = "\0";
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    int count_size = 0;
    if (pFrame)
    {
        // edge
        sprintf_s(str, "Edge:%s", coedge.edge.c_str());
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM edgeRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
        auto find_it = m_id_to_virtual_edges.find(coedge.edge);
        if (find_it != m_id_to_virtual_edges.end())
        {
            CreateViewVirtualEdge(edgeRoot, *find_it->second);
        }

        // loop
        sprintf_s(str, "Loop:%s", coedge.loop.c_str());
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM loopRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

        // loop
        sprintf_s(str, "Sense:%d", coedge.sense);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM senseRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
    }
}

void CModelViewerView::CreateViewVirtualEdge(HTREEITEM& root, const fr::recognize_welding::Edge& edge)
{
    const unsigned int data_size = 255;
    wchar_t w_str[data_size + 1] = { 0x00 };
    char str[data_size] = "\0";
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    if (pFrame)
    {
        std::string curve_type = "Edge";
        // curve
        auto curve = edge.curve;
        fr::recognize_welding::UnitVector3D direction;
        fr::recognize_welding::Point3D rootPoint;
        double paramScale = 0.0;
        if (avro_fr::union_util::is_same_type<fr::recognize_welding::Straight>(curve))
        {
            curve_type = "Straight";
            auto straight = curve.get_Straight();
            direction = straight.direction;
            rootPoint = straight.rootPoint;
            paramScale = straight.paramScale;
        }
        else if (avro_fr::union_util::is_same_type<fr::recognize_welding::Polyline>(curve))
        {
            curve_type = "Polyline";
        }
        else if (avro_fr::union_util::is_same_type<fr::recognize_welding::Ellipse>(curve))
        {
            curve_type = "Ellipse";
        }
        else if (avro_fr::union_util::is_same_type<fr::recognize_welding::Circle>(curve))
        {
            curve_type = "Circle";
        }

        sprintf_s(str, "%s", curve_type.c_str());
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM curveRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
        CreateViewVirtualCurve(curveRoot, curve);

        // start
        sprintf_s(str, "Start:%s", edge.start.c_str());
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM startRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
        pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(startRoot, DWORD_PTR(&edge.start));
        auto find_it = m_id_to_virtual_vertices.find(edge.start);
        if(find_it != m_id_to_virtual_vertices.end())
        {
            auto vertex = find_it->second;
            CreateViewVirtualVertex(startRoot, *vertex);
        }

        // end
        sprintf_s(str, "End:%s", edge.end.c_str());
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM endRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
        pFrame->m_wndAvroView.m_wndAvroTreeView.SetItemData(endRoot, DWORD_PTR(&edge.end));
        find_it = m_id_to_virtual_vertices.find(edge.end);
        if (find_it != m_id_to_virtual_vertices.end())
        {
            auto vertex = find_it->second;
            CreateViewVirtualVertex(endRoot, *vertex);
        }

        // paramRange
        sprintf_s(str, "ParamRange:[%f,%f]", edge.paramRange.start.get_double(), edge.paramRange.end.get_double());
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM paramRangeRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

        // sense
        sprintf_s(str, "Sense:%d", edge.sense);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM senseRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
    }
}


void CModelViewerView::CreateViewVirtualCurve(HTREEITEM& root, const fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__6__& curve)
{
    const unsigned int data_size = 255;
    wchar_t w_str[data_size + 1] = { 0x00 };
    char str[data_size] = "\0";
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    if (pFrame)
    {
        std::string curve_type = "Edge";
        if (avro_fr::union_util::is_same_type<fr::recognize_welding::Straight>(curve))
        {
            curve_type = "Straight";
            fr::recognize_welding::UnitVector3D direction;
            fr::recognize_welding::Point3D rootPoint;
            auto straight = curve.get_Straight();
            double paramScale = 0.0;
            direction = straight.direction;
            rootPoint = straight.rootPoint;
            paramScale = straight.paramScale;

            // Geometry direction
            sprintf_s(str, "Direction:%06.3f,%06.3f,%06.3f", direction.x, direction.y, direction.z);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
            // Geometry rootPoint
            sprintf_s(str, "RootPoint:%06.3f,%06.3f,%06.3f", rootPoint.x, rootPoint.y, rootPoint.z);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
            // Geometry paramScale
            sprintf_s(str, "ParamScale:%f", paramScale);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
        }
        else if (avro_fr::union_util::is_same_type<fr::recognize_welding::Polyline>(curve))
        {
            curve_type = "Polyline";
        }
        else if (avro_fr::union_util::is_same_type<fr::recognize_welding::Ellipse>(curve))
        {
            curve_type = "Ellipse";
            auto ellipse = curve.get_Ellipse();
            auto center = ellipse.center;
            auto normal = ellipse.normal;
            auto majorAxis = ellipse.majorAxis;
            auto paramRange = ellipse.paramRange;
            auto radiusRatio = ellipse.radiusRatio;
            auto periodic = ellipse.periodic;
            auto paramOff = ellipse.paramOff;
            
            // center
            sprintf_s(str, "Center:%06.3f,%06.3f,%06.3f", center.x, center.y, center.z);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

            // normal
            sprintf_s(str, "Normal:%06.3f,%06.3f,%06.3f", normal.x, normal.y, normal.z);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

            // majorAxis
            sprintf_s(str, "MajorAxis:%06.3f,%06.3f,%06.3f", majorAxis.x, majorAxis.y, majorAxis.z);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

            // ParamRange
            sprintf_s(str, "ParamRange:[%06.3f,%06.3f]", paramRange.start.get_double(), paramRange.end.get_double());
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

            // radiusRatio
            sprintf_s(str, "RadiusRatio:%06.3f", radiusRatio);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

            // periodic
            sprintf_s(str, "Periodic:%d", periodic);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

            // paramOff
            sprintf_s(str, "ParamOff:%f", paramOff.value);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
        }
        else if (avro_fr::union_util::is_same_type<fr::recognize_welding::Circle>(curve))
        {
            curve_type = "Circle";
            auto circle = curve.get_Circle();

            auto center = circle.center;
            auto normal = circle.normal;
            auto majorAxis = circle.majorAxis;
            auto paramRange = circle.paramRange;
            auto radiusRatio = circle.radiusRatio;
            auto periodic = circle.periodic;
            auto paramOff = circle.paramOff;

            // center
            sprintf_s(str, "Center:%06.3f,%06.3f,%06.3f", center.x, center.y, center.z);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

            // normal
            sprintf_s(str, "Normal:%06.3f,%06.3f,%06.3f", normal.x, normal.y, normal.z);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

            // majorAxis
            sprintf_s(str, "MajorAxis:%06.3f,%06.3f,%06.3f", majorAxis.x, majorAxis.y, majorAxis.z);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

            // ParamRange
            sprintf_s(str, "ParamRange:%06.3f %06.3f", paramRange.start.get_double(), paramRange.end.get_double());
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

            // radiusRatio
            sprintf_s(str, "RadiusRatio:%06.3f", radiusRatio);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

            // periodic
            sprintf_s(str, "Periodic:%d", periodic);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);

            // paramOff
            sprintf_s(str, "ParamOff:%f", paramOff.value);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
        }
    }
}

void CModelViewerView::CreateViewVirtualVertex(HTREEITEM& root, const fr::recognize_welding::Vertex& vertex)
{
    const unsigned int data_size = 255;
    wchar_t w_str[data_size + 1] = { 0x00 };
    char str[data_size] = "\0";
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    if (pFrame)
    {
        sprintf_s(str, "(%06.3f,%06.3f,%06.3f)", vertex.position.x, vertex.position.y, vertex.position.z);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        HTREEITEM vertexRoot = pFrame->m_wndAvroView.m_wndAvroTreeView.InsertItem(w_str, 1, 1, root);
    }
}


void CModelViewerView::CreateViewJsonTreeItems(const char* fileName, json11::Json& jsonContents, bool isAdd)
{
    const unsigned int data_size = 255;
    wchar_t w_str[data_size + 1] = { 0x00 };
    char str[data_size] = "\0";
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    if (pFrame)
    {
        if (!isAdd)
        {
            pFrame->m_wndJsonView.m_wndJsonTreeView.DeleteAllItems();
        }
        
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, fileName, data_size, w_str, data_size + 1);
        HTREEITEM jsonTreeRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 0, 0);

        auto json_contents = jsonContents;
        const char* JSON_KEY_VERSION = "version";
        const char* JSON_KEY_FERTURES = "features";
        const char* JSON_KEY_PRODUCTS = "products";
        const char* JSON_KEY_TOPOLOGIES = "topologies";
        if (json_contents.is_array() && !json_contents.array_items().empty())
        {
            // 一番目はSemanticFeatures
            const auto json_content = json_contents.array_items().at(0);
            const auto& version = json_content[JSON_KEY_VERSION].int_value();
            if (json_content[JSON_KEY_FERTURES].is_array())
            {
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_FERTURES, data_size, w_str, data_size + 1);
                HTREEITEM featuresRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, jsonTreeRoot);

                const auto jsonFeatures = json_content[JSON_KEY_FERTURES].array_items();
                CreateViewJsonTreeFeatures(featuresRoot, jsonFeatures);
            }
        }
    }

}

void CModelViewerView::CreateViewJsonTreeFeatures(HTREEITEM& featuresTreeRoot, const json11::Json& jsonFeatures)
{
    const unsigned int data_size = 255;
    wchar_t w_str[data_size + 1] = { 0x00 };
    char str[data_size] = "\0";
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    if (pFrame)
    {
        const char* JSON_KEY_FEATURE_PRIMITIVE = "Feature.Primitive.";
        const char* JSON_KEY_FEATURE_SEMANTIC = "Feature.Semantic.";

        int primitives_count = 0;
        int semantics_count = 0;

        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "Primitive", data_size, w_str, data_size + 1);
        HTREEITEM primitivesRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featuresTreeRoot);

        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "Semantic", data_size, w_str, data_size + 1);
        HTREEITEM semanticsRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featuresTreeRoot);

        const json11::Json::array features = jsonFeatures.array_items();

        for (int i = 0; i < features.size(); ++i)
        {
            const auto feature = features.at(i);
            if (feature.type() == json11::Json::Type::OBJECT)
            {
                for (auto& feature_item : feature.object_items())
                {
                    std::string feature_key = feature_item.first;
                    auto feature_object = feature_item.second;

                    if (feature_key.find(JSON_KEY_FEATURE_PRIMITIVE) != std::string::npos)
                    {
                        ++primitives_count;
                        size_t index = feature_key.find(JSON_KEY_FEATURE_PRIMITIVE);
                        feature_key.erase(0, strlen(JSON_KEY_FEATURE_PRIMITIVE));
                        feature_key += " ";
                        feature_key += feature_item.second["id"].string_value();
                        feature_key += "(" + std::to_string(primitives_count) + ")";
                        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, feature_key.c_str(), data_size, w_str, data_size + 1);
                        HTREEITEM featureRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, primitivesRoot);

                        for (int ii = 0; ii < feature_object["faces"].array_items().size(); ++ii)
                        {
                            auto face = feature_object["faces"].array_items().at(ii);
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, face.string_value().c_str(), data_size, w_str, data_size + 1);
                            HTREEITEM facesRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                            pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(facesRoot, DWORD_PTR(&face.string_value()));
                        }

                    }
                    else if (feature_key.find(JSON_KEY_FEATURE_SEMANTIC) != std::string::npos)
                    {
                        const char* JSON_KEY_FEATURE_SEMANTIC_TYPE = "_type";
                        ++semantics_count;
                        feature_key = feature_item.second[JSON_KEY_FEATURE_SEMANTIC_TYPE].string_value();
                        feature_key += " ";
                        feature_key += feature_item.second["id"].string_value();
                        feature_key += "(" + std::to_string(semantics_count) + ")";
                        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, feature_key.c_str(), data_size, w_str, data_size + 1);
                        HTREEITEM featureRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, semanticsRoot);

                        // faces
                        const char* JSON_KEY_SEMANTIC_FACES = "faces";
                        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_FACES, data_size, w_str, data_size + 1);
                        HTREEITEM facesRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                        for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_FACES].array_items().size(); ++ii)
                        {
                            auto object_item = feature_object[JSON_KEY_SEMANTIC_FACES].array_items().at(ii);
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                            HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, facesRoot);
                            pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                        }

                        // tops
                        const char* JSON_KEY_SEMANTIC_TOPS = "tops";
                        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_TOPS, data_size, w_str, data_size + 1);
                        HTREEITEM topsRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                        for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_TOPS].array_items().size(); ++ii)
                        {
                            auto object_item = feature_object[JSON_KEY_SEMANTIC_TOPS].array_items().at(ii);
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                            HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, topsRoot);
                            pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                        }

                        // bottoms
                        const char* JSON_KEY_SEMANTIC_BOTTOMS = "bottoms";
                        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_BOTTOMS, data_size, w_str, data_size + 1);
                        HTREEITEM bottomsRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                        for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_BOTTOMS].array_items().size(); ++ii)
                        {
                            auto object_item = feature_object[JSON_KEY_SEMANTIC_BOTTOMS].array_items().at(ii);
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                            HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, bottomsRoot);
                            pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                        }

                        // laterals
                        const char* JSON_KEY_SEMANTIC_LATERALS = "laterals";
                        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_LATERALS, data_size, w_str, data_size + 1);
                        HTREEITEM lateralsRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                        for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_LATERALS].array_items().size(); ++ii)
                        {
                            auto object_item = feature_object[JSON_KEY_SEMANTIC_LATERALS].array_items().at(ii);
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                            HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, lateralsRoot);
                            pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                        }

                        // topEdges
                        const char* JSON_KEY_SEMANTIC_TOPEDGES = "topEdges";
                        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_TOPEDGES, data_size, w_str, data_size + 1);
                        HTREEITEM topEdgesRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                        for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_TOPEDGES].array_items().size(); ++ii)
                        {
                            auto object_item = feature_object[JSON_KEY_SEMANTIC_TOPEDGES].array_items().at(ii);
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                            HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, topEdgesRoot);
                            pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                        }

                        // bottomEdges
                        const char* JSON_KEY_SEMANTIC_BOTTOMEDGES = "bottomEdges";
                        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_BOTTOMEDGES, data_size, w_str, data_size + 1);
                        HTREEITEM bottomEdgesRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                        for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_BOTTOMEDGES].array_items().size(); ++ii)
                        {
                            auto object_item = feature_object[JSON_KEY_SEMANTIC_BOTTOMEDGES].array_items().at(ii);
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                            HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, bottomEdgesRoot);
                            pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                        }

                        // supports
                        const char* JSON_KEY_SEMANTIC_SUPPORTS = "supports";
                        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_SUPPORTS, data_size, w_str, data_size + 1);
                        HTREEITEM supportsRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                        for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_SUPPORTS].array_items().size(); ++ii)
                        {
                            auto object_item = feature_object[JSON_KEY_SEMANTIC_SUPPORTS].array_items().at(ii);
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                            HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, supportsRoot);
                            pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                        }

                        // supportTops
                        const char* JSON_KEY_SEMANTIC_SUPPORTTOPS = "supportTops";
                        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_SUPPORTTOPS, data_size, w_str, data_size + 1);
                        HTREEITEM supportTopsRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                        for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_SUPPORTTOPS].array_items().size(); ++ii)
                        {
                            auto object_item = feature_object[JSON_KEY_SEMANTIC_SUPPORTTOPS].array_items().at(ii);
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                            HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, supportTopsRoot);
                            pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                        }

                        // supportBottoms
                        const char* JSON_KEY_SEMANTIC_SUPPORTBOTTOMS = "supportBottoms";
                        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_SUPPORTBOTTOMS, data_size, w_str, data_size + 1);
                        HTREEITEM supportBottomsRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                        for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_SUPPORTBOTTOMS].array_items().size(); ++ii)
                        {
                            auto object_item = feature_object[JSON_KEY_SEMANTIC_SUPPORTBOTTOMS].array_items().at(ii);
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                            HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, supportBottomsRoot);
                            pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                        }

                        // 特別タイプの対応
                        const char* JSON_KEY_FEATURE_SEMANTIC_TYPE_SHARP_BEND = "SHARP_BEND";
                        const char* JSON_KEY_FEATURE_SEMANTIC_TYPE_BOGO_BEND = "BOGOBEND";
                        const char* JSON_KEY_FEATURE_SEMANTIC_TYPE_PLATE = "PLATE";
                        if (feature_item.second[JSON_KEY_FEATURE_SEMANTIC_TYPE].string_value() == std::string(JSON_KEY_FEATURE_SEMANTIC_TYPE_SHARP_BEND))
                        {
                            // supports1
                            const char* JSON_KEY_SEMANTIC_SUPPORTS1 = "supports1";
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_SUPPORTS1, data_size, w_str, data_size + 1);
                            HTREEITEM supports1Root = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                            for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_SUPPORTS1].array_items().size(); ++ii)
                            {
                                auto object_item = feature_object[JSON_KEY_SEMANTIC_SUPPORTS1].array_items().at(ii);
                                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                                HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, supports1Root);
                                pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                            }

                            // supports2
                            const char* JSON_KEY_SEMANTIC_SUPPORTS2 = "supports2";
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_SUPPORTS2, data_size, w_str, data_size + 1);
                            HTREEITEM supports2Root = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                            for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_SUPPORTS2].array_items().size(); ++ii)
                            {
                                auto object_item = feature_object[JSON_KEY_SEMANTIC_SUPPORTS2].array_items().at(ii);
                                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                                HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, supports2Root);
                                pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                            }
                        }
                        else if (feature_item.second[JSON_KEY_FEATURE_SEMANTIC_TYPE].string_value() == std::string(JSON_KEY_FEATURE_SEMANTIC_TYPE_BOGO_BEND))
                        {
                            // filletFaces
                            const char* JSON_KEY_SEMANTIC_FILLET_FACES = "filletFaces";
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_FILLET_FACES, data_size, w_str, data_size + 1);
                            HTREEITEM filletFacesRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                            for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_FILLET_FACES].array_items().size(); ++ii)
                            {
                                auto object_item = feature_object[JSON_KEY_SEMANTIC_FILLET_FACES].array_items().at(ii);
                                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                                HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, filletFacesRoot);
                                pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                            }

                            // supportFaces
                            const char* JSON_KEY_SEMANTIC_SUPPORT_FACES = "supportFaces";
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_SUPPORT_FACES, data_size, w_str, data_size + 1);
                            HTREEITEM supportFacesRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                            for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_SUPPORT_FACES].array_items().size(); ++ii)
                            {
                                auto object_item = feature_object[JSON_KEY_SEMANTIC_SUPPORT_FACES].array_items().at(ii);
                                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                                HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, supportFacesRoot);
                                pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                            }
                        }
                        else if (feature_item.second[JSON_KEY_FEATURE_SEMANTIC_TYPE].string_value() == std::string(JSON_KEY_FEATURE_SEMANTIC_TYPE_PLATE))
                        {
                            // solidFeatures
                            const char* JSON_KEY_SEMANTIC_SOLID_FEATURES = "solidFeatures";
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_SOLID_FEATURES, data_size, w_str, data_size + 1);
                            HTREEITEM solidFeaturesRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                            for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_SOLID_FEATURES].array_items().size(); ++ii)
                            {
                                auto object_item = feature_object[JSON_KEY_SEMANTIC_SOLID_FEATURES].array_items().at(ii);
                                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                                HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, solidFeaturesRoot);
                                pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                            }

                            // parts
                            const char* JSON_KEY_SEMANTIC_PARTS = "parts";
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_PARTS, data_size, w_str, data_size + 1);
                            HTREEITEM partsRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                            for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_PARTS].array_items().size(); ++ii)
                            {
                                auto object_item = feature_object[JSON_KEY_SEMANTIC_PARTS].array_items().at(ii);
                                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                                HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, partsRoot);
                                pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                            }

                            // machinings
                            const char* JSON_KEY_SEMANTIC_PLATE_MACHININGS = "machinings";
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_PLATE_MACHININGS, data_size, w_str, data_size + 1);
                            HTREEITEM machiningsRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                            for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_PLATE_MACHININGS].array_items().size(); ++ii)
                            {
                                auto object_item = feature_object[JSON_KEY_SEMANTIC_PLATE_MACHININGS].array_items().at(ii);
                                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                                HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, machiningsRoot);
                                pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                            }

                            // walls
                            const char* JSON_KEY_SEMANTIC_PLATE_WALLS = "walls";
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_PLATE_WALLS, data_size, w_str, data_size + 1);
                            HTREEITEM wallsRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                            for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_PLATE_WALLS].array_items().size(); ++ii)
                            {
                                auto object_item = feature_object[JSON_KEY_SEMANTIC_PLATE_WALLS].array_items().at(ii);
                                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                                HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, wallsRoot);
                                pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                            }

                            // bends
                            const char* JSON_KEY_SEMANTIC_PLATE_BENDS = "bends";
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_PLATE_BENDS, data_size, w_str, data_size + 1);
                            HTREEITEM bendsRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                            for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_PLATE_BENDS].array_items().size(); ++ii)
                            {
                                auto object_item = feature_object[JSON_KEY_SEMANTIC_PLATE_BENDS].array_items().at(ii);
                                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                                HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, bendsRoot);
                                pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                            }

                            // holes
                            const char* JSON_KEY_SEMANTIC_PLATE_HOLES = "holes";
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_PLATE_HOLES, data_size, w_str, data_size + 1);
                            HTREEITEM holesRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                            for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_PLATE_HOLES].array_items().size(); ++ii)
                            {
                                auto object_item = feature_object[JSON_KEY_SEMANTIC_PLATE_HOLES].array_items().at(ii);
                                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                                HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, holesRoot);
                                pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                            }

                            // cutouts
                            const char* JSON_KEY_SEMANTIC_PLATE_CUTOUTS = "cutouts";
                            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, JSON_KEY_SEMANTIC_PLATE_CUTOUTS, data_size, w_str, data_size + 1);
                            HTREEITEM cutoutsRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, featureRoot);
                            for (int ii = 0; ii < feature_object[JSON_KEY_SEMANTIC_PLATE_CUTOUTS].array_items().size(); ++ii)
                            {
                                auto object_item = feature_object[JSON_KEY_SEMANTIC_PLATE_CUTOUTS].array_items().at(ii);
                                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, object_item.string_value().c_str(), data_size, w_str, data_size + 1);
                                HTREEITEM itemRoot = pFrame->m_wndJsonView.m_wndJsonTreeView.InsertItem(w_str, 1, 1, cutoutsRoot);
                                pFrame->m_wndJsonView.m_wndJsonTreeView.SetItemData(itemRoot, DWORD_PTR(&object_item.string_value()));
                            }
                        }
                    }
                    else
                    {

                    }
                }

                int pause = 1;
            }
        }
    }
}

void CModelViewerView::CreateViewJsonTreePrimitive(HTREEITEM& featuresTreeRoot, const json11::Json& jsonFeatures)
{
    const char* JSON_KEY_FEATURE_PRIMITIVE = "Feature.Primitive.";
}

void CModelViewerView::CreateViewJsonTreeSemantic(HTREEITEM& featuresTreeRoot, const json11::Json& jsonFeatures)
{


}

void CModelViewerView::RenderModelSat()
{
    if (m_display_data)
    {
        delete m_display_data;
        m_display_data = new AcisMesh::DisplayData;
        CreateMeshFromEntityList(m_entity_list, *m_display_data, m_selected_list, m_pmi_list);
    }

    if (m_display_data)
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);    // アトリビュート復帰できるように、一つ追加
        auto m_data = m_display_data;
        for (unsigned int jj = 0; jj < m_data->faceMesh.size(); jj++)
        {
            const GLfloat red = (GLfloat)m_data->faceMesh[jj].color.red();
            const GLfloat green = (GLfloat)m_data->faceMesh[jj].color.green();
            const GLfloat blue = (GLfloat)m_data->faceMesh[jj].color.blue();
            glColor4f(red, green, blue, m_fFaceAlpha);
            glBegin(GL_TRIANGLES);
            unsigned int nT = (unsigned int)(m_data->faceMesh[jj].numIndices / 3);
            for (unsigned int ii = 0; ii < nT; ii++)
            {
                int ll = 3 * ii + m_data->faceMesh[jj].baseIndex;
                int kk = 3 * m_data->triangles[ll];
                glNormal3f(m_data->normalCoords[kk], m_data->normalCoords[kk + 1], m_data->normalCoords[kk + 2]);
                glVertex3f(m_data->faceCoords[kk], m_data->faceCoords[kk + 1], m_data->faceCoords[kk + 2]);
                kk = 3 * m_data->triangles[ll + 1];
                glNormal3f(m_data->normalCoords[kk], m_data->normalCoords[kk + 1], m_data->normalCoords[kk + 2]);
                glVertex3f(m_data->faceCoords[kk], m_data->faceCoords[kk + 1], m_data->faceCoords[kk + 2]);
                kk = 3 * m_data->triangles[ll + 2];
                glNormal3f(m_data->normalCoords[kk], m_data->normalCoords[kk + 1], m_data->normalCoords[kk + 2]);
                glVertex3f(m_data->faceCoords[kk], m_data->faceCoords[kk + 1], m_data->faceCoords[kk + 2]);
            }
            glEnd();
        }

        glLineWidth(m_fLineWidth);
        for (unsigned int nn = 0; nn < m_data->edgeMesh.size(); nn++)
        {
            const GLfloat red = (GLfloat)m_data->edgeMesh[nn].color.red();
            const GLfloat green = (GLfloat)m_data->edgeMesh[nn].color.green();
            const GLfloat blue = (GLfloat)m_data->edgeMesh[nn].color.blue();
            glColor4f(red, green, blue, m_fLineAlpha);
            glBegin(GL_LINE_STRIP);
            unsigned int nI = (unsigned int)(m_data->edgeMesh[nn].numIndices / 3);
            for (unsigned int ii = 0; ii < nI; ii++)
            {
                int ll = 3 * ii + m_data->edgeMesh[nn].baseVertex;
                glVertex3f(m_data->edgeCoords[ll], m_data->edgeCoords[ll + 1], m_data->edgeCoords[ll + 2]);
            }
            glEnd();
        }

        // DrawArrowd(0, 0, 0, 10, 10, 10);

        glPointSize(m_fPointSize);
        for (unsigned int nn = 0; nn < m_data->vertices.size(); nn++)
        {
            const GLfloat red = (GLfloat)m_data->vertices[nn].color.red();
            const GLfloat green = (GLfloat)m_data->vertices[nn].color.green();
            const GLfloat blue = (GLfloat)m_data->vertices[nn].color.blue();
            const GLfloat x = (GLfloat)m_data->vertices[nn].x;
            const GLfloat y = (GLfloat)m_data->vertices[nn].y;
            const GLfloat z = (GLfloat)m_data->vertices[nn].z;

            glColor4f(red, green, blue, m_fPointAlpha);
            glBegin(GL_POINTS);
            glVertex3f(x, y, z);
            glEnd();
        }
        // PMI表示
        if (Setting::IS_DISPALY_CLICKED_PMI)
        {
            for (int i = 0; i < m_pmi_list.size(); ++i)
            {
                if (m_pmi_list[i])
                {
                    delete m_pmi_list[i];
                }
            }
            m_pmi_list.clear();
            for (int nn = 0; nn < m_selected_list.count(); nn++)
            {
                if (is_EDGE(m_selected_list[nn]))
                {
                    EDGE* edge = dynamic_cast<EDGE*>(m_selected_list[nn]);
                    if (!edge)
                    {
                        continue;
                    }
                    COGLJText* edge_point_text = CreateEdgePMI(edge);
                    if (!edge_point_text)
                    {
                        continue;
                    }

                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos3f((GLfloat)edge->mid_pos().x(), (GLfloat)edge->mid_pos().y(), (GLfloat)edge->mid_pos().z());
                    edge_point_text->Render();
                    m_pmi_list.emplace_back(edge_point_text);

                    // start
                    double start_pos[3] = {
                        edge->start_pos().x(),
                        edge->start_pos().y(),
                        edge->start_pos().z()
                    };
                    COGLJText* edge_start_point_text = CreateVertexPMI(start_pos);
                    if (!edge_start_point_text)
                    {
                        continue;
                    }
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos3f((GLfloat)start_pos[0], (GLfloat)start_pos[1], (GLfloat)start_pos[2]);
                    edge_start_point_text->Render();
                    m_pmi_list.emplace_back(edge_start_point_text);

                    double end_pos[3] = {
                        edge->end_pos().x(),
                        edge->end_pos().y(),
                        edge->end_pos().z()
                    };
                    COGLJText* edge_end_point_text = CreateVertexPMI(end_pos);
                    if (!edge_end_point_text)
                    {
                        continue;
                    }
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos3f((GLfloat)end_pos[0], (GLfloat)end_pos[1], (GLfloat)end_pos[2]);
                    edge_end_point_text->Render();
                    m_pmi_list.emplace_back(edge_end_point_text);
                }
            }
        }
        glPopAttrib();    // アトリビュートの復帰
    }
}

void CModelViewerView::DrawArrowd(double x0, double y0, double z0,
    double x1, double y1, double z1)
{
    GLUquadricObj* arrows[2];
    double x2, y2, z2, len, ang;

    x2 = x1 - x0; y2 = y1 - y0; z2 = z1 - z0;
    len = sqrt(x2 * x2 + y2 * y2 + z2 * z2);
    if (len != 0.0)
    {
        ang = acos(z2 * len / (sqrt(x2 * x2 + y2 * y2 + z2 * z2) * len)) / M_PI * 180.0;

        glPushMatrix();
        glTranslated(x0, y0, z0);
        glRotated(ang, -y2 * len, x2 * len, 0.0);
        arrows[0] = gluNewQuadric();
        gluQuadricDrawStyle(arrows[0], GLU_FILL);
        gluCylinder(arrows[0], len / 80, len / 80, len * 0.9, 8, 8);
        glPushMatrix();
        glTranslated(0.0, 0.0, len * 0.9);
        arrows[1] = gluNewQuadric();
        gluQuadricDrawStyle(arrows[1], GLU_FILL);
        gluCylinder(arrows[1], len / 30, 0.0f, len / 10, 8, 8);
        glPopMatrix();
        glPopMatrix();
    }
}

void CModelViewerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
    ClientToScreen(&point);
    OnContextMenu(this, point);
}

void CModelViewerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
    theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CModelViewerView の診断

#ifdef _DEBUG
void CModelViewerView::AssertValid() const
{
    COpenGLPickView::AssertValid();
}

void CModelViewerView::Dump(CDumpContext& dc) const
{
    COpenGLPickView::Dump(dc);
}

CModelViewerDoc* CModelViewerView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CModelViewerDoc)));
    return (CModelViewerDoc*)m_pDocument;
}
#endif //_DEBUG

std::string CModelViewerView::GetPropertyById(std::string strId) const
{
    std::string strProperty = "Null";
    auto find_sheet_assembly_it = m_id_to_sheet_assemblies.find(strId);
    if (find_sheet_assembly_it != m_id_to_sheet_assemblies.end())
    {
        auto feature = find_sheet_assembly_it->second;
        strProperty = "";
        strProperty += "SheetAssembly\r\n";
        strProperty += "Id : " + feature.id + "\r\n";
        strProperty += "BoundLow : " + std::to_string(feature.bound.low.x) + "," + std::to_string(feature.bound.low.y) + "," + std::to_string(feature.bound.low.z) + "\r\n";
        strProperty += "BoundHigh : " + std::to_string(feature.bound.high.x) + "," + std::to_string(feature.bound.high.y) + "," + std::to_string(feature.bound.high.z) + "\n";
        strProperty += "AxisPos : " + std::to_string(feature.layoutAxis.pos.x) + "," + std::to_string(feature.layoutAxis.pos.y) + "," + std::to_string(feature.layoutAxis.pos.z) + "\r\n";
        strProperty += "AxisDir : " + std::to_string(feature.layoutAxis.dir.x) + "," + std::to_string(feature.layoutAxis.dir.y) + "," + std::to_string(feature.layoutAxis.dir.z) + "\r\n";
        strProperty += "AxisOrthogonalDir : " + std::to_string(feature.layoutAxis.orthogonalDir.x) + "," + std::to_string(feature.layoutAxis.orthogonalDir.y) + "," + std::to_string(feature.layoutAxis.orthogonalDir.z) + "\r\n";
        strProperty += "likelihood : " + std::to_string(feature.likelihood) + "\r\n";
        for (auto& error : feature.errors)
        {
            strProperty += strProperty + std::to_string(error) + "\r\n";
        }
        return strProperty;
    }

    auto find_side_welding_it = m_id_to_side_weldings.find(strId);
    if (find_side_welding_it != m_id_to_side_weldings.end())
    {
        auto feature = find_side_welding_it->second;
        strProperty = "";
        strProperty += "SideWelding\r\n";
        strProperty += "Id : " + feature.id + "\r\n";
        return strProperty;
    }

    auto find_surface_welding_it = m_id_to_surface_weldings.find(strId);
    if (find_surface_welding_it != m_id_to_surface_weldings.end())
    {
        auto feature = find_surface_welding_it->second;
        strProperty = "";
        strProperty += "SurfaceWelding\r\n";
        strProperty += "Id : " + feature.id + "\r\n";
        return strProperty;
    }


    auto find_line_welding_part_it = m_id_to_line_welding_parts.find(strId);
    if (find_line_welding_part_it != m_id_to_line_welding_parts.end())
    {
        auto feature = find_line_welding_part_it->second;
        strProperty = "";
        strProperty += "LineWeldingPart\r\n";
        strProperty += "Id : " + feature.id + "\r\n";
        strProperty += "Length : " + std::to_string(feature.length) + "\r\n";
        strProperty += "IsEnabledGrooveWelding : " + std::to_string(feature.isEnabledGrooveWelding) + "\r\n";
        return strProperty;
    }

    auto find_surface_welding_part_it = m_id_to_suface_welding_parts.find(strId);
    if (find_surface_welding_part_it != m_id_to_suface_welding_parts.end())
    {
        auto feature = find_surface_welding_part_it->second;
        strProperty = "";
        strProperty += "SurfaceWeldingPart\r\n";
        strProperty += "Id : " + feature.id + "\r\n";
        return strProperty;
    }

    auto find_face_it = m_id_to_virtual_faces.find(strId);
    if (find_face_it != m_id_to_virtual_faces.end())
    {
        auto feature = find_face_it->second;
        strProperty = "";
        strProperty += "VirtualFace\r\n";
        strProperty += "Id : " + feature->id + "\r\n";
        strProperty += "Area : " + std::to_string(feature->area) + "\r\n";
        strProperty += "Sense : " + std::to_string(feature->sense) + "\r\n";
        strProperty += "Loops  \r\n";
        for (int i = 0; i< feature->loops.size();i++)
        {
            auto loop = feature->loops[i];
            strProperty += "\t Loop["+ std::to_string(i) + "] : " + loop + " \r\n";
        }
        return strProperty;
    }

    auto find_loop_it = m_id_to_virtual_loops.find(strId);
    if (find_loop_it != m_id_to_virtual_loops.end())
    {
        auto feature = find_loop_it->second;
        strProperty = "";
        strProperty += "VirtualLoop\r\n";
        strProperty += "Id : " + feature->id + "\r\n";
        return strProperty;
    }

    auto find_coedge_it = m_id_to_virtual_coedges.find(strId);
    if (find_coedge_it != m_id_to_virtual_coedges.end())
    {
        auto feature = find_coedge_it->second;
        strProperty = "";
        strProperty += "VirtualCoedge\r\n";
        strProperty += "Id : " + feature->id + "\r\n";
        return strProperty;
    }

    auto find_edge_it = m_id_to_virtual_edges.find(strId);
    if (find_edge_it != m_id_to_virtual_edges.end())
    {
        auto feature = find_edge_it->second;
        strProperty = "";
        strProperty += "VirtualEdge\r\n";
        strProperty += "ParamRange : " + std::to_string(feature->paramRange.start.get_double()) + "," + std::to_string(feature->paramRange.end.get_double()) + "\r\n";
        strProperty += "Sense : " + std::to_string(feature->sense) + "\r\n";

        std::string curve_type = "Edge";
        // curve
        auto curve = feature->curve;

        if (avro_fr::union_util::is_same_type<fr::recognize_welding::Straight>(curve))
        {
            curve_type = "Straight";
            
            fr::recognize_welding::UnitVector3D direction;
            fr::recognize_welding::Point3D rootPoint;
            double paramScale = 0.0;

            auto straight = curve.get_Straight();
            direction = straight.direction;
            rootPoint = straight.rootPoint;
            paramScale = straight.paramScale;

            strProperty += "Type : " + curve_type + "\r\n";
            strProperty += "\t direction : " + std::to_string(direction.x) + "," + std::to_string(direction.y) + "," + std::to_string(direction.z) + "\r\n";
            strProperty += "\t rootPoint : " + std::to_string(rootPoint.x) + "," + std::to_string(rootPoint.y) + "," + std::to_string(rootPoint.z) + "\r\n";
            strProperty += "\t paramScale : " + std::to_string(paramScale) + "\r\n";
        }
        else if (avro_fr::union_util::is_same_type<fr::recognize_welding::Polyline>(curve))
        {
            curve_type = "Polyline";

            auto polyline = curve.get_Polyline();
            for (int i = 0; i < polyline.points.size(); ++i)
            {
                auto point = polyline.points[i];
                strProperty += "\t point[" + std::to_string(i) + "] : " + std::to_string(point.x) + "," + std::to_string(point.y) + "," + std::to_string(point.z) + "\r\n";
            }
        }
        else if (avro_fr::union_util::is_same_type<fr::recognize_welding::Ellipse>(curve))
        {
            fr::recognize_welding::Point3D center;
            fr::recognize_welding::UnitVector3D normal;
            fr::recognize_welding::Vector3D majorAxis;
            double radiusRatio = 0.0;
            fr::recognize_welding::Parameter paramOff;
            bool periodic = false;
            fr::recognize_welding::Interval paramRange;

            curve_type = "Ellipse";
            auto circle = curve.get_Ellipse();
            center = circle.center;
            normal = circle.normal;
            majorAxis = circle.majorAxis;
            paramOff = circle.paramOff;
            periodic = circle.periodic;
            paramRange = circle.paramRange;

            strProperty += "Type : " + curve_type + "\r\n";
            strProperty += "\t Center : " + std::to_string(center.x) + "," + std::to_string(center.y) + "," + std::to_string(center.z) + "\r\n";
            strProperty += "\t Normal : " + std::to_string(normal.x) + "," + std::to_string(normal.y) + "," + std::to_string(normal.z) + "\r\n";
            strProperty += "\t MajorAxis : " + std::to_string(majorAxis.x) + "," + std::to_string(majorAxis.y) + "," + std::to_string(majorAxis.z) + "\r\n";
            strProperty += "\t ParamRange : [" + std::to_string(paramRange.start.get_double()) + "," + std::to_string(paramRange.end.get_double()) + "]\r\n";

            strProperty += "\t RadiusRatio : " + std::to_string(radiusRatio) + "\r\n";
            strProperty += "\t ParamOff : " + std::to_string(paramOff.value) + "\r\n";
            strProperty += "\t Periodic : " + std::to_string(periodic) + "\r\n";
        }
        else if (avro_fr::union_util::is_same_type<fr::recognize_welding::Circle>(curve))
        {
            fr::recognize_welding::Point3D center;
            fr::recognize_welding::UnitVector3D normal;
            fr::recognize_welding::Vector3D majorAxis;
            double radiusRatio = 0.0;
            fr::recognize_welding::Parameter paramOff;
            bool periodic = false;
            fr::recognize_welding::Interval paramRange;

            curve_type = "Circle";
            auto circle = curve.get_Circle();
            center = circle.center;
            normal = circle.normal;
            majorAxis = circle.majorAxis;
            paramOff = circle.paramOff;
            periodic = circle.periodic;
            paramRange = circle.paramRange;

            strProperty += "Type : " + curve_type + "\r\n";
            strProperty += "\t Center : " + std::to_string(center.x) + "," + std::to_string(center.y) + "," + std::to_string(center.z) + "\r\n";
            strProperty += "\t Normal : " + std::to_string(normal.x) + "," + std::to_string(normal.y) + "," + std::to_string(normal.z) + "\r\n";
            strProperty += "\t MajorAxis : " + std::to_string(majorAxis.x) + "," + std::to_string(majorAxis.y) + "," + std::to_string(majorAxis.z) + "\r\n";
            strProperty += "\t ParamRange : [" + std::to_string(paramRange.start.get_double()) + "," + std::to_string(paramRange.end.get_double()) + "]\r\n";

            strProperty += "\t RadiusRatio : " + std::to_string(radiusRatio) + "\r\n";
            strProperty += "\t ParamOff : " + std::to_string(paramOff.value) + "\r\n";
            strProperty += "\t Periodic : " + std::to_string(periodic) + "\r\n";
        }

        return strProperty;
    }

    auto find_vertex_it = m_id_to_virtual_vertices.find(strId);
    if (find_vertex_it != m_id_to_virtual_vertices.end())
    {
        auto feature = find_vertex_it->second;
        strProperty = "";
        strProperty += "VirtualVertex\r\n";
        strProperty += "Id : " + feature->id + "\r\n";
        strProperty += "Pos : " + std::to_string(feature->position.x) + "," + std::to_string(feature->position.y) + "," + std::to_string(feature->position.z) + "\r\n";
        return strProperty;
    }
    return strProperty;
}

void CModelViewerView::DoTracking(CPoint point)
{
    if ((TM_ROTATION == m_eTrackingMode && !m_bRotation)
        || (TM_PAN == m_eTrackingMode && !m_bPan)
        || (TM_ZOOM == m_eTrackingMode && !m_bZoom))
    {       // 無効なマウス操作対応処理
        return;
    }

    // 親クラスの関数の呼び出し
    COpenGLPickView::DoTracking(point);
}

void CModelViewerView::RenderModel(ERENDERMODE eRenderMode)
{
    RenderModelSat();
    if (0 == m_model.m_vVertex.size())
    {
        return;
    }

    unsigned int uiIndexTriangle;
    unsigned int uiIndexLine;
    unsigned int uiIndexPoint;
    unsigned int ui3;

    glPushAttrib(GL_ALL_ATTRIB_BITS);    // アトリビュート復帰できるように、一つ追加

    // 面
    if (m_bRenderFace)
    {
        if (RM_RENDER == eRenderMode)
        {
            glColor3f(0.5f, 0.5f, 0.0f);
        }
        if (RM_PICK_ELEMENTTYPE == eRenderMode)
        {
            glColor3f(1.0f, 0.0f, 0.0f);
        }
        for (uiIndexTriangle = 0; uiIndexTriangle < m_model.m_vIndexedTriangle.size(); uiIndexTriangle++)
        {
            if (RM_PICK_ELEMENTID == eRenderMode)
            {
                glColor3ub(GetRValue(uiIndexTriangle), GetGValue(uiIndexTriangle), GetBValue(uiIndexTriangle));
            }
            glBegin(GL_TRIANGLES);
            glVertex3fv((float*)&m_model.m_vVertex[m_model.m_vIndexedTriangle[uiIndexTriangle].m_ui3IndexVertex[0]]);
            glVertex3fv((float*)&m_model.m_vVertex[m_model.m_vIndexedTriangle[uiIndexTriangle].m_ui3IndexVertex[1]]);
            glVertex3fv((float*)&m_model.m_vVertex[m_model.m_vIndexedTriangle[uiIndexTriangle].m_ui3IndexVertex[2]]);
            glEnd();
        }
        // ピック面の描画
        if (RM_RENDER == eRenderMode)
        {
            if (RET_FACE == m_auiName_picked[1])
            {
                uiIndexTriangle = m_auiName_picked[2];
                glColor3f(1.0f, 1.0f, 0.0f);    // ピック色
                glBegin(GL_TRIANGLES);
                glVertex3fv((float*)&m_model.m_vVertex[m_model.m_vIndexedTriangle[uiIndexTriangle].m_ui3IndexVertex[0]]);
                glVertex3fv((float*)&m_model.m_vVertex[m_model.m_vIndexedTriangle[uiIndexTriangle].m_ui3IndexVertex[1]]);
                glVertex3fv((float*)&m_model.m_vVertex[m_model.m_vIndexedTriangle[uiIndexTriangle].m_ui3IndexVertex[2]]);
                glEnd();
            }
        }
    }
    // 線
    if (m_bRenderLine)
    {
        glLineWidth(2.0);
        if (RM_RENDER == eRenderMode)
        {
            glColor3f(0.0f, 0.5f, 0.5f);
        }
        if (RM_PICK_ELEMENTTYPE == eRenderMode)
        {
            glColor3f(0.0f, 1.0f, 0.0f);
        }
        for (uiIndexTriangle = 0; uiIndexTriangle < m_model.m_vIndexedTriangle.size(); ++uiIndexTriangle)
        {
            for (ui3 = 0; ui3 < 3; ++ui3)
            {
                if (RM_PICK_ELEMENTID == eRenderMode)
                {
                    uiIndexLine = uiIndexTriangle * 3 + ui3;
                    glColor3ub(GetRValue(uiIndexLine), GetGValue(uiIndexLine), GetBValue(uiIndexLine));
                }
                glBegin(GL_LINES);
                glVertex3fv((float*)&m_model.m_vVertex[m_model.m_vIndexedTriangle[uiIndexTriangle].m_ui3IndexVertex[(ui3 + 0) % 3]]);
                glVertex3fv((float*)&m_model.m_vVertex[m_model.m_vIndexedTriangle[uiIndexTriangle].m_ui3IndexVertex[(ui3 + 1) % 3]]);
                glEnd();
            }
        }
        // ピック線の描画
        if (RM_RENDER == eRenderMode)
        {
            if (RET_LINE == m_auiName_picked[1])
            {
                uiIndexLine = m_auiName_picked[2];
                uiIndexTriangle = uiIndexLine / 3;
                ui3 = uiIndexLine % 3;
                glLineWidth(5.0f);
                glColor3f(0.0f, 1.0f, 1.0f);
                glBegin(GL_LINES);
                glVertex3fv((float*)&m_model.m_vVertex[m_model.m_vIndexedTriangle[uiIndexTriangle].m_ui3IndexVertex[(ui3 + 0) % 3]]);
                glVertex3fv((float*)&m_model.m_vVertex[m_model.m_vIndexedTriangle[uiIndexTriangle].m_ui3IndexVertex[(ui3 + 1) % 3]]);
                glEnd();
            }
        }
    }
    // 点
    if (m_bRenderPoint)
    {
        glPointSize(5.0);
        if (RM_RENDER == eRenderMode)
        {
            glColor3f(0.5f, 0.0f, 0.5f);
        }
        if (RM_PICK_ELEMENTTYPE == eRenderMode)
        {
            glColor3f(0.0f, 0.0f, 1.0f);
        }
        for (uiIndexPoint = 0; uiIndexPoint < m_model.m_vVertex.size(); uiIndexPoint++)
        {
            if (RM_PICK_ELEMENTID == eRenderMode)
            {
                glColor3ub((GLubyte)GetRValue(uiIndexPoint), (GLubyte)GetGValue(uiIndexPoint), (GLubyte)GetBValue(uiIndexPoint));
            }
            glBegin(GL_POINTS);
            glVertex3fv((float*)&m_model.m_vVertex[uiIndexPoint]);
            glEnd();
        }
        // ピック点の描画
        if (RM_RENDER == eRenderMode)
        {
            if (RET_POINT == m_auiName_picked[1])
            {
                glPointSize(10.0f);
                glColor3f(1.0f, 0.0f, 1.0f);
                uiIndexPoint = m_auiName_picked[2];
                glBegin(GL_POINTS);
                glVertex3fv((float*)&m_model.m_vVertex[uiIndexPoint]);
                glEnd();
            }
        }
    }

    glPopAttrib();    // アトリビュートの復帰
    return;
}

void CModelViewerView::IdentifyPickRegionPixels(unsigned int uiCountPickRegionPixel, unsigned int aauiNamePickRegionPixel[][NAMEARRAYSIZE])
{
    int  iId_selected = -1;
    unsigned int  uiElementType_selected = RET_FACE + 1;
    long lSquareDist_selected = (2 + PICKREGIONOFFSET) * (2 + PICKREGIONOFFSET) + (2 + PICKREGIONOFFSET) * (2 + PICKREGIONOFFSET);
    long lSquareDist_current;
    int  x;
    int  y;
    for (unsigned int ui = 0; ui < uiCountPickRegionPixel; ui++)
    {
        if (0 == aauiNamePickRegionPixel[ui][1])
        { // モデルの外側
            continue;
        }

        if (!m_bPickPoint && RET_POINT == aauiNamePickRegionPixel[ui][1])
        {    // 点ピックOFFの場合は、点はピックできない
            continue;
        }
        if (!m_bPickLine && RET_LINE == aauiNamePickRegionPixel[ui][1])
        {    // 線ピックOFFの場合は、線はピックできない
            continue;
        }
        if (!m_bPickFace && RET_FACE == aauiNamePickRegionPixel[ui][1])
        {    // 面ピックOFFの場合は、面はピックできない
            continue;
        }

        if (uiElementType_selected < aauiNamePickRegionPixel[ui][1])
        { // 要素タイプ的に、優先順位が低い
            continue;
        }
        if (uiElementType_selected > aauiNamePickRegionPixel[ui][1])
        { // 要素タイプ的に、優先順位が低い
            iId_selected = ui;
            uiElementType_selected = aauiNamePickRegionPixel[ui][1];
            x = ui % (1 + 2 * PICKREGIONOFFSET) - PICKREGIONOFFSET;
            y = ui / (1 + 2 * PICKREGIONOFFSET) - PICKREGIONOFFSET;
            lSquareDist_selected = x * x + y * y;
            continue;
        }
        // 要素タイプ的に、優先順位が同じ場合は、ピック領域の中心に近いものが優先度が高い。
        x = ui % (1 + 2 * PICKREGIONOFFSET) - PICKREGIONOFFSET;
        y = ui / (1 + 2 * PICKREGIONOFFSET) - PICKREGIONOFFSET;
        lSquareDist_current = x * x + y * y;
        if (lSquareDist_selected > lSquareDist_current)
        {
            iId_selected = ui;
            uiElementType_selected = aauiNamePickRegionPixel[ui][1];
            lSquareDist_selected = lSquareDist_current;
            continue;
        }
    }

    if (-1 == iId_selected)
    {
        return;
    }

    // 名前列メンバの更新
    memcpy(m_auiName_picked, aauiNamePickRegionPixel[iId_selected], NAMEARRAYSIZE * sizeof(unsigned int));
}

// CModelViewerView メッセージ ハンドラー
void CModelViewerView::OnViewRotation()
{
    // TODO: ここにコマンド ハンドラ コードを追加します。
    m_bRotation = !m_bRotation;
}

void CModelViewerView::OnViewPan()
{
    // TODO: ここにコマンド ハンドラ コードを追加します。
    m_bPan = !m_bPan;
}

void CModelViewerView::OnViewZoom()
{
    // TODO: ここにコマンド ハンドラ コードを追加します。
    m_bZoom = !m_bZoom;
}

void CModelViewerView::OnUpdateViewRotation(CCmdUI* pCmdUI)
{
    // TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
    pCmdUI->SetCheck(m_bRotation);
}

void CModelViewerView::OnUpdateViewPan(CCmdUI* pCmdUI)
{
    // TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
    pCmdUI->SetCheck(m_bPan);
}

void CModelViewerView::OnUpdateViewZoom(CCmdUI* pCmdUI)
{
    // TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
    pCmdUI->SetCheck(m_bZoom);
}

void CModelViewerView::OnViewRenderpoint()
{
    // TODO: ここにコマンド ハンドラ コードを追加します。
    m_bRenderPoint = !m_bRenderPoint;
    Invalidate(FALSE);
}

void CModelViewerView::OnViewRenderline()
{
    // TODO: ここにコマンド ハンドラ コードを追加します。
    m_bRenderLine = !m_bRenderLine;
    Invalidate(FALSE);
}

void CModelViewerView::OnViewRenderface()
{
    // TODO: ここにコマンド ハンドラ コードを追加します。
    m_bRenderFace = !m_bRenderFace;
    Invalidate(FALSE);
}

void CModelViewerView::OnUpdateViewRenderpoint(CCmdUI* pCmdUI)
{
    // TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
    pCmdUI->SetCheck(m_bRenderPoint);
}

void CModelViewerView::OnUpdateViewRenderline(CCmdUI* pCmdUI)
{
    // TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
    pCmdUI->SetCheck(m_bRenderLine);
}

void CModelViewerView::OnUpdateViewRenderface(CCmdUI* pCmdUI)
{
    // TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
    pCmdUI->SetCheck(m_bRenderFace);
}

void CModelViewerView::OnViewPickpoint()
{
    // TODO: ここにコマンド ハンドラ コードを追加します。
    m_bPickPoint = !m_bPickPoint;
    Invalidate(FALSE);
}

void CModelViewerView::OnViewPickline()
{
    // TODO: ここにコマンド ハンドラ コードを追加します。
    m_bPickLine = !m_bPickLine;
    Invalidate(FALSE);
}

void CModelViewerView::OnViewPickface()
{
    // TODO: ここにコマンド ハンドラ コードを追加します。
    m_bPickFace = !m_bPickFace;
    Invalidate(FALSE);
}

void CModelViewerView::OnUpdateViewPickpoint(CCmdUI* pCmdUI)
{
    // TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
    pCmdUI->SetCheck(m_bPickPoint);
}

void CModelViewerView::OnUpdateViewPickline(CCmdUI* pCmdUI)
{
    // TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
    pCmdUI->SetCheck(m_bPickLine);
}

void CModelViewerView::OnUpdateViewPickface(CCmdUI* pCmdUI)
{
    // TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
    pCmdUI->SetCheck(m_bPickFace);
}

void CModelViewerView::OnFileOpen()
{
    // TODO: ここにコマンド ハンドラー コードを追加します。
    CFileDialog        dlg(TRUE);
    dlg.m_ofn.lpstrFilter = _T("STL ファイル (*.stl)\0*.stl\0")
        _T("All Files (*.*)\0*.*\0\0");
    dlg.m_ofn.nFilterIndex = 1;
    dlg.m_ofn.Flags |= OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

    INT_PTR iResponse = dlg.DoModal();
    if (iResponse == IDOK)
    {
        char* pszFileName;
#ifdef UNICODE
        size_t length = wcslen(dlg.GetPathName().GetBuffer()) + 1;
        pszFileName = (LPSTR)malloc(length * sizeof(WCHAR));
        WideCharToMultiByte(CP_ACP,    // CODE PAGE: ANSI code page
            0,
            dlg.GetPathName().GetBuffer(),
            -1,
            pszFileName,
            (int)length * sizeof(WCHAR),
            NULL,
            NULL);
#else
        pszFileName = _strdup(dlg.GetPathName().GetBuffer());
#endif
        CModel model;
        if (LoadStlFile(pszFileName, model))
        {
            memset(m_auiName_picked, 0x00, NAMEARRAYSIZE * sizeof(unsigned int));
            m_model = model;
        }
        else
        {
            AfxMessageBox(_T("ファイル読み込みに失敗しました。"));
        }
        free(pszFileName);
        Invalidate();
    }
}

void CModelViewerView::OnFileNew()
{
    ENTITY_LIST new_sat_file; // 読み込むSATファイルENTITY_LIST
    m_entity_list.clear();
    m_entity_list.add(new_sat_file);
    if (m_display_data)
    {
        delete m_display_data;
        m_display_data = nullptr;
    }

    m_display_data = new AcisMesh::DisplayData;
    boost::filesystem::path path("ENTITY_LIST");
    CreateEnitiyTreeItems(new_sat_file, path.filename().string(), false);
    SetEntityMap(m_entity_list);
    Invalidate();
}

void CModelViewerView::OnFileOpenSat()
{
    CModelViewerView::ReadSatFile(false);
    Invalidate();
}

void CModelViewerView::OnFileOpenAddSat()
{
    CModelViewerView::ReadSatFile(true);
    Invalidate();
}

void CModelViewerView::OnFileOpenAvro()
{
    CDialogAvro dlg;
    if (dlg.DoModal() == IDOK)
    {
        std::string str_welding_result = dlg.m_avro;
        std::string schema_file_path = dlg.m_schema;
        bool is_export_edge = dlg.m_isExportEdge == TRUE ? true : false;
        int importMode = dlg.m_importMode;

        switch (importMode)
        {
        case 0: // 溶接認識AVRO
        {
            ReadWeldingResultRaw(str_welding_result, schema_file_path, is_export_edge);
        }
        default:
            break;
        }
    }

    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    if (pFrame)
    {
        pFrame->m_wndEntityView.Invalidate();
    }

    Invalidate();
}

void CModelViewerView::OnFileOpenJson()
{
    CFileDialog file_open_dialog(TRUE);
    file_open_dialog.m_ofn.lpstrFilter = _T("JSON ファイル (*.json)\0*.json\0")
        _T("All Files (*.*)\0*.*\0\0");
    file_open_dialog.m_ofn.nFilterIndex = 1;
    file_open_dialog.m_ofn.Flags |= OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    INT_PTR iResponse = file_open_dialog.DoModal();
    if (iResponse == IDOK)
    {
        char* file_name;
        size_t length = wcslen(file_open_dialog.GetPathName().GetBuffer()) + 1;
        file_name = (LPSTR)malloc(length * sizeof(WCHAR));
        WideCharToMultiByte(CP_ACP,    // CODE PAGE: ANSI code page
            0,
            file_open_dialog.GetPathName().GetBuffer(),
            -1,
            file_name,
            (int)length * sizeof(WCHAR),
            NULL,
            NULL);


        std::ifstream ifstream_wc(file_name);
        if (!ifstream_wc)
        {
            return;
        }
        // 関数jsonファイルを読み込み、streamを返す
        auto get_json_contents = [](const std::ifstream& ifstream)
        {
            std::stringstream sstream;
            sstream << ifstream.rdbuf();
            return sstream.str();
        };
        std::string err;
        auto json_contents = json11::Json::parse(get_json_contents(ifstream_wc), err);
        if (!err.empty())
        {
            return;
        }

        char drive[MAX_PATH + 1]
            , dir[MAX_PATH + 1]
            , fname[MAX_PATH + 1]
            , ext[MAX_PATH + 1];

        _splitpath_s(file_name, drive, dir, fname, ext);//パス名を構成要素に分解します

        CreateViewJsonTreeItems(fname, json_contents, true);

        m_json_contents_list.emplace_back(json_contents);
    }

    Invalidate();
}

void CModelViewerView::OnFileSaveAs()
{
    char strFilter[] = { "SAT Files (*.sat)|*.sat|" };
    CFileDialog fileDlg(FALSE, CString(".sat"), NULL, 0, CString(strFilter));
    if (fileDlg.DoModal() == IDOK)
    {
        char* pszFileName;
#ifdef UNICODE
        size_t length = wcslen(fileDlg.GetPathName().GetBuffer()) + 1;
        pszFileName = (LPSTR)malloc(length * sizeof(WCHAR));
        WideCharToMultiByte(CP_ACP,    // CODE PAGE: ANSI code page
            0,
            fileDlg.GetPathName().GetBuffer(),
            -1,
            pszFileName,
            (int)length * sizeof(WCHAR),
            NULL,
            NULL);
#else
        pszFileName = _strdup(dlg.GetPathName().GetBuffer());
#endif
        if(m_entity_list.count())
        {
            if (SaveSAT(pszFileName, &m_entity_list))
            {
                const unsigned int data_size = 255;
                wchar_t w_str[data_size + 1] = { 0x00 };
                char str[data_size] = "\0";
                sprintf_s(str, "%s", pszFileName);
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
                AfxMessageBox(w_str);
            }
        }else
        {
            
        }

    }
    Invalidate();
}

void CModelViewerView::OnSetLookAtCenter()
{
    CDialogLookAt dlg;
    if (dlg.DoModal() == IDOK)
    {

    }
}

void CModelViewerView::OnShapePoint()
{
    CDialogShapePoint dlg;
    if (dlg.DoModal() == IDOK)
    {
        const SPAposition point(dlg.m_pointX, dlg.m_pointY, dlg.m_pointZ);
        //VERTEX* ver = new VERTEX(new APOINT(0,0,0));
        //ENTITY_LIST create_shape_list; // 読み込むSATファイルENTITY_LIST

        //m_entity_list.add(ver);

        //if (m_display_data)
        //{
        //    delete m_display_data;
        //    m_display_data = nullptr;
        //}

        //m_display_data = new AcisMesh::DisplayData;
        //if (CreateMeshFromEntityList(m_entity_list, *m_display_data, m_selected_list, m_pmi_list))
        //{

        //    std::string name("VERTEX");
        //    CreateEnitiyTreeItems(create_shape_list, name, true);
        //}

        //SetEntityMap(m_entity_list);
    }
    Invalidate();
}

// api_mk_ed_line
void CModelViewerView::OnShapeEdgeStraight()
{
    CDialogShapeEdgeStraight dlg;
    if (dlg.DoModal() == IDOK)
    {
        EDGE* edge_line = nullptr;
        const SPAposition start_point(dlg.m_startX, dlg.m_startY, dlg.m_startZ);
        const SPAposition end_point(dlg.m_endX, dlg.m_endY, dlg.m_endZ);
        // api_mk_ed_line
        api_mk_ed_line(
            start_point,
            end_point,
            edge_line
        );

        if(edge_line)
        {
            ENTITY_LIST create_shape_list; // 読み込むSATファイルENTITY_LIST

            m_entity_list.add(edge_line);
            create_shape_list.add(edge_line);

            if (m_display_data)
            {
                delete m_display_data;
                m_display_data = nullptr;
            }

            m_display_data = new AcisMesh::DisplayData;
            if (CreateMeshFromEntityList(m_entity_list, *m_display_data, m_selected_list, m_pmi_list))
            {
                std::string name("EDGE");
                CreateEnitiyTreeItems(create_shape_list, name, true);
            }

            SetEntityMap(m_entity_list);
        }
    }

    Invalidate();
}

// api_mk_ed_ellipse
void CModelViewerView::OnShapeEdgeEllipse()
{
    EDGE* edge_ellipse = nullptr;
    const SPAposition center(0,0,0);
    const SPAunit_vector normal(0,0,1);

    const SPAvector major_axis(10,10,0);
    const double radius_ratio = 1.0;
    const double start_angle = 0.0;
    const double end_angle = 1.0;

    api_mk_ed_ellipse(
        center,
        normal,
        major_axis,
        radius_ratio,
        start_angle,
        end_angle,
        edge_ellipse
    );

    ENTITY_LIST create_shape_list; // 読み込むSATファイルENTITY_LIST

    m_entity_list.add(edge_ellipse);
    create_shape_list.add(edge_ellipse);

    if (m_display_data)
    {
        delete m_display_data;
        m_display_data = nullptr;
    }

    m_display_data = new AcisMesh::DisplayData;
    if (CreateMeshFromEntityList(m_entity_list, *m_display_data, m_selected_list, m_pmi_list))
    {
        std::string name("EDGE");
        CreateEnitiyTreeItems(create_shape_list, name, true);
    }

    SetEntityMap(m_entity_list);
    Invalidate();
}

// api_make_cnface
void CModelViewerView::OnShapeFaceCone()
{
    CDialogShapeFaceCone dlg;
    if (dlg.DoModal() == IDOK)
    {
        SPAposition    center(0, 0, 0);
        SPAunit_vector normal_axis(0, 0, 1);
        SPAvector      major_axis(1, 0, 0);
        double         ratio = 1;
        double         sin_angle = 0;
        double         cos_angle = 1;
        double         start_angle = 0;
        double         end_angle = 0.5 * M_PI;
        double         height = 5;
        FACE* result_face = nullptr;
        result_face = create_shape_face_cone(
            center,
            normal_axis,
            major_axis,
            ratio,
            sin_angle,
            cos_angle,
            start_angle,
            end_angle,
            height
        );

        ENTITY_LIST create_shape_list; // 読み込むSATファイルENTITY_LIST

        m_entity_list.add(result_face);
        create_shape_list.add(result_face);

        if (m_display_data)
        {
            delete m_display_data;
            m_display_data = nullptr;
        }

        m_display_data = new AcisMesh::DisplayData;
        if (CreateMeshFromEntityList(m_entity_list, *m_display_data, m_selected_list, m_pmi_list))
        {
            std::string name("FACE(CONE)");
            CreateEnitiyTreeItems(create_shape_list, name, true);
        }

        SetEntityMap(m_entity_list);
    }
}

// api_make_cnface
void CModelViewerView::OnShapeFacePlane()
{
    CDialogShapeFacePlane dlg;
    if (dlg.DoModal() == IDOK)
    {
        double corner_x = std::stod(CStringA(dlg.m_originX.GetBuffer()).GetBuffer());
        double corner_y = std::stod(CStringA(dlg.m_originY.GetBuffer()).GetBuffer());
        double corner_z = std::stod(CStringA(dlg.m_originZ.GetBuffer()).GetBuffer());

        double normal_x = std::stod(CStringA(dlg.m_normalX.GetBuffer()).GetBuffer());
        double normal_y = std::stod(CStringA(dlg.m_normalY.GetBuffer()).GetBuffer());
        double normal_z = std::stod(CStringA(dlg.m_normalZ.GetBuffer()).GetBuffer());

        double width = std::stod(CStringA(dlg.m_width.GetBuffer()).GetBuffer());
        double height = std::stod(CStringA(dlg.m_height.GetBuffer()).GetBuffer());
        outcome result;
        // Make a planar face.
        const SPAposition corner(corner_x, corner_y, corner_z);
        const SPAunit_vector normal(normal_x, normal_y, normal_z);
        FACE* pl_face = nullptr;

        result = api_face_plane(corner, width, height, &normal, pl_face);
        if(!result.ok())
        {
            std::string err(find_err_mess(result.error_number()));
            return;
        }

        ENTITY_LIST create_shape_list; // 読み込むSATファイルENTITY_LIST

        m_entity_list.add(pl_face);
        create_shape_list.add(pl_face);

        if (m_display_data)
        {
            delete m_display_data;
            m_display_data = nullptr;
        }

        m_display_data = new AcisMesh::DisplayData;
        if (CreateMeshFromEntityList(m_entity_list, *m_display_data, m_selected_list, m_pmi_list))
        {
            std::string name("FACE(PLANE)");
            CreateEnitiyTreeItems(create_shape_list, name, true);
        }

        SetEntityMap(m_entity_list);

        std::string str_result = "corner : " + std::to_string(corner_x) + "," + std::to_string(corner_y) + "," + std::to_string(corner_z) + "\r\n"
            + "normal : " + std::to_string(normal_x) + "," + std::to_string(normal_y) + "," + std::to_string(normal_z) + "\r\n"
            + "width : " + std::to_string(width) + "\r\n"
            + "height : " + std::to_string(height) + "\r\n";

        const unsigned int data_size = 255;
        wchar_t w_str[data_size + 1] = { 0x00 };
        char str[data_size] = "\0";
        sprintf_s(str, "%s", str_result.c_str());
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        AfxMessageBox(w_str);
    }
    Invalidate();
}

void CModelViewerView::OnShapeFaceSphere()
{

}

void CModelViewerView::OnShapeFaceSpline()
{

}

void CModelViewerView::OnShapeFaceTorus()
{

}

void CModelViewerView::OnShapeSolidBlock()
{
    const SPAposition pt1(0,0,0);
    const SPAposition pt2(100,100,100);
    BODY* block = NULL;
    outcome result = api_solid_block(
        pt1,
        pt2,
        block
    );
    ENTITY_LIST create_shape_list; // 読み込むSATファイルENTITY_LIST

    m_entity_list.add(block);
    create_shape_list.add(block);

    if (m_display_data)
    {
        delete m_display_data;
        m_display_data = nullptr;
    }

    m_display_data = new AcisMesh::DisplayData;
    if (CreateMeshFromEntityList(m_entity_list, *m_display_data, m_selected_list, m_pmi_list))
    {
        std::string name("Entity");
        CreateEnitiyTreeItems(create_shape_list, name, true);
    }

    SetEntityMap(m_entity_list);
    Invalidate();
}

void CModelViewerView::OnShapeSolidCylinder()
{

}

void CModelViewerView::OnShapeSolidSphere()
{
    const SPAposition center(0, 0, 0);
    const double radius = 10;
    BODY* sph = nullptr;
    outcome result = api_solid_sphere(
        center,
        radius,
        sph
    );
    ENTITY_LIST create_shape_list; // 読み込むSATファイルENTITY_LIST

    m_entity_list.add(sph);
    create_shape_list.add(sph);

    if (m_display_data)
    {
        delete m_display_data;
        m_display_data = nullptr;
    }

    m_display_data = new AcisMesh::DisplayData;
    if (CreateMeshFromEntityList(m_entity_list, *m_display_data, m_selected_list, m_pmi_list))
    {
        std::string name("Entity");
        CreateEnitiyTreeItems(create_shape_list, name, true);
    }

    SetEntityMap(m_entity_list);
    Invalidate();
}

void CModelViewerView::OnShapeSolidTorus()
{
    const SPAposition center(0,0,0);
    const double major_radius = 10;
    const double minor_radius = 5;
    BODY* tor = nullptr;
    outcome result = api_solid_torus(
        center,
        major_radius,
        minor_radius,
        tor
    );

    ENTITY_LIST create_shape_list; // 読み込むSATファイルENTITY_LIST

    m_entity_list.add(tor);
    create_shape_list.add(tor);

    if (m_display_data)
    {
        delete m_display_data;
        m_display_data = nullptr;
    }

    m_display_data = new AcisMesh::DisplayData;
    if (CreateMeshFromEntityList(m_entity_list, *m_display_data, m_selected_list, m_pmi_list))
    {
        std::string name("Entity");
        CreateEnitiyTreeItems(create_shape_list, name, true);
    }

    SetEntityMap(m_entity_list);
    Invalidate();
}

// API_SUBTRACT
void CModelViewerView::OnApiSubtract()
{
    CMainFrame* p_frame = (CMainFrame*)AfxGetMainWnd();
    if (!p_frame)
    {

        return;
    }
    p_frame->m_dlgApiSubtract.ShowWindow(SW_SHOW);

    CDialogApiSubtract dlg;
    if (dlg.DoModal() == IDOK)
    {
        std::string str_result;
        const unsigned int data_size = 255;
        wchar_t w_str[data_size + 1] = { 0x00 };
        char str[data_size] = "\0";


        // m_entity_listの１番目ターゲット
        // m_entity_listの２番目分割Face
        if (m_entity_list.count() < 2)
        {
            str_result = "m_entity_listの１番目ターゲット\r\nm_entity_listの２番目分割Face";
            sprintf_s(str, "%s", str_result.c_str());
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            AfxMessageBox(w_str);

            return;
        }

        if (!is_BODY(m_entity_list[0]))
        {
            return;
        }

        BODY* sph_body = dynamic_cast<BODY*>(m_entity_list[0]);
        if (sph_body == nullptr)
        {
            return;
        }

        if (!is_FACE(m_entity_list[1]))
        {
            return;
        }
        FACE* pl_face = dynamic_cast<FACE*>(m_entity_list[1]);

        std::string subtract_target = CStringA(dlg.m_targetEdit).GetBuffer();
        std::string subtract_face = CStringA(dlg.m_faceEdit).GetBuffer();
        if (!subtract_face.empty())
        {
            for (int i = 0; i < m_entity_list.count(); ++i)
            {
                ENTITY_LIST faces;
                api_get_faces(m_entity_list[i], faces);
                for (int j = 0; j < faces.count(); ++j)
                {
                    FACE* face = dynamic_cast<FACE*>(faces[j]);
                    std::string uid = find_attribute(face, "uid");
                    if (uid == subtract_face)
                    {
                        pl_face = face;
                        break;
                    }
                }

            }
        }

        ENTITY* sph_body_copy = nullptr;
        api_copy_entity(sph_body, sph_body_copy);
        ENTITY* pl_face_entity = nullptr;
        api_copy_entity(pl_face, pl_face_entity);

        FACE* pl_face1 = dynamic_cast<FACE*>(pl_face_entity);

        outcome result;

        // Make a body with the planar face.
        BODY* tool = nullptr;
        result = api_mk_by_faces(nullptr, 1, &pl_face, tool);
        if (!result.ok())
        {
            std::string err(find_err_mess(result.error_number()));
            return;
        }

        // Subtract the planar body (open shell) from the sphere.
        result = api_subtract(tool, sph_body);
        if (!result.ok())
        {
            std::string err(find_err_mess(result.error_number()));
            return;
        }

        api_reverse_face(pl_face1);
        result = api_mk_by_faces(nullptr, 1, &pl_face1, tool);
        if (!result.ok())
        {
            std::string err(find_err_mess(result.error_number()));
            return;
        }
        result = api_subtract(tool, dynamic_cast<BODY*>(sph_body_copy));
        if (!result.ok())
        {
            std::string err(find_err_mess(result.error_number()));
            return;
        }
        //m_entity_list.remove(0);

        ENTITY_LIST read_sat_file; // 読み込むSATファイルENTITY_LIST
        read_sat_file.add(sph_body);
        read_sat_file.add(sph_body_copy);
        m_entity_list.clear();

        m_entity_list.add(read_sat_file);

        if (m_display_data)
        {
            delete m_display_data;
            m_display_data = nullptr;
        }

        m_display_data = new AcisMesh::DisplayData;
        if (CreateMeshFromEntityList(m_entity_list, *m_display_data, m_selected_list, m_pmi_list))
        {
            boost::filesystem::path path("BODY(subtract)");
            CreateEnitiyTreeItems(read_sat_file, path.filename().string(), false);
        }

        SetEntityMap(m_entity_list);
    }
}


void CModelViewerView::OnApiUnite()
{
    CMainFrame* p_frame = (CMainFrame*)AfxGetMainWnd();
    if (!p_frame)
    {

        return;
    }
    {
        std::string str_result;
        const unsigned int data_size = 255;
        wchar_t w_str[data_size + 1] = { 0x00 };
        char str[data_size] = "\0";
        // m_entity_listの１番目ターゲット
        // m_entity_listの２番目分割Face
        if (m_entity_list.count() < 2 )
        {
            str_result = "m_entity_listの１番目ターゲット\r\nm_entity_listの２番目分割Face";
            sprintf_s(str, "%s", str_result.c_str());
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
            AfxMessageBox(w_str);

            return;
        }

        BODY* blank(dynamic_cast<BODY*>(m_entity_list[0]));
        for (int i = 1; i < m_entity_list.count(); ++i)
        {
            BODY* body(dynamic_cast<BODY*>(m_entity_list[i]));
            auto result = api_boolean(body, blank, BOOL_TYPE::UNION);
        }

        ENTITY_LIST read_sat_file; // 読み込むSATファイルENTITY_LIST
        read_sat_file.add(blank);
        m_entity_list.clear();

        m_entity_list.add(read_sat_file);

        if (m_display_data)
        {
            delete m_display_data;
            m_display_data = nullptr;
        }

        m_display_data = new AcisMesh::DisplayData;
        if (CreateMeshFromEntityList(m_entity_list, *m_display_data, m_selected_list, m_pmi_list))
        {
            boost::filesystem::path path("BODY(union)");
            CreateEnitiyTreeItems(read_sat_file, path.filename().string(), false);
        }

        SetEntityMap(m_entity_list);
    }
}

// CModelViewerView メッセージ ハンドラー
void CModelViewerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    COpenGLPickView::OnKeyDown(nChar, nRepCnt, nFlags);
    switch (nChar)
    {
    case 0x31:
    {
        POINT p;
        if (GetCursorPos(&p))
        {
            this->ScreenToClient(&p);
            m_eTrackingMode = TM_ROTATION;
            DoTracking(p);
        }
        break;
    }

        break;
    case 0x32:
    {
        POINT p;
        if (GetCursorPos(&p))
        {
            this->ScreenToClient(&p);
            m_eTrackingMode = TM_PAN;
            DoTracking(p);
        }
        break;
    }

        break;
    case 0x33:
    {
        POINT p;
        if (GetCursorPos(&p))
        {
            this->ScreenToClient(&p);
            m_eTrackingMode = TM_ZOOM;
            DoTracking(p);
        }
        break;
    }
    default:
        break;
    }
}


void CModelViewerView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    POINT p;
    if (GetCursorPos(&p))
    {
        this->ScreenToClient(&p);
        m_eTrackingMode = TM_NONE;
        DoTracking(p);
    }
    COpenGLPickView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CModelViewerView::OnApiSubtractBnClickedOk()
{
    std::string str_result;
    const unsigned int data_size = 255;
    wchar_t w_str[data_size + 1] = { 0x00 };
    char str[data_size] = "\0";
    // m_entity_listの１番目ターゲット
    // m_entity_listの２番目分割Face
    if (m_entity_list.count() < 2)
    {
        str_result = "m_entity_listの１番目ターゲット\r\nm_entity_listの２番目分割Face";
        sprintf_s(str, "%s", str_result.c_str());
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, data_size, w_str, data_size + 1);
        AfxMessageBox(w_str);

        return;
    }

    if (!is_BODY(m_entity_list[0]))
    {
        return;
    }

    BODY* sph_body = dynamic_cast<BODY*>(m_entity_list[0]);
    if (sph_body == nullptr)
    {
        return;
    }

    if (!is_FACE(m_entity_list[1]))
    {
        return;
    }
    FACE* pl_face = dynamic_cast<FACE*>(m_entity_list[1]);

    ENTITY* sph_body_copy = nullptr;
    api_copy_entity(sph_body, sph_body_copy);
    ENTITY* pl_face_entity = nullptr;
    api_copy_entity(pl_face, pl_face_entity);

    FACE* pl_face1 = dynamic_cast<FACE*>(pl_face_entity);

    outcome result;

    // Make a body with the planar face.
    BODY* tool = nullptr;
    result = api_mk_by_faces(nullptr, 1, &pl_face, tool);
    if (!result.ok())
    {
        std::string err(find_err_mess(result.error_number()));
        return;
    }

    // Subtract the planar body (open shell) from the sphere.
    result = api_subtract(tool, sph_body);
    if (!result.ok())
    {
        std::string err(find_err_mess(result.error_number()));
        return;
    }

    api_reverse_face(pl_face1);
    result = api_mk_by_faces(nullptr, 1, &pl_face1, tool);
    if (!result.ok())
    {
        std::string err(find_err_mess(result.error_number()));
        return;
    }
    result = api_subtract(tool, dynamic_cast<BODY*>(sph_body_copy));
    if (!result.ok())
    {
        std::string err(find_err_mess(result.error_number()));
        return;
    }
    //m_entity_list.remove(0);

    ENTITY_LIST read_sat_file; // 読み込むSATファイルENTITY_LIST
    read_sat_file.add(sph_body);
    read_sat_file.add(sph_body_copy);
    m_entity_list.clear();

    m_entity_list.add(read_sat_file);

    if (m_display_data)
    {
        delete m_display_data;
        m_display_data = nullptr;
    }

    m_display_data = new AcisMesh::DisplayData;
    if (CreateMeshFromEntityList(m_entity_list, *m_display_data, m_selected_list, m_pmi_list))
    {
        boost::filesystem::path path("subtract(body)");
        CreateEnitiyTreeItems(read_sat_file, path.filename().string(), false);
    }

    SetEntityMap(m_entity_list);
}



