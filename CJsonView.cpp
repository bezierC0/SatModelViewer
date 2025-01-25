#include "pch.h"
#include "CJsonView.h"
#include "framework.h"
#include "MainFrm.h"
#include "Resource.h"
#include "ModelViewer.h"
#include "ModelViewerView.h"


CJsonView::CJsonView() noexcept
{
    m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

CJsonView::~CJsonView()
{

}

BEGIN_MESSAGE_MAP(CJsonView, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_CONTEXTMENU()
    ON_WM_PAINT()
    ON_WM_SETFOCUS()
    ON_COMMAND(ID_POPUP_JSON_PICK, &CJsonView::OnPopupJsonPick)
    ON_COMMAND(ID_POPUP_JSON_CLEAR, &CJsonView::OnPopupJsonClear)
    ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

int CJsonView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rectDummy;
    rectDummy.SetRectEmpty();

    // ビューの作成:
    const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    if (!m_wndJsonTreeView.Create(dwViewStyle, rectDummy, this, 1))
    {
        TRACE0("クラス ビューを作成できませんでした\n");
        return -1;      // 作成できない場合
    }

    // イメージの読み込み:
    m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
    m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* ロックされています*/);

    OnChangeVisualStyle();

    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

    m_wndToolBar.SetOwner(this);

    // すべてのコマンドが、親フレーム経由ではなくこのコントロール経由で渡されます:
    m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

    //CMenu menuSort;
    //menuSort.LoadMenu(IDR_POPUP_SORT);

    //m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

    //CClassViewMenuButton* pButton = DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

    //if (pButton != nullptr)
    //{
    //    pButton->m_bText = FALSE;
    //    pButton->m_bImage = TRUE;
    //    pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
    //    pButton->SetMessageWnd(this);
    //}

    // 静的ツリー ビュー データ (ダミー コード) を入力します
    FillClassView();

    m_dlgAvroProperty.Create(IDD_DIALOG_AVRO_PROPERTY, this);

    return 0;
}

void CJsonView::OnSize(UINT nType, int cx, int cy)
{
    CDockablePane::OnSize(nType, cx, cy);
    AdjustLayout();
}

void CJsonView::FillClassView()
{

}

void CJsonView::OnContextMenu(CWnd* pWnd, CPoint point)
{
    CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndJsonTreeView;
    ASSERT_VALID(pWndTree);

    if (pWnd != pWndTree)
    {
        CDockablePane::OnContextMenu(pWnd, point);
        return;
    }

    if (point != CPoint(-1, -1))
    {
        // クリックされた項目の選択:
        CPoint ptTree = point;
        pWndTree->ScreenToClient(&ptTree);

        UINT flags = 0;
        HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
        if (hTreeItem != nullptr)
        {
            pWndTree->SelectItem(hTreeItem);
        }
    }

    pWndTree->SetFocus();
    CMenu menu;
    menu.LoadMenu(IDR_POPUP_SORT);

    CMenu* pSumMenu = menu.GetSubMenu(0);

    //if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
    //{
    //    CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

    //    if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
    //        return;

    //    ((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
    //    UpdateDialogControls(this, FALSE);
    //}

    if (point != CPoint(-1, -1))
    {
        // クリックされた項目の選択:
        CPoint ptTree = point;
        pWndTree->ScreenToClient(&ptTree);

        UINT flags = 0;
        HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
        if (hTreeItem != nullptr && flags == TVHT_ONITEMLABEL)
        {
            CMenu Menu;
            CPoint pt = point;
            //ClientToScreen(&pt);
            if (Menu.LoadMenu(IDR_POPUP_JSON))
            {
                CMenu* pSubMenu = Menu.GetSubMenu(0);
                if (pSubMenu != NULL)
                {
                    pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
                }
            }
        }
        else
        {
            CMenu Menu;
            CPoint pt = point;
            //ClientToScreen(&pt);
            if (Menu.LoadMenu(IDR_POPUP_JSON))
            {
                CMenu* pSubMenu = Menu.GetSubMenu(0);
                //pSubMenu->get
                if (pSubMenu != NULL)
                {
                    int count = pSubMenu->GetMenuItemCount();
                    pSubMenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED);
                    pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
                }
            }
        }
    }
}

void CJsonView::AdjustLayout()
{
    if (GetSafeHwnd() == nullptr)
    {
        return;
    }

    CRect rectClient;
    GetClientRect(rectClient);

    int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

    m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
    m_wndJsonTreeView.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CJsonView::PreTranslateMessage(MSG* pMsg)
{
    return CDockablePane::PreTranslateMessage(pMsg);
}

void CJsonView::OnPaint()
{
    CPaintDC dc(this); // 描画のデバイス コンテキスト

    CRect rectTree;
    m_wndJsonTreeView.GetWindowRect(rectTree);
    ScreenToClient(rectTree);

    rectTree.InflateRect(1, 1);
    dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}


void CJsonView::OnSetFocus(CWnd* pOldWnd)
{
    CDockablePane::OnSetFocus(pOldWnd);

    m_wndJsonTreeView.SetFocus();
}

void CJsonView::OnChangeVisualStyle()
{
    m_classViewImages.DeleteImageList();

    UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

    CBitmap bmp;
    if (!bmp.LoadBitmap(uiBmpId))
    {
        TRACE(_T("ビットマップを読み込めませんでした: %x\n"), uiBmpId);
        ASSERT(FALSE);
        return;
    }

    BITMAP bmpObj;
    bmp.GetBitmap(&bmpObj);

    UINT nFlags = ILC_MASK;

    nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

    m_classViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
    m_classViewImages.Add(&bmp, RGB(255, 0, 0));

    m_wndJsonTreeView.SetImageList(&m_classViewImages, TVSIL_NORMAL);


}

void CJsonView::OnPopupProperty()
{
    //m_dlgAvroProperty.ShowWindow(SW_SHOW);
}

void CJsonView::OnPopupJsonPick()
{
    CMainFrame* p_frame = (CMainFrame*)AfxGetMainWnd();
    if (!p_frame)
    {
        return;
    }
    CModelViewerView* p_view = (CModelViewerView*)p_frame->GetActiveView();
    if (!p_view)
    {
        return;
    }

    HTREEITEM selected_item = p_frame->m_wndJsonView.m_wndJsonTreeView.GetSelectedItem();
    if (selected_item != nullptr)
    {
        std::string* selectItem = (std::string*)p_frame->m_wndJsonView.m_wndJsonTreeView.GetItemData(selected_item);
        if (selectItem != nullptr && !selectItem->empty())
        {
            std::string str_uid(*selectItem);
            bool has_add = false;

            for (int i = 0; i < p_view->m_picked_string_list.size(); ++i)
            {
                if (p_view->m_picked_string_list.at(i) == str_uid)
                {
                    has_add = true;
                }
            }

            if (!has_add)
            {
                p_view->m_picked_string_list.emplace_back(str_uid);
            }

            for (auto str_uid : p_view->m_picked_string_list)
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
        }
    }
    p_view->Invalidate();
}


void CJsonView::OnPopupJsonClear()
{
    CMainFrame* p_frame = (CMainFrame*)AfxGetMainWnd();
    if (!p_frame)
    {
        return;
    }
    CModelViewerView* p_view = (CModelViewerView*)p_frame->GetActiveView();
    if (!p_view)
    {
        return;
    }

    p_view->m_picked_string_list.clear();
    p_view->m_selected_list.clear();
    p_view->Invalidate();
}


void CJsonView::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

    CDockablePane::OnRButtonUp(nFlags, point);
}
