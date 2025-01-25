#pragma once
#include "CViewTreeJson.h"
#include "CDialogAvroProperty.h"



class CJsonView : public CDockablePane
{
public:
    CJsonView() noexcept;
    virtual ~CJsonView();

    void AdjustLayout();
    void OnChangeVisualStyle();

    // オーバーライド
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);


public:
    CMFCToolBar             m_wndToolBar;
    CViewTreeJson           m_wndJsonTreeView;
    CImageList              m_classViewImages;
    CDialogAvroProperty     m_dlgAvroProperty;
    UINT                    m_nCurrSort;

    void FillClassView();

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPopupProperty();

    afx_msg void OnPopupJsonPick();
    afx_msg void OnPopupJsonClear();
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};

