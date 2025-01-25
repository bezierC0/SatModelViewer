
// OpenGLPickView.cpp : COpenGLPickView クラスの実装
//

#include "pch.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
//#include "OpenGLPick.h"
#endif

//#include "OpenGLPickDoc.h"
#include "OpenGLPickView.h"
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenGLPickView

IMPLEMENT_DYNCREATE(COpenGLPickView, COpenGLTrackView)

BEGIN_MESSAGE_MAP(COpenGLPickView, COpenGLTrackView)
    ON_WM_SIZE()
    ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// COpenGLPickView コンストラクション/デストラクション

COpenGLPickView::COpenGLPickView()
{
    // TODO: 構築コードをここに追加します。
    memset( m_auiName_picked, 0x00, NAMEARRAYSIZE * sizeof( unsigned int ) );

    m_uiTextureID = 0;
    m_uiRenderBufferID = 0;
    m_uiFrameBufferID = 0;

    m_pOGLJText_x = new COGLJText(_T("Meiryo UI"), 16, COGLJText::OP_LEFTTOP);
    m_pOGLJText_x->Format(_T("X"));
    m_pOGLJText_y = new COGLJText(_T("Meiryo UI"), 16, COGLJText::OP_LEFTTOP);
    m_pOGLJText_y->Format(_T("Y"));
    m_pOGLJText_z = new COGLJText(_T("Meiryo UI"), 16, COGLJText::OP_LEFTTOP);
    m_pOGLJText_z->Format(_T("Z"));

}

COpenGLPickView::~COpenGLPickView()
{
    if (m_pOGLJText_x)
    {
        delete m_pOGLJText_x;
    }
    if (m_pOGLJText_y)
    {
        delete m_pOGLJText_y;
    }
    if (m_pOGLJText_z)
    {
        delete m_pOGLJText_z;
    }
    if (m_pOGLJText_drawrate)
    {
        delete m_pOGLJText_drawrate;
    }
}

BOOL COpenGLPickView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
    //  修正してください。

    return COpenGLTrackView::PreCreateWindow(cs);
}

// COpenGLPickView 描画

//void COpenGLPickView::OnDraw(CDC* /*pDC*/)
//{
//    CDocument* pDoc = GetDocument();
//    ASSERT_VALID(pDoc);
//    if (!pDoc)
//        return;
//
//    // TODO: この場所にネイティブ データ用の描画コードを追加します。
//}


// COpenGLPickView 診断

#ifdef _DEBUG
void COpenGLPickView::AssertValid() const
{
    COpenGLTrackView::AssertValid();
}

void COpenGLPickView::Dump(CDumpContext& dc) const
{
    COpenGLTrackView::Dump(dc);
}

CDocument* COpenGLPickView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDocument)));
    return (CDocument*)m_pDocument;
}
#endif //_DEBUG

void COpenGLPickView::CreateFrameBuffer( int iWidth, int iHeight )
{
    // 名前の生成
    if( 0 == m_uiTextureID )
    {
        glGenTextures( 1, &m_uiTextureID );
        glGenRenderbuffers( 1, &m_uiRenderBufferID );
        glGenFramebuffers( 1, &m_uiFrameBufferID );
    }

    // テクスチャの構築
    glBindTexture( GL_TEXTURE_2D, m_uiTextureID );    // テクスチャのバインド
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_cx, m_cy, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );    // もっとも近いセルの色を使用＝色補間しない。GL_LINEARは、色補間する。
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );    // もっとも近いセルの色を使用＝色補間しない。GL_LINEARは、色補間する。
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP ); // 画像の端のピクセルの色を使う
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP ); // 画像の端のピクセルの色を使う
    glBindTexture( GL_TEXTURE_2D, 0 );    // バインドの解除

    // レンダーバッファーの構築
    glBindRenderbuffer( GL_RENDERBUFFER, m_uiRenderBufferID );        // レンダーバッファのバインド
    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_cx, m_cy );        // レンダーバッファの深度、サイズの設定
    glBindRenderbuffer( GL_RENDERBUFFER, 0 ); // バインドの解除
}

void COpenGLPickView::DestroyFrameBuffer()
{
    if( 0 == m_uiTextureID )
    {
        return;
    }

    glDeleteFramebuffers( 1, &m_uiFrameBufferID );
    glDeleteRenderbuffers( 1, &m_uiRenderBufferID );
    glDeleteTextures( 1, &m_uiTextureID );

    m_uiFrameBufferID = 0;
    m_uiRenderBufferID = 0;
    m_uiTextureID = 0;
}

void COpenGLPickView::UninitializeOpenGL()
{
    DestroyFrameBuffer();

    COpenGLTrackView::UninitializeOpenGL();
}

bool COpenGLPickView::RenderScene()
{
    RenderModel( RM_RENDER );

    // 軸の先端に日本語文字列の描画
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(10.0, 0.0, 0.0);
    m_pOGLJText_x->Render();
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(0.0, 10.0, 0.0);
    m_pOGLJText_y->Render();
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(0.0, 0.0, 10.0);
    m_pOGLJText_z->Render();

    return true;
}

void COpenGLPickView::RenderModel( ERENDERMODE eRenderMode )
{
    unsigned int uiCountPoint = 8;
    float points[8][3] = { -5, -5, -5,
                              5, -5, -5,
                              5,  5, -5,
                             -5,  5, -5,
                             -5, -5,  5,
                              5, -5,  5,
                              5,  5,  5,
                             -5,  5,  5 };
    unsigned int uiCountTriangle = 12;
    unsigned int triangles[12][3] = { 0, 1, 3,
                              1, 2, 3,
                              1, 5, 2,
                              5, 6, 2,
                              5, 4, 6,
                              4, 7, 6,
                              4, 0, 7,
                              0, 3, 7,
                              3, 2, 7,
                              2, 6, 7,
                              4, 5, 0,
                              5, 1, 0 };
    unsigned int uiIndexTriangle;
    unsigned int uiIndexLine;
    unsigned int uiIndexPoint;
    unsigned int ui3;

    glPushAttrib( GL_ALL_ATTRIB_BITS );    // アトリビュート復帰できるように、一つ追加

    // 面
    if( RM_RENDER == eRenderMode )
    {
        glColor3f( 0.5f, 0.5f, 0.0f );
    }
    if( RM_PICK_ELEMENTTYPE == eRenderMode )
    {
        glColor3f( 1.0f, 0.0f, 0.0f );
    }
    for( uiIndexTriangle = 0; uiIndexTriangle < uiCountTriangle; uiIndexTriangle++ )
    {
        if( RM_PICK_ELEMENTID == eRenderMode )
        {
            glColor3ub( GetRValue(uiIndexTriangle), GetGValue(uiIndexTriangle), GetBValue(uiIndexTriangle) );
        }
        glBegin( GL_TRIANGLES );
        glVertex3fv( points[triangles[uiIndexTriangle][0]] );
        glVertex3fv( points[triangles[uiIndexTriangle][1]] );
        glVertex3fv( points[triangles[uiIndexTriangle][2]] );
        glEnd();
    }
    // ピック面の描画
    if( RM_RENDER == eRenderMode )
    {
        if( RET_FACE == m_auiName_picked[1] )
        {
            uiIndexTriangle = m_auiName_picked[2];
            glColor3f( 1.0f, 1.0f, 0.0f );    // ピック色
            glBegin( GL_TRIANGLES );
            glVertex3fv( points[triangles[uiIndexTriangle][0]] );
            glVertex3fv( points[triangles[uiIndexTriangle][1]] );
            glVertex3fv( points[triangles[uiIndexTriangle][2]] );
            glEnd();
        }
    }
    // 線
    glLineWidth( 2.0 );
    if( RM_RENDER == eRenderMode )
    {
        glColor3f( 0.0f, 0.5f, 0.5f );
    }
    if( RM_PICK_ELEMENTTYPE == eRenderMode )
    {
        glColor3f( 0.0f, 1.0f, 0.0f );
    }
    for( uiIndexTriangle = 0; uiIndexTriangle < uiCountTriangle; ++uiIndexTriangle )
    {
        for( ui3 = 0; ui3 < 3; ++ui3 )
        {
            if( RM_PICK_ELEMENTID == eRenderMode )
            {
                uiIndexLine = uiIndexTriangle * 3 + ui3;
                glColor3ub( GetRValue(uiIndexLine), GetGValue(uiIndexLine), GetBValue(uiIndexLine) );
            }
            glBegin( GL_LINES );
            glVertex3fv( points[triangles[uiIndexTriangle][( ui3 + 0 ) % 3]] );
            glVertex3fv( points[triangles[uiIndexTriangle][( ui3 + 1 ) % 3]] );
            glEnd();
        }
    }
    // ピック線の描画
    if( RM_RENDER == eRenderMode )
    {
        if( RET_LINE == m_auiName_picked[1] )
        {
            uiIndexLine = m_auiName_picked[2];
            uiIndexTriangle = uiIndexLine / 3;
            ui3 = uiIndexLine % 3;
            glLineWidth( 5.0f );
            glColor3f( 0.0f, 1.0f, 1.0f );
            glBegin( GL_LINES );
            glVertex3fv( points[triangles[uiIndexTriangle][( ui3 + 0 ) % 3]] );
            glVertex3fv( points[triangles[uiIndexTriangle][( ui3 + 1 ) % 3]] );
            glEnd();
        }
    }
    // 点
    glPointSize( 5.0 );
    if( RM_RENDER == eRenderMode )
    {
        glColor3f( 0.5f, 0.0f, 0.5f );
    }
    if( RM_PICK_ELEMENTTYPE == eRenderMode )
    {
        glColor3f( 0.0f, 0.0f, 1.0f );
    }
    for( uiIndexPoint = 0; uiIndexPoint < uiCountPoint; uiIndexPoint++ )
    {
        if( RM_PICK_ELEMENTID == eRenderMode )
        {
            glColor3ub( GetRValue(uiIndexPoint), GetGValue(uiIndexPoint), GetBValue(uiIndexPoint) );
        }
        glBegin( GL_POINTS );
        glVertex3fv( points[uiIndexPoint] );
        glEnd();
    }
    // ピック点の描画
    if( RM_RENDER == eRenderMode )
    {
        if( RET_POINT == m_auiName_picked[1] )
        {
            glPointSize( 10.0f );
            glColor3f( 1.0f, 0.0f, 1.0f );
            uiIndexPoint = m_auiName_picked[2];
            glBegin( GL_POINTS );
            glVertex3fv( points[uiIndexPoint] );
            glEnd();
        }
    }

    glPopAttrib();    // アトリビュートの復帰
    return;
}

// ピッキング
int COpenGLPickView::DoPicking( int x, int y )
{
    // フレームバッファのバインド
    glBindFramebuffer( GL_FRAMEBUFFER, m_uiFrameBufferID );
    // カラーアタッチメントとしてテクスチャを指定する
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uiTextureID, 0 );
    // 深度アタッチメントとしてレンダーバッファを指定する
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_uiRenderBufferID );

    int iCountHit = 0;
    int iStatus = glCheckFramebufferStatus( GL_FRAMEBUFFER );
    if( GL_FRAMEBUFFER_COMPLETE != iStatus )
    {    // フレームバッファーが万全でない
        // ウィンドウシステムが提供するフレームバッファに差し替え
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        return false;
    }

    GLubyte r;
    GLubyte g;
    GLubyte b;

    // 名前列の配列
    unsigned int aauiNamePickRegionPixel[PICKREGIONPIXELCOUNT][NAMEARRAYSIZE];
    // 名前列の配列の初期化
    memset( aauiNamePickRegionPixel, 0x00, PICKREGIONPIXELCOUNT * NAMEARRAYSIZE * sizeof( unsigned int ) );
    // ピクセル色の配列
    GLubyte abtColorPickRegionPixel[PICKREGIONPIXELCOUNT * 4];
    // ピクセル色の配列の初期化
    memset( abtColorPickRegionPixel, 0x00, PICKREGIONPIXELCOUNT * 4 * sizeof( GLubyte ) );

    glDisable( GL_LIGHTING ); // 光
    glEnable( GL_BLEND ); // 半透明およびアンチエイリアシング
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ピック描画（要素タイプ別）
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    RenderModel( RM_PICK_ELEMENTTYPE );
    glReadPixels( x - PICKREGIONOFFSET,
        m_cy - y - PICKREGIONOFFSET,
        1 + 2 * PICKREGIONOFFSET,
        1 + 2 * PICKREGIONOFFSET,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        abtColorPickRegionPixel );
    // ピック領域の色をピック名配列に変換
    for( unsigned int ui = 0; ui < PICKREGIONPIXELCOUNT; ++ui )
    {
        r = abtColorPickRegionPixel[ui * 4 + 0];
        g = abtColorPickRegionPixel[ui * 4 + 1];
        b = abtColorPickRegionPixel[ui * 4 + 2];
        if( 255 == r )
        { // 面
            aauiNamePickRegionPixel[ui][1] = RET_FACE;
            ++iCountHit;
        }
        else if( 255 == g )
        { // 線
            aauiNamePickRegionPixel[ui][1] = RET_LINE;
            ++iCountHit;
        }
        else if( 255 == b )
        { // 点
            aauiNamePickRegionPixel[ui][1] = RET_POINT;
            ++iCountHit;
        }
    }
    if( 0 != iCountHit )
    { // ヒットあり
        // ピック描画（要素番号別）
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        RenderModel( RM_PICK_ELEMENTID );
        glReadPixels( x - PICKREGIONOFFSET,
            m_cy - y - PICKREGIONOFFSET,
            1 + 2 * PICKREGIONOFFSET,
            1 + 2 * PICKREGIONOFFSET,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            abtColorPickRegionPixel );
        // ピック領域の色をピック名配列に変換
        for( unsigned int ui = 0; ui < PICKREGIONPIXELCOUNT; ui++ )
        {
            r = abtColorPickRegionPixel[ui * 4 + 0];
            g = abtColorPickRegionPixel[ui * 4 + 1];
            b = abtColorPickRegionPixel[ui * 4 + 2];
            // 色を番号に変換し、名前配列にセット
            aauiNamePickRegionPixel[ui][2] = RGB( r, g, b );
        }

        // ピック領域ピクセルを鑑定し、ピック要素を特定する
        IdentifyPickRegionPixels( PICKREGIONPIXELCOUNT, aauiNamePickRegionPixel );
    }

    // ウィンドウシステムが提供するフレームバッファに差し替え
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );

    return ( 0 != iCountHit );
}

// ピック領域ピクセルを鑑定し、ピック要素を特定する
void COpenGLPickView::IdentifyPickRegionPixels( unsigned int uiCountPickRegionPixel, unsigned int aauiNamePickRegionPixel[][NAMEARRAYSIZE] )
{
    int  iId_selected = -1;
    unsigned int  uiElementType_selected = RET_FACE + 1;
    long lSquareDist_selected = ( 2 + PICKREGIONOFFSET ) * ( 2 + PICKREGIONOFFSET ) + ( 2 + PICKREGIONOFFSET ) * ( 2 + PICKREGIONOFFSET );
    long lSquareDist_current;
    int  x;
    int  y;
    for( unsigned int ui = 0; ui < uiCountPickRegionPixel; ui++ )
    {
        if( 0 == aauiNamePickRegionPixel[ui][1] )
        { // モデルの外側
            continue;
        }
        if( uiElementType_selected < aauiNamePickRegionPixel[ui][1] )
        { // 要素タイプ的に、優先順位が低い
            continue;
        }
        if( uiElementType_selected > aauiNamePickRegionPixel[ui][1] )
        { // 要素タイプ的に、優先順位が低い
            iId_selected = ui;
            uiElementType_selected = aauiNamePickRegionPixel[ui][1];
            x = ui % ( 1 + 2 * PICKREGIONOFFSET ) - PICKREGIONOFFSET;
            y = ui / ( 1 + 2 * PICKREGIONOFFSET ) - PICKREGIONOFFSET;
            lSquareDist_selected = x * x + y * y;
            continue;
        }
        // 要素タイプ的に、優先順位が同じ場合は、ピック領域の中心に近いものが優先度が高い。
        x = ui % ( 1 + 2 * PICKREGIONOFFSET ) - PICKREGIONOFFSET;
        y = ui / ( 1 + 2 * PICKREGIONOFFSET ) - PICKREGIONOFFSET;
        lSquareDist_current = x * x + y * y;
        if( lSquareDist_selected > lSquareDist_current )
        {
            iId_selected = ui;
            uiElementType_selected = aauiNamePickRegionPixel[ui][1];
            lSquareDist_selected = lSquareDist_current;
            continue;
        }
    }

    if( -1 == iId_selected )
    {
        return;
    }

    // 名前列メンバの更新
    memcpy( m_auiName_picked, aauiNamePickRegionPixel[iId_selected], NAMEARRAYSIZE * sizeof( unsigned int ) );
}

// COpenGLPickView メッセージ ハンドラー


void COpenGLPickView::OnSize( UINT nType, int cx, int cy )
{
    COpenGLTrackView::OnSize( nType, cx, cy );

    // TODO: ここにメッセージ ハンドラー コードを追加します。
    if( 0 >= cx || 0 >= cy )
    {
        return;
    }

    CreateFrameBuffer( cx, cy );
}


void COpenGLPickView::OnRButtonUp( UINT nFlags, CPoint point )
{
    // TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
    if( 0 < DoPicking( point.x, point.y ) )
    {    // ピックに成功したら再描画
        Invalidate( FALSE );
        return;
    }

    COpenGLTrackView::OnRButtonUp( nFlags, point );
}
