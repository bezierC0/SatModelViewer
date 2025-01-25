#pragma once
#include <vector>
#include <string>
#include "rgbcolor.hxx"
#include "boost\optional\optional.hpp"
#include "OGLJText.h"

// START FR
#include "recognized_welding_result.h"
//#include "avro_fr_utility.hpp"
// END FR
class ENTITY_LIST;
class ENTITY;
class FACE;
class EDGE;
class SPAposition;
class SPAunit_vector;
class SPAvector;

const double HIGH_LIGHT_COLOR[3] = { 1.0, 1.0, 0.0 }; // 黄色

class AcisMesh
{
public:
    struct FaceMesh
    {
        unsigned int numIndices;
        unsigned int baseVertex;
        unsigned int baseIndex;
        rgb_color color;
        FaceMesh()
        {
            numIndices = 0;
            baseVertex = 0;
            baseIndex = 0;
            color = 0;
        }
    };
    struct EdgeMesh
    {
        unsigned int numIndices;
        unsigned int baseVertex;
        rgb_color color;
        EdgeMesh()
        {
            numIndices = 0;
            baseVertex = 0;
            color = 0;
        }
    };

    struct EdgeVertex
    {
        float x = 0.0;
        float y = 0.0;
        float z = 0.0;
        rgb_color color;
    };

    struct DisplayData
    {
        std::vector<float> faceCoords;
        std::vector<int> triangles;
        std::vector<float> normalCoords;
        std::vector<float> edgeCoords;
        std::vector<AcisMesh::FaceMesh> faceMesh;
        std::vector<AcisMesh::EdgeMesh> edgeMesh;
        std::vector<AcisMesh::EdgeVertex> vertices;
    };
    // Takes ownership of the DisplayData
    AcisMesh(DisplayData* dd);
    ~AcisMesh();

    bool Display() const;


private:
    int m_level;
    DisplayData* m_data;
};

int initialize_acis();

void terminate_acis(int level);

bool CreateMeshFromEntityList(ENTITY_LIST& el, AcisMesh::DisplayData& dd, ENTITY_LIST& selectedList, std::vector<COGLJText*>& pmiList);

// SATファイル保存
int SaveSAT(const char* file_name, const ENTITY_LIST* opEntities);

// SATファイル読み込み
int RestoreSAT(const char* file_name, ENTITY_LIST& elist);

// 属性取得
std::string find_attribute(ENTITY* ent, const char* name);

// entityタイプ取得
std::string get_entity_type(ENTITY* ent);

// faceタイプ取得
std::string get_face_type(ENTITY* ent);

// edgeタイプ取得
std::string get_edge_type(ENTITY* ent);

// entityの情報取得
std::string get_entity_info(ENTITY* ent);

// api_make_cnface使ってface cone 生成
FACE* create_shape_face_cone(
    const SPAposition& center,              // center of the base of the cone.
    const SPAunit_vector& normal_axis,      // normal of the cone.
    const SPAvector& major_axis,            // major axis of the cone.
    double radius_ratio,                    // radius ratio of the base.
    double sint,                            // sine of the half angle of the cone
    double cost,                            // cosine of the half angle
    double st_ang,                          // start angle of the base ellipse
    double end_ang,                         // end angle of the base ellipse
    double height                           // height
);

// avro変換
boost::optional<std::vector<EDGE*>> convert_welding_edge_to_acis_edge(const fr::recognize_welding::Edge* edge);