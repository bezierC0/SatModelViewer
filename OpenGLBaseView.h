
// OpenGLBaseView.h : COpenGLBaseView クラスのインターフェイス
//

#pragma once

#include <gl\gl.h>
#include <gl\glu.h>
#include "GL/freeglut.h"
#include "OpenGLExtFuncs.h"

class COpenGLBaseView : public CView
{
    // メンバ変数
private:
    HGLRC    m_hRC;
    CDC*    m_pDC;
protected:
    int        m_cx;
    int        m_cy;

    // メンバ関数
private:
    bool    InitializeOpenGL();
    bool    SetupPixelFormat();
    bool    SetupViewport();
    bool    PreRenderScene() { return true; }
    void    RenderStockScene();
    bool    PostRenderScene() { return true; }
    void    RenderAxis();
protected:
    virtual void    UninitializeOpenGL();
    virtual bool    SetupProjectionMatrix();
    virtual bool    SetupModelViewMatrix();
    virtual bool    RenderScene() { return true; }

protected: // シリアル化からのみ作成します。
    COpenGLBaseView();
    DECLARE_DYNCREATE(COpenGLBaseView)

// 属性
public:
    double eye[3]       = { 100.0, 0.0, 0.0 };
    double center[3]    = { 0.0, 0.0, 0.0 };
    CDocument* GetDocument() const;

// 操作
public:

// オーバーライド
public:
    virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 実装
public:
    virtual ~COpenGLBaseView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg BOOL OnEraseBkgnd( CDC* pDC );
    afx_msg void OnSize( UINT nType, int cx, int cy );
    afx_msg void OnDestroy();
};

#ifndef _DEBUG  // OpenGLBaseView.cpp のデバッグ バージョン
inline CDocument* COpenGLBaseView::GetDocument() const
   { return reinterpret_cast<CDocument*>(m_pDocument); }
#endif

