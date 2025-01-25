#include "pch.h"
#include <string>
#include "framework.h"
#include "ModelViewerView.h"
#include "ViewTreeAvro.h"
#include "MainFrm.h"
#include "setting.h"

#include "avro_fr_utility.hpp"

// START ACIS
#include "lists.hxx"
#include "entity.hxx"
#include "kernapi.hxx"
// END ACIS

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTreeAvro

CViewTreeAvro::CViewTreeAvro() noexcept
{
}

CViewTreeAvro::~CViewTreeAvro()
{
}

BEGIN_MESSAGE_MAP(CViewTreeAvro, CTreeCtrl)
    ON_NOTIFY_REFLECT(TVN_SELCHANGING, &CViewTreeAvro::OnTvnSelchanging)
    ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CViewTreeAvro::OnTvnSelchanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTreeAvro メッセージ ハンドラー

BOOL CViewTreeAvro::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

    NMHDR* pNMHDR = (NMHDR*)lParam;
    ASSERT(pNMHDR != nullptr);

#pragma warning(suppress : 26454)
    if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != nullptr)
    {
        GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
    }

    return bRes;
}


void CViewTreeAvro::OnTvnSelchanging(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    // TODO: ここにコントロール通知ハンドラー コードを追加します。

    *pResult = 0;
}

static bool bflag = false;
void CViewTreeAvro::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

    auto selected_item = pNMTreeView->itemNew.hItem;
    if (selected_item != NULL)
    {
        CMainFrame* p_frame = (CMainFrame*)AfxGetMainWnd();
        if (p_frame)
        {
            CModelViewerView* p_view = (CModelViewerView*)p_frame->GetActiveView();
            if (p_view)
            {
                p_view->m_selected_list.clear();
                if (Setting::IS_DISPALY_CLICKED_PMI)
                {
                    for (int i = 0; i < p_view->m_pmi_list.size(); ++i)
                    {
                        if (p_view->m_pmi_list[i])
                        {
                            delete p_view->m_pmi_list[i];
                        }
                    }
                    p_view->m_pmi_list.clear();
                }

                std::string* showData = (std::string*)this->GetItemData(selected_item);
                if (showData)
                {
                    std::string str_uid(*showData);
                    std::vector<std::string> selected_topology;
                    // SurfaceWeldingのtopPartとbottomPart
                    auto surface_welding_find_it = p_view->m_id_to_surface_weldings.find(str_uid); 
                    if (surface_welding_find_it != p_view->m_id_to_surface_weldings.end())
                    {
                        for (const auto& part : surface_welding_find_it->second.parts)
                        {
                            // LineWeldingPartのprimaryEdge取得
                            auto line_welding_part_find_it = p_view->m_id_to_line_welding_parts.find(part);
                            if (line_welding_part_find_it != p_view->m_id_to_line_welding_parts.end())
                            {
                                selected_topology.emplace_back(line_welding_part_find_it->second.primaryEdge);
                                selected_topology.emplace_back(line_welding_part_find_it->second.secondaryEdge);
                            }
                        }
                        str_uid.clear();
                    }

                    // SideWeldingのtopPartとbottomPart
                    auto side_welding_find_it = p_view->m_id_to_side_weldings.find(str_uid);
                    if (side_welding_find_it != p_view->m_id_to_side_weldings.end())
                    {
                        // LineWeldingPartのprimaryEdge取得
                        auto line_welding_part_find_it = p_view->m_id_to_line_welding_parts.find(side_welding_find_it->second.topPart);
                        if (line_welding_part_find_it != p_view->m_id_to_line_welding_parts.end())
                        {
                            selected_topology.emplace_back(line_welding_part_find_it->second.primaryEdge);
                            selected_topology.emplace_back(line_welding_part_find_it->second.secondaryEdge);
                        }
                        line_welding_part_find_it = p_view->m_id_to_line_welding_parts.find(side_welding_find_it->second.bottomPart);
                        if (line_welding_part_find_it != p_view->m_id_to_line_welding_parts.end())
                        {
                            selected_topology.emplace_back(line_welding_part_find_it->second.primaryEdge);
                            selected_topology.emplace_back(line_welding_part_find_it->second.secondaryEdge);
                        }
                        str_uid.clear();
                    }
                    // LineWeldingPartのprimaryEdge取得
                    auto line_welding_part_find_it = p_view->m_id_to_line_welding_parts.find(str_uid);
                    if (line_welding_part_find_it != p_view->m_id_to_line_welding_parts.end())
                    {
                        selected_topology.emplace_back(line_welding_part_find_it->second.primaryEdge);
                        selected_topology.emplace_back(line_welding_part_find_it->second.secondaryEdge);
                        str_uid.clear();
                    }

                    if (!str_uid.empty())
                    {
                        selected_topology.emplace_back(str_uid);
                    }

                    for(const auto& str_uid : selected_topology)
                    {
                        auto face_find_it = p_view->m_uid_to_face_map.find(str_uid);
                        if (face_find_it != p_view->m_uid_to_face_map.end())
                        {
                            p_view->m_selected_list.add(face_find_it->second);
                        }
                        else
                        {
                            auto edge_find_it = p_view->m_uid_to_edge_map.find(str_uid);
                            if (edge_find_it != p_view->m_uid_to_edge_map.end())
                            {
                                p_view->m_selected_list.add(edge_find_it->second);
                            }
                            else
                            {
                                auto vertex_find_it = p_view->m_uid_to_vertex_map.find(str_uid);
                                if (vertex_find_it != p_view->m_uid_to_vertex_map.end())
                                {
                                    p_view->m_selected_list.add(vertex_find_it->second);
                                }
                                
                            }
                        }
                    }

                    p_frame->m_wndAvroView.m_dlgAvroProperty.m_propertyEdit = p_view->GetPropertyById(str_uid).c_str();
                    p_frame->m_wndAvroView.m_dlgAvroProperty.UpdateData(FALSE);
                    p_frame->m_wndAvroView.m_dlgAvroProperty.Invalidate();
                }
                p_view->Invalidate();
            }
        }
    }
    *pResult = 0;
}
