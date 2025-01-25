#include "pch.h"
#include <string>
#include "framework.h"
#include "ModelViewerView.h"
#include "ViewTreeEntity.h"
#include "MainFrm.h"
#include "setting.h"

#include "lists.hxx"
#include "entity.hxx"
#include "kernapi.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTreeEntity

CViewTreeEntity::CViewTreeEntity() noexcept
{
}

CViewTreeEntity::~CViewTreeEntity()
{
}

BEGIN_MESSAGE_MAP(CViewTreeEntity, CTreeCtrl)
    ON_NOTIFY_REFLECT(TVN_SELCHANGING, &CViewTreeEntity::OnTvnSelchanging)
    ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CViewTreeEntity::OnTvnSelchanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTreeEntity メッセージ ハンドラー

BOOL CViewTreeEntity::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
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


void CViewTreeEntity::OnTvnSelchanging(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    // TODO: ここにコントロール通知ハンドラー コードを追加します。

    *pResult = 0;
}

static bool bflag = false;
void CViewTreeEntity::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
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

            auto selected_item = pNMTreeView->itemNew.hItem;
            //HTREEITEM selected_item = this->GetSelectedItem();
            if (selected_item != NULL)
            {
                ENTITY* showData = (ENTITY*)this->GetItemData(selected_item);
                if (showData)
                {
                    p_view->m_selected_list.add(showData);
                    std::string info("");
                    info += get_entity_type(showData);
                    info += get_entity_info(showData);
                    p_frame->m_wndEntityView.m_dlgEntityProperty.m_propertyEdit = info.c_str();
                    p_frame->m_wndEntityView.m_dlgEntityProperty.UpdateData(FALSE);
                    p_frame->m_wndEntityView.m_dlgEntityProperty.Invalidate();
                    
                }
            }
            p_view->Invalidate();
        }
    }
    *pResult = 0;
}
