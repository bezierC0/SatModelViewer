
// OpenGLPickView.h : COpenGLPickView クラスのインターフェイス
//

#pragma once

#include "OpenGLTrackView.h"

#include "OGLJText.h"

#define NAMEARRAYSIZE           (10)        // 名前列の大きさ
#define PICKREGIONOFFSET        (5)         // ピック領域のオフセット量
#define PICKREGIONPIXELCOUNT    ( 1 + 2 * PICKREGIONOFFSET ) * ( 1 + 2 * PICKREGIONOFFSET )    // ピック領域のピクセル数

// Render Mode
enum ERENDERMODE
{
    RM_UNDEFINED = 0,
    RM_RENDER,            // 通常描画
    RM_PICK_ELEMENTTYPE,// ピック描画（要素タイプ）
    RM_PICK_ELEMENTID,    // ピック描画（要素番号）
};

enum ERENDERELEMENTTYPE
{
    RET_UNDEFINED = 0,
    RET_POINT,
    RET_LINE,
    RET_FACE,
};

class COpenGLPickView : public COpenGLTrackView
{
    // メンバ変数
private:
    GLuint            m_uiTextureID;
    GLuint            m_uiRenderBufferID;
    GLuint            m_uiFrameBufferID;
protected:
    unsigned int    m_auiName_picked[NAMEARRAYSIZE];    // ピックされた要素の名前列
    COGLJText*      m_pOGLJText_x = nullptr;
    COGLJText*      m_pOGLJText_y = nullptr;
    COGLJText*      m_pOGLJText_z = nullptr;
    COGLJText*      m_pOGLJText_drawrate = nullptr;

    // メンバ関数
private:
    void    CreateFrameBuffer( int iWidth, int iHeight );
    void    DestroyFrameBuffer();
    void    UninitializeOpenGL();
    bool    RenderScene();
    int     DoPicking( int x, int y );
protected:
    virtual void    RenderModel( ERENDERMODE eRenderMode );
    virtual void    IdentifyPickRegionPixels( unsigned int uiCountPickRegionPixel, unsigned int  aauiNamePickRegionPixel[][NAMEARRAYSIZE] );

protected: // シリアル化からのみ作成します。
    COpenGLPickView();
    DECLARE_DYNCREATE(COpenGLPickView)

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
    virtual ~COpenGLPickView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnSize( UINT nType, int cx, int cy );
    afx_msg void OnRButtonUp( UINT nFlags, CPoint point );
};

#ifndef _DEBUG  // OpenGLPickView.cpp のデバッグ バージョン
inline CDocument* COpenGLPickView::GetDocument() const
   { return reinterpret_cast<CDocument*>(m_pDocument); }
#endif

