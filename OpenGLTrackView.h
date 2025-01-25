
// OpenGLTrackView.h : COpenGLTrackView クラスのインターフェイス
//

#pragma once

#include "OpenGLBaseView.h"

// TrackingMode
enum ETRACKINGMODE
{
    TM_NONE = 0,
    TM_ROTATION,
    TM_PAN,
    TM_ZOOM,
};

class COpenGLTrackView : public COpenGLBaseView
{
    // メンバ変数
private:
    CPoint            m_ptLast;
    GLfloat            m_f3RenderingCenter[3];        // 描画中心
    GLfloat            m_fRenderingRate;            // 拡大率
    GLfloat            m_f16ViewingTransformMatrix[16];
protected:
    ETRACKINGMODE    m_eTrackingMode;

    // メンバ関数
private:
    void    BeginTracking( CPoint point, ETRACKINGMODE eTrackingMode );
    void    EndTracking();
    bool    SetupProjectionMatrix();
    bool    SetupModelViewMatrix();
protected:
    virtual void    DoTracking( CPoint point );

protected: // シリアル化からのみ作成します。
    COpenGLTrackView();
    DECLARE_DYNCREATE(COpenGLTrackView)

// 属性
public:
    CDocument* GetDocument() const;

// 操作
public:

// オーバーライド
public:
//    virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 実装
public:
    virtual ~COpenGLTrackView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
    afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
    afx_msg void OnMButtonDown( UINT nFlags, CPoint point );
    afx_msg void OnMButtonUp( UINT nFlags, CPoint point );
    afx_msg void OnMouseMove( UINT nFlags, CPoint point );
    afx_msg BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
};

#ifndef _DEBUG  // OpenGLTrackView.cpp のデバッグ バージョン
inline CDocument* COpenGLTrackView::GetDocument() const
   { return reinterpret_cast<CDocument*>(m_pDocument); }
#endif

