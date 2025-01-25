
// OpenGLBaseView.cpp : COpenGLBaseView クラスの実装
//

#include "pch.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
//#include "OpenGLBase.h"
#endif

//#include "OpenGLBaseDoc.h"
#include "OpenGLBaseView.h"
#include <string>

#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "freeglutd.lib" )

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenGLBaseView

IMPLEMENT_DYNCREATE(COpenGLBaseView, CView)

BEGIN_MESSAGE_MAP(COpenGLBaseView, CView)
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

// COpenGLBaseView コンストラクション/デストラクション

COpenGLBaseView::COpenGLBaseView() :
    m_hRC( NULL ),
    m_pDC( NULL ),
    m_cx( 0 ),
    m_cy( 0 )
{
    // TODO: 構築コードをここに追加します。

}

COpenGLBaseView::~COpenGLBaseView()
{
}

BOOL COpenGLBaseView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
    //  修正してください。
    cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    return CView::PreCreateWindow(cs);
}

// COpenGLBaseView 描画

void COpenGLBaseView::OnDraw(CDC* /*pDC*/)
{
    CDocument* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // TODO: この場所にネイティブ データ用の描画コードを追加します。
    if( !m_pDC )
    {
        return;
    }

    // レンダリングコンテキストの設定
    // MDI描画において必須。SDIにおいては省略可(InitializeOpenGL関数内で呼び出し済みなので）。
    // 以降のOpenGL関数による描画は、指定したレンダリングコンテキス上に描画される。
    wglMakeCurrent( m_pDC->GetSafeHdc(), m_hRC );

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    PreRenderScene();

    glPushMatrix();
    RenderStockScene();
    glPopMatrix();

    glPushMatrix();
    RenderScene();
    glPopMatrix();

    PostRenderScene();

    glFinish();

    SwapBuffers( m_pDC->GetSafeHdc() );
}

// COpenGLBaseView 診断

#ifdef _DEBUG
void COpenGLBaseView::AssertValid() const
{
    CView::AssertValid();
}

void COpenGLBaseView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CDocument* COpenGLBaseView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDocument)));
    return (CDocument*)m_pDocument;
}
#endif //_DEBUG

bool COpenGLBaseView::InitializeOpenGL()
{
    // デバイスコンテキストの作成
    m_pDC = new CClientDC( this );
    if( NULL == m_pDC )
    {
        ASSERT( !"m_pDC is NULL" );
        return false;
    }

    // OpenGL pixel format の設定
    if( !SetupPixelFormat() )
    {
        ASSERT( !"SetupPixelFormat failed" );
        return false;
    }

    // レンダリングコンテキストの作成
    m_hRC = wglCreateContext( m_pDC->GetSafeHdc() );
    if( 0 == m_hRC )
    {
        ASSERT( !"wglCreateContext failed" );
        return false;
    }

    // レンダリングコンテキストの設定
    if( TRUE != wglMakeCurrent( m_pDC->GetSafeHdc(), m_hRC ) )
    {
        ASSERT( !"wglMakeCurrent failed" );
        return false;
    }

    // OpenGL拡張機能を使えるようにする
    if( !InitOpenGLExtFuncs() )
    {
        ASSERT( !"InitOpenGLExtFuncs failed" );
        return false;
    }

    // クリア処理
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);    // クリアカラー
    glClearDepth( 1.0f );    // クリア深度

    // デプス処理
    glEnable( GL_DEPTH_TEST );    // デプステスト
    glDepthFunc( GL_LEQUAL );    //デプスファンク（同じか、手前にあるもので上描いていく）

    // ポリゴン処理
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );    // ポリゴン（GL_TRIANGLES等）は、両面共塗りつぶす。
    glDisable( GL_CULL_FACE ); // 裏を向いている面のカリング（カリングしない＝裏面も描画）
    glEnable( GL_POLYGON_OFFSET_FILL ); // ポリゴンオフセットフィル（面と線が重なる場合のカスレ防止）
    glPolygonOffset( 1.0f, 1.0f ); // ポリゴンオフセット量

    // ライティング処理
    glDisable( GL_LIGHTING ); // 光源
    glEnable( GL_NORMALIZE ); // OpenGL側で法線ベクトルを単位法線ベクトル化するようにする
    glDisable( GL_COLOR_MATERIAL ); // カラー設定値をマテリアルとして使用
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return true;
}

bool COpenGLBaseView::SetupPixelFormat()
{
    static PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof( PIXELFORMATDESCRIPTOR ),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        24,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        32,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    int iPixelFormat;
    if( 0 == ( iPixelFormat = ChoosePixelFormat( m_pDC->GetSafeHdc(), &pfd ) ) )
    {
        ASSERT( !"ChoosePixelFormat is failed" );
        return FALSE;
    }

    if( TRUE != SetPixelFormat( m_pDC->GetSafeHdc(), iPixelFormat, &pfd ) )
    {
        ASSERT( !"SetPixelFormat is failed" );
        return FALSE;
    }

    return true;
}

void COpenGLBaseView::UninitializeOpenGL()
{
    wglMakeCurrent( NULL, NULL );
    if( m_hRC )
    {
        wglDeleteContext( m_hRC );
        m_hRC = NULL;
    }
    if( m_pDC )
    {
        delete m_pDC;
        m_pDC = NULL;
    }
}

bool COpenGLBaseView::SetupViewport()
{
    glViewport( 0,
        0,
        m_cx,
        m_cy );
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        CString str;
        str.Format(_T("ERROR:%i"), error);
        AfxMessageBox(str);
        return false;
    }
    return true;
}

bool COpenGLBaseView::SetupProjectionMatrix()
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho( -m_cx * 0.5 / 10.0,
        m_cx * 0.5 / 10.0,
        -m_cy * 0.5 / 10.0,
        m_cy * 0.5 / 10.0,
        0.1,
        2000.0 );

    // 投影変換行列に対する処理はここで完結。
    // 以降の変換行列処理は、モデルビュー変換行列に対して実施されるようにする。
    glMatrixMode( GL_MODELVIEW );
    return true;
}

bool COpenGLBaseView::SetupModelViewMatrix()
{
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    gluLookAt(eye[0], eye[1], eye[2],    // 視点
        center[0], center[1], center[2],    // 注視点
        0.0, 1.0, 0.0 );    // 上ベクトル

    return true;
}

void COpenGLBaseView::RenderStockScene()
{
    RenderAxis();
}

void COpenGLBaseView::RenderAxis()
{
    const double origin_length = 10;
    float    points[4][3];
    points[0][0] = 0.0;
    points[0][1] = 0.0;
    points[0][2] = 0.0;
    points[1][0] = origin_length;
    points[1][1] = 0.0;
    points[1][2] = 0.0;
    points[2][0] = 0.0;
    points[2][1] = origin_length;
    points[2][2] = 0.0;
    points[3][0] = 0.0;
    points[3][1] = 0.0;
    points[3][2] = origin_length;

    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );    // 色 | 線情報

    glLineWidth( 2.0 );
    glBegin( GL_LINES );

    glColor3ub( 255, 0, 0 );
    glVertex3fv( points[0] );
    glVertex3fv( points[1] );

    glColor3ub( 0, 255, 0 );
    glVertex3fv( points[0] );
    glVertex3fv( points[2] );

    glColor3ub( 0, 0, 255 );
    glVertex3fv( points[0] );
    glVertex3fv( points[3] );
    glEnd();

    glPopAttrib();
}

// COpenGLBaseView メッセージ ハンドラー


BOOL COpenGLBaseView::OnEraseBkgnd( CDC* pDC )
{
    // TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
    return TRUE;

    //return CView::OnEraseBkgnd( pDC );
}


void COpenGLBaseView::OnSize( UINT nType, int cx, int cy )
{
    CView::OnSize( nType, cx, cy );

    // TODO: ここにメッセージ ハンドラー コードを追加します。
    if( 0 >= cx || 0 >= cy )
    {
        return;
    }

    if( NULL == m_hRC )
    {
        // OpenGL初期化処理
        InitializeOpenGL();
    }

    if( NULL != m_hRC )
    {
        // 画面大きさの設定
        m_cx = cx;
        m_cy = cy;

        //m_cx = 1200;
        //m_cy = 400;

        // ビューポートの設定
        SetupViewport();

        // 投影変換行列の設定
        SetupProjectionMatrix();

        // モデリング＆ビューイング変換行列の設定
        SetupModelViewMatrix();
    }
}


void COpenGLBaseView::OnDestroy()
{
    CView::OnDestroy();

    // TODO: ここにメッセージ ハンドラー コードを追加します。
    UninitializeOpenGL();
}
