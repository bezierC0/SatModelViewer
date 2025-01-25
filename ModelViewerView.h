#pragma once
#include "OpenGLPickView.h"
#include "ModelViewerDoc.h"

#include "Model.h"
#include "AcisLib.h"
#include "lists.hxx"
#include "utility\json\json11.hpp"

class CModelViewerView : public COpenGLPickView
{
    // メンバ変数
public:
    BOOL m_bRotation;
    BOOL m_bPan;
    BOOL m_bZoom;
    BOOL m_bRenderPoint;
    BOOL m_bRenderLine;
    BOOL m_bRenderFace;
    BOOL m_bPickPoint;
    BOOL m_bPickLine;
    BOOL m_bPickFace;

    CModel m_model;

    // 描画表示（START）
    float m_fLineWidth      = 1.0;    // Lineサイズ
    float m_fPointSize      = 1.0;    // 頂点サイズ
    float m_fFaceAlpha      = 1.0;    // Face透明度
    float m_fLineAlpha      = 1.0;    // Line透明度
    float m_fPointAlpha     = 1.0;    // 頂点透明度
    // 描画表示（END）

    ENTITY_LIST                     m_entity_list;                                  // satデータ
    ENTITY_LIST                     m_selected_list;                                // ListViewで選択されているItem
    std::vector<std::string>        m_picked_string_list;                           // pickリスト
    std::vector<COGLJText*>         m_pmi_list;                                     // 寸法表示
    AcisMesh::DisplayData*          m_display_data = nullptr;                       // SATから変換したPolygonデータ（表示）

    std::map<std::string, ENTITY*>  m_uid_to_face_map;                              // uidとACIS FACEキーのマップ
    std::map<std::string, ENTITY*>  m_uid_to_edge_map;                              // uidとACIS EDGEのマップ
    std::map<std::string, ENTITY*>  m_uid_to_vertex_map;                            // uidとACIS VERTEXのマップ

    fr::recognize_welding::RecognizedWeldingResultRaw   m_welding_result_raw;       // 溶接avro
    ENTITY_LIST                                         m_virtualTopology_list;     // TopologyList
    std::vector<json11::Json>                           m_json_contents_list;       // JSONリスト

    std::map<std::string, fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__0__>   m_id_to_features;   // idとLineWeldingPartのマップ
    std::map<std::string, fr::recognize_welding::SheetAssembly>         m_id_to_sheet_assemblies;                   // idとSheetAssemblyのマップ
    std::map<std::string, fr::recognize_welding::SideWelding>           m_id_to_side_weldings;                      // idとSideWeldingのマップ
    std::map<std::string, fr::recognize_welding::SurfaceWelding>        m_id_to_surface_weldings;                   // idとSurfaceWeldingのマップ
    std::map<std::string, fr::recognize_welding::LineWeldingPart>       m_id_to_line_welding_parts;                 // idとLineWeldingPartのマップ
    std::map<std::string, fr::recognize_welding::NonWeldingPart>        m_id_to_non_welding_parts;                  // idとNonWeldingPartのマップ
    std::map<std::string, fr::recognize_welding::SurfaceWeldingPart>    m_id_to_suface_welding_parts;               // idとSurfaceWeldingPartのマップ

    std::map<std::string, const fr::recognize_welding::Face*>           m_id_to_virtual_faces;                      // idとrecognize_welding::Coedgeのマップ
    std::map<std::string, const fr::recognize_welding::Loop*>           m_id_to_virtual_loops;                      // idとrecognize_welding::Loopのマップ
    std::map<std::string, const fr::recognize_welding::Coedge*>         m_id_to_virtual_coedges;                    // idとrecognize_welding::Coedgeのマップ
    std::map<std::string, const fr::recognize_welding::Edge*>           m_id_to_virtual_edges;                      // idとrecognize_welding::Edgeのマップ
    std::map<std::string, const fr::recognize_welding::Vertex*>         m_id_to_virtual_vertices;                   // idとrecognize_welding::Vertexのマップ

    // メンバ関数
private:
    void    DoTracking(CPoint point);
    void    RenderModel(ERENDERMODE eRenderMode);

    void    IdentifyPickRegionPixels(unsigned int uiCountPickRegionPixel, unsigned int aauiNamePickRegionPixel[][NAMEARRAYSIZE]);

protected: // シリアル化からのみ作成します。
    CModelViewerView();
    DECLARE_DYNCREATE(CModelViewerView)

    // 属性
public:
    CModelViewerDoc* GetDocument() const;
    std::string GetPropertyById(std::string strId) const;

    // 操作
public:

    // オーバーライド
public:
    //virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

    // ENTITY_LIST処理（START）
    void ReadSatFile(bool is_add);
    void SetEntityMap(ENTITY_LIST& entity_list);
    COGLJText* CreatePMI(AcisMesh::EdgeVertex edgeVertex);
    COGLJText* CreateEdgePMI(ENTITY* edge);
    COGLJText* CreateVertexPMI(double vertex[3]);

    void CreateEnitiyTreeItems(const ENTITY_LIST& entity_list, std::string file_name, bool is_add);
    void CreateEnitiyTreeFaceItems(HTREEITEM& root, ENTITY* face);
    void CreateEnitiyTreeEdgeItems(HTREEITEM& edge_root, const EDGE*& edge_edge);
    // ENTITY_LIST処理（END）
    
    // AVRO処理（START）
    void ReadWeldingResultRaw(const std::string str_welding_result, const std::string schema_file_path, bool is_export_edge);
    void CreateViewTreeAvroItems(const fr::recognize_welding::RecognizedWeldingResultRaw& welding_result_raw, std::string file_name);
    void SetFeatures(const fr::recognize_welding::RecognizedWeldingResultRaw& welding_result_raw);
    void SetVirtualTopologies(const fr::recognize_welding::RecognizedWeldingResultRaw& welding_result_raw);
    
    void CreateViewTreeFeatures(HTREEITEM& avroTreeRoot, const fr::recognize_welding::RecognizedWeldingResultRaw& welding_result_raw);
    void CreateViewTreeVirtualTopologies(HTREEITEM& avroTreeRoot, const fr::recognize_welding::RecognizedWeldingResultRaw& welding_result_raw);
    void CreateViewAssemblyTree(HTREEITEM& avroTreeRoot, const fr::recognize_welding::RecognizedWeldingResultRaw& welding_result_raw);
    void CreateViewSurfaceWeldingPart(HTREEITEM& root, const fr::recognize_welding::SurfaceWeldingPart& part);
    void CreateViewLineWeldingPart(HTREEITEM& root, const fr::recognize_welding::LineWeldingPart& part);
    void CreateViewPrimaryOrSecondary(HTREEITEM& root, const fr::recognize_welding::LineWeldingPart& part);
    void CreateViewVirtualFace(HTREEITEM& root, const fr::recognize_welding::Face& face);
    void CreateViewVirtualLoop(HTREEITEM& root, const fr::recognize_welding::Loop& loop);
    void CreateViewVirtualCoedge(HTREEITEM& root, const fr::recognize_welding::Coedge& coedge);
    void CreateViewVirtualEdge(HTREEITEM& root, const fr::recognize_welding::Edge& edge);
    void CreateViewVirtualCurve(HTREEITEM& root, const fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__6__& curve);
    void CreateViewVirtualVertex(HTREEITEM& root, const fr::recognize_welding::Vertex& vertex);
    // AVRO処理（END）

    // JSON(START)
    void CreateViewJsonTreeItems(const char* fileName, json11::Json& jsonContents, bool isAdd = false);
    void CreateViewJsonTreeFeatures(HTREEITEM& featuresTreeRoot, const json11::Json& jsonContents);
    void CreateViewJsonTreePrimitive(HTREEITEM& featuresTreeRoot, const json11::Json& jsonFeatures);
    void CreateViewJsonTreeSemantic(HTREEITEM& featuresTreeRoot, const json11::Json& jsonFeatures);

    // JSON(END)

    void RenderModelSat(); // SAT描画
    void DrawArrowd(double x0, double y0, double z0, double x1, double y1, double z1); // 矢印描画
    // 実装
public:
    virtual ~CModelViewerView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

    // 生成された、メッセージ割り当て関数
public:
    afx_msg void OnFilePrintPreview();
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

    afx_msg void OnViewRotation();
    afx_msg void OnViewPan();
    afx_msg void OnViewZoom();
    afx_msg void OnUpdateViewRotation(CCmdUI* pCmdUI);
    afx_msg void OnUpdateViewPan(CCmdUI* pCmdUI);
    afx_msg void OnUpdateViewZoom(CCmdUI* pCmdUI);
    afx_msg void OnViewRenderpoint();
    afx_msg void OnViewRenderline();
    afx_msg void OnViewRenderface();
    afx_msg void OnUpdateViewRenderpoint(CCmdUI* pCmdUI);
    afx_msg void OnUpdateViewRenderline(CCmdUI* pCmdUI);
    afx_msg void OnUpdateViewRenderface(CCmdUI* pCmdUI);
    afx_msg void OnViewPickpoint();
    afx_msg void OnViewPickline();
    afx_msg void OnViewPickface();
    afx_msg void OnUpdateViewPickpoint(CCmdUI* pCmdUI);
    afx_msg void OnUpdateViewPickline(CCmdUI* pCmdUI);
    afx_msg void OnUpdateViewPickface(CCmdUI* pCmdUI);
    afx_msg void OnFileOpen();
    afx_msg void OnFileNew();
    afx_msg void OnFileOpenSat();
    afx_msg void OnFileOpenAddSat();
    afx_msg void OnFileOpenAvro();      //ID_FILE_OPEN_AVRO
    afx_msg void OnFileOpenJson();      //ID_FILE_OPEN_JSON
    afx_msg void OnFileSaveAs();      //ID_FILE_OPEN_JSON
    afx_msg void OnSetLookAtCenter();   //

    afx_msg void OnShapePoint();
    // Edge作成
    afx_msg void OnShapeEdgeStraight();
    afx_msg void OnShapeEdgeEllipse();
    // Face作成
    afx_msg void OnShapeFaceCone();
    afx_msg void OnShapeFacePlane();
    afx_msg void OnShapeFaceSphere();
    afx_msg void OnShapeFaceSpline();
    afx_msg void OnShapeFaceTorus();
    // Solid作成
    afx_msg void OnShapeSolidBlock();
    afx_msg void OnShapeSolidCylinder();
    afx_msg void OnShapeSolidSphere();
    afx_msg void OnShapeSolidTorus();

    afx_msg void OnApiSubtract();
    afx_msg void OnApiUnite();
    DECLARE_MESSAGE_MAP()
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnApiSubtractBnClickedOk();
};

#ifndef _DEBUG  // ModelViewerView.cpp のデバッグ バージョン
inline CModelViewerDoc* CModelViewerView::GetDocument() const
{
    return reinterpret_cast<CModelViewerDoc*>(m_pDocument);
}
#endif