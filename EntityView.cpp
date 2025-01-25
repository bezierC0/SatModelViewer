
#include "pch.h"
#include "framework.h"
#include "mainfrm.h"
#include "EntityView.h"
#include "Resource.h"
#include "ModelViewer.h"
#include "ModelViewerView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntityView

CEntityView::CEntityView() noexcept
{
}

CEntityView::~CEntityView()
{
}

BEGIN_MESSAGE_MAP(CEntityView, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_CONTEXTMENU()
    ON_COMMAND(ID_PROPERTIES, OnProperties)
    ON_COMMAND(ID_OPEN, OnFileOpen)
    ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
    ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
    ON_COMMAND(ID_EDIT_CUT, OnEditCut)
    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
    ON_WM_PAINT()
    ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar メッセージ ハンドラー

int CEntityView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rectDummy;
    rectDummy.SetRectEmpty();

    // ビューの作成:
    const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_CHECKBOXES;

    if (!m_wndEntityView.Create(dwViewStyle, rectDummy, this, 4))
    {
        TRACE0("TreeViewを作成できませんでした\n");
        return -1;      // 作成できない場合
    }

    // ビューのイメージの読み込み:
    //m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
    //m_wndEntityView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

    m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
    m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ロックされています*/);

    OnChangeVisualStyle();

    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

    m_wndToolBar.SetOwner(this);

    // すべてのコマンドが、親フレーム経由ではなくこのコントロール経由で渡されます:
    m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

    // 静的ツリー ビュー データ (ダミー コード) を入力します
    FillFileView();

    m_dlgEntityProperty.Create(IDD_DIALOG_ENTITY_PROPERTY, this);

    AdjustLayout();

    return 0;
}

void CEntityView::OnSize(UINT nType, int cx, int cy)
{
    CDockablePane::OnSize(nType, cx, cy);
    AdjustLayout();
}

void CEntityView::FillFileView()
{

}

void CEntityView::OnContextMenu(CWnd* pWnd, CPoint point)
{
    CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndEntityView;
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
    theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CEntityView::AdjustLayout()
{
    if (GetSafeHwnd() == nullptr)
    {
        return;
    }

    CRect rectClient;
    GetClientRect(rectClient);

    int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

    m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
    m_wndEntityView.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CEntityView::OnProperties()
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
    
    HTREEITEM selected_item = p_frame->m_wndEntityView.m_wndEntityView.GetSelectedItem();
    if (selected_item != nullptr)
    {
        ENTITY* selectItem = (ENTITY*)p_frame->m_wndEntityView.m_wndEntityView.GetItemData(selected_item);
        if (selectItem != nullptr)
        {
            std::string info("");
            info += get_entity_type(selectItem);
            info += get_entity_info(selectItem);
            p_frame->m_wndEntityView.m_dlgEntityProperty.m_propertyEdit = info.c_str();
            p_frame->m_wndEntityView.m_dlgEntityProperty.UpdateData(FALSE);
            p_frame->m_wndEntityView.m_dlgEntityProperty.Invalidate();
        }

    }
    m_dlgEntityProperty.ShowWindow(SW_SHOW);
}

void CEntityView::OnFileOpen()
{
    // TODO: ここにコマンド ハンドラー コードを追加します
}

void CEntityView::OnFileOpenWith()
{
    // TODO: ここにコマンド ハンドラー コードを追加します
}

void CEntityView::OnDummyCompile()
{
    // TODO: ここにコマンド ハンドラー コードを追加します
}

void CEntityView::OnEditCut()
{
    // TODO: ここにコマンド ハンドラー コードを追加します
}

void CEntityView::OnEditCopy()
{
    // TODO: ここにコマンド ハンドラー コードを追加します
}

void CEntityView::OnEditClear()
{
    // TODO: ここにコマンド ハンドラー コードを追加します
}

void CEntityView::OnPaint()
{
    CPaintDC dc(this); // 描画のデバイス コンテキスト

    CRect rectTree;
    m_wndEntityView.GetWindowRect(rectTree);
    ScreenToClient(rectTree);

    rectTree.InflateRect(1, 1);
    dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CEntityView::OnSetFocus(CWnd* pOldWnd)
{
    CDockablePane::OnSetFocus(pOldWnd);

    m_wndEntityView.SetFocus();
}

void CEntityView::OnChangeVisualStyle()
{
    m_wndToolBar.CleanUpLockedImages();
    m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ロックされました*/);

    //m_FileViewImages.DeleteImageList();

    UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

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

    //m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
    //m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

    //m_wndEntityView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}


