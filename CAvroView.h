#pragma once

#include "ViewTreeAvro.h"
#include "CDialogAvroProperty.h"

class CClassToolBar : public CMFCToolBar
{
    virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
    {
        CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
    }

    virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CAvroView : public CDockablePane
{
public:
    CAvroView() noexcept;
    virtual ~CAvroView();

    void AdjustLayout();
    void OnChangeVisualStyle();

public:
    CClassToolBar           m_wndToolBar;
    CViewTreeAvro           m_wndAvroTreeView;
    CImageList              m_classViewImages;
    CDialogAvroProperty     m_dlgAvroProperty;
    UINT                    m_nCurrSort;


    void FillClassView();

    // オーバーライド
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnClassAddMemberFunction();
    afx_msg void OnClassAddMemberVariable();
    afx_msg void OnClassDefinition();
    afx_msg void OnClassProperties();
    afx_msg void OnFileOpenAvroToolBar();
    afx_msg void OnPaint();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
    afx_msg void OnSort(UINT id);
    afx_msg void OnUpdateSort(CCmdUI* pCmdUI);

    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnPopupProperty();
};

