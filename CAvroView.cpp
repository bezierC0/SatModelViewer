
#include "pch.h"
#include "framework.h"
#include "MainFrm.h"
#include "CAvroView.h"
#include "Resource.h"
#include "ModelViewer.h"
#include "ModelViewerView.h"

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
    friend class CAvroView;

    DECLARE_SERIAL(CClassViewMenuButton)

public:
    CClassViewMenuButton(HMENU hMenu = nullptr) noexcept : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
    {
    }

    virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
        BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
    {
        pImages = CMFCToolBar::GetImages();

        CAfxDrawState ds;
        pImages->PrepareDrawImage(ds);

        CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

        pImages->EndDrawImage(ds);
    }
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// コンストラクション/デストラクション
//////////////////////////////////////////////////////////////////////

CAvroView::CAvroView() noexcept
{
    m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

CAvroView::~CAvroView()
{
}

BEGIN_MESSAGE_MAP(CAvroView, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_CONTEXTMENU()
    ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
    ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
    ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
    ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
    ON_COMMAND(ID_NEW_FOLDER, OnFileOpenAvroToolBar)
    ON_WM_PAINT()
    ON_WM_SETFOCUS()
    ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
    ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
//    ON_WM_RBUTTONUP()
    ON_COMMAND(ID_POPUP_PROPERTY, &CAvroView::OnPopupProperty)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAvroView メッセージ ハンドラー

int CAvroView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rectDummy;
    rectDummy.SetRectEmpty();

    // ビューの作成:
    const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    if (!m_wndAvroTreeView.Create(dwViewStyle, rectDummy, this, 2))
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

    CMenu menuSort;
    menuSort.LoadMenu(IDR_POPUP_SORT);

    m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

    CClassViewMenuButton* pButton = DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

    if (pButton != nullptr)
    {
        pButton->m_bText = FALSE;
        pButton->m_bImage = TRUE;
        pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
        pButton->SetMessageWnd(this);
    }

    // 静的ツリー ビュー データ (ダミー コード) を入力します
    FillClassView();

    m_dlgAvroProperty.Create(IDD_DIALOG_AVRO_PROPERTY, this);

    return 0;
}

void CAvroView::OnSize(UINT nType, int cx, int cy)
{
    CDockablePane::OnSize(nType, cx, cy);
    AdjustLayout();
}

void CAvroView::FillClassView()
{

}

void CAvroView::OnContextMenu(CWnd* pWnd, CPoint point)
{
    CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndAvroTreeView;
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
            if (Menu.LoadMenu(IDR_AVRO_VIEW_POPUP))
            {
                CMenu* pSubMenu = Menu.GetSubMenu(0);
                if (pSubMenu != NULL)
                {
                    pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
                }
            }
        }
    }
}

void CAvroView::AdjustLayout()
{
    if (GetSafeHwnd() == nullptr)
    {
        return;
    }

    CRect rectClient;
    GetClientRect(rectClient); 

    int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

    m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
    m_wndAvroTreeView.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CAvroView::PreTranslateMessage(MSG* pMsg)
{
    return CDockablePane::PreTranslateMessage(pMsg);
}

void CAvroView::OnSort(UINT id)
{
    if (m_nCurrSort == id)
    {
        return;
    }

    m_nCurrSort = id;

    CClassViewMenuButton* pButton = DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

    if (pButton != nullptr)
    {
        pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
        m_wndToolBar.Invalidate();
        m_wndToolBar.UpdateWindow();
    }
}

void CAvroView::OnUpdateSort(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CAvroView::OnClassAddMemberFunction()
{
    AfxMessageBox(_T("メンバー関数の追加..."));
}

void CAvroView::OnClassAddMemberVariable()
{
    // TODO: ここにコマンド ハンドラー コードを追加します
}

void CAvroView::OnClassDefinition()
{
    // TODO: ここにコマンド ハンドラー コードを追加します
}

void CAvroView::OnClassProperties()
{
    // TODO: ここにコマンド ハンドラー コードを追加します
}

void CAvroView::OnFileOpenAvroToolBar()
{
    CMainFrame* p_frame = (CMainFrame*)AfxGetMainWnd();
    if (p_frame)
    {
        CModelViewerView* p_view = (CModelViewerView*)p_frame->GetActiveView();
        if (p_view)
        {
            p_view->OnFileOpenAvro();
        }
    }
}

void CAvroView::OnPaint()
{
    CPaintDC dc(this); // 描画のデバイス コンテキスト

    CRect rectTree;
    m_wndAvroTreeView.GetWindowRect(rectTree);
    ScreenToClient(rectTree);

    rectTree.InflateRect(1, 1);
    dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CAvroView::OnSetFocus(CWnd* pOldWnd)
{
    CDockablePane::OnSetFocus(pOldWnd);

    m_wndAvroTreeView.SetFocus();
}

void CAvroView::OnChangeVisualStyle()
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

    m_wndAvroTreeView.SetImageList(&m_classViewImages, TVSIL_NORMAL);

    m_wndToolBar.CleanUpLockedImages();
    m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* ロックされました*/);
}


void CAvroView::OnPopupProperty()
{
    m_dlgAvroProperty.ShowWindow(SW_SHOW);
}


