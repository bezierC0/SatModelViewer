
// MainFrm.h : CMainFrame クラスのインターフェイス
//

#pragma once
#include "EntityView.h"
#include "CAvroView.h"
#include "CJsonView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "CDialogApiSubtract.h"

class CMainFrame : public CFrameWndEx
{

protected: // シリアル化からのみ作成します。
    CMainFrame() noexcept;
    DECLARE_DYNCREATE(CMainFrame)

    // 属性
public:

    // 操作
public:

    // オーバーライド
public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

    // 実装
public:
    virtual ~CMainFrame();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

public:  // コントロール バー用メンバー
    CMFCMenuBar         m_wndMenuBar;
    CMFCToolBar         m_wndToolBar;
    CMFCStatusBar       m_wndStatusBar;
    COutputWnd          m_wndOutput;
    CPropertiesWnd      m_wndProperties;
    CEntityView         m_wndEntityView;    // satTreeView
    CAvroView           m_wndAvroView;      // avroTreeView
    CJsonView           m_wndJsonView;      // jsonTreeView

    CDialogApiSubtract  m_dlgApiSubtract; // 分割Dialog

    // 生成された、メッセージ割り当て関数
protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnViewCustomize();
    afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
    afx_msg void OnApplicationLook(UINT id);
    afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    DECLARE_MESSAGE_MAP()

    BOOL CreateDockingWindows();
    void SetDockingWindowIcons(BOOL bHiColorIcons);
};


