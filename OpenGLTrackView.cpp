
// OpenGLTrackView.cpp : COpenGLTrackView クラスの実装
//

#include "pch.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
//#include "OpenGLTrack.h"
#endif

//#include "OpenGLTrackDoc.h"
#include "OpenGLTrackView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenGLTrackView

IMPLEMENT_DYNCREATE(COpenGLTrackView, COpenGLBaseView)

BEGIN_MESSAGE_MAP(COpenGLTrackView, COpenGLBaseView)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MBUTTONDOWN()
    ON_WM_MBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// COpenGLTrackView コンストラクション/デストラクション

COpenGLTrackView::COpenGLTrackView() :
    m_eTrackingMode( TM_NONE ),
    m_fRenderingRate( 10 )
{
    // TODO: 構築コードをここに追加します。
    m_f3RenderingCenter[0] = (GLfloat)center[0];
    m_f3RenderingCenter[1] = (GLfloat)center[1];
    m_f3RenderingCenter[2] = (GLfloat)center[2];
    m_f16ViewingTransformMatrix[0] = 1.0f; m_f16ViewingTransformMatrix[1] = 0.0f; m_f16ViewingTransformMatrix[2] = 0.0f; m_f16ViewingTransformMatrix[3] = 0.0f;
    m_f16ViewingTransformMatrix[4] = 0.0f; m_f16ViewingTransformMatrix[5] = 1.0f; m_f16ViewingTransformMatrix[6] = 0.0f; m_f16ViewingTransformMatrix[7] = 0.0f;
    m_f16ViewingTransformMatrix[8] = 0.0f; m_f16ViewingTransformMatrix[9] = 0.0f; m_f16ViewingTransformMatrix[10] = 1.0f; m_f16ViewingTransformMatrix[11] = 0.0f;
    m_f16ViewingTransformMatrix[12] = 0.0f; m_f16ViewingTransformMatrix[13] = 0.0f; m_f16ViewingTransformMatrix[14] = 0.0f; m_f16ViewingTransformMatrix[15] = 1.0f;
}

COpenGLTrackView::~COpenGLTrackView()
{
}

BOOL COpenGLTrackView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
    //  修正してください。

    return COpenGLBaseView::PreCreateWindow(cs);
}

// COpenGLTrackView 描画

//void COpenGLTrackView::OnDraw(CDC* /*pDC*/)
//{
//    CDocument* pDoc = GetDocument();
//    ASSERT_VALID(pDoc);
//    if (!pDoc)
//        return;
//
//    // TODO: この場所にネイティブ データ用の描画コードを追加します。
//}


// COpenGLTrackView 診断

#ifdef _DEBUG
void COpenGLTrackView::AssertValid() const
{
    COpenGLBaseView::AssertValid();
}

void COpenGLTrackView::Dump(CDumpContext& dc) const
{
    COpenGLBaseView::Dump(dc);
}

CDocument* COpenGLTrackView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDocument)));
    return (CDocument*)m_pDocument;
}
#endif //_DEBUG

void COpenGLTrackView::BeginTracking( CPoint point, ETRACKINGMODE eTrackingMode )
{
    SetCapture();
    m_eTrackingMode = eTrackingMode;
    m_ptLast = point;
}

void COpenGLTrackView::EndTracking()
{
    ReleaseCapture();
    m_eTrackingMode = TM_NONE;
}

void COpenGLTrackView::DoTracking( CPoint point )
{
    LONG lDeltaX = point.x - m_ptLast.x;
    LONG lDeltaY = point.y - m_ptLast.y;
    m_ptLast = point;
    switch( m_eTrackingMode )
    {
        case TM_NONE:
            break;
        case TM_ROTATION:
            {
                // クライアント領域の縦横の短い方の長さ分のピクセル動かすと半周（180度）回るように
                float fAngle_deg = (float)( sqrt( lDeltaX * lDeltaX + lDeltaY * lDeltaY ) * 180.0 / ( m_cx < m_cy ? m_cx : m_cy ) );
                // 回転マトリックスを現在のオブジェクトフォームマトリックスに掛け合わせる
                glPushMatrix();
                glLoadIdentity();
                glRotatef( fAngle_deg, (GLfloat)lDeltaY, (GLfloat)lDeltaX, 0 );
                glMultMatrixf( (GLfloat*)m_f16ViewingTransformMatrix );
                glGetFloatv( GL_MODELVIEW_MATRIX, (GLfloat*)m_f16ViewingTransformMatrix );
                glPopMatrix();
                // モデリング＆ビューイング変換行列の設定
                SetupModelViewMatrix();
                Invalidate( FALSE );
            }
            break;
        case TM_PAN:
            {
                m_f3RenderingCenter[0] -= lDeltaX / m_fRenderingRate;
                m_f3RenderingCenter[1] += lDeltaY / m_fRenderingRate;
                // モデリング＆ビューイング変換行列の設定
                SetupModelViewMatrix();
                Invalidate( FALSE );
            }
            break;
        case TM_ZOOM:
            {    // 拡大・縮小による描画倍率操作
                // +500で倍率2倍に、+250で倍率1.5倍に、
                // -500で倍率0.5倍に、-1000で倍率0.25倍に
                m_fRenderingRate *= (float)pow( 2, -lDeltaY * 0.002f );
                // 投影変換行列の設定
                SetupProjectionMatrix();
                Invalidate( FALSE );
            }
            break;
        default:
            ASSERT( !"未対応" );
            break;
    }
}

bool COpenGLTrackView::SetupProjectionMatrix()
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho( -m_cx * 0.5 / m_fRenderingRate,    // left
        m_cx * 0.5 / m_fRenderingRate,    // right
        -m_cy * 0.5 / m_fRenderingRate,    // buttom
        m_cy * 0.5 / m_fRenderingRate,    // top
        0.1,            // near
        2000.0 );            // far

    // 投影変換行列に対する処理はここで完結。
    // 以降の変換行列処理は、モデルビュー変換行列に対して実施されるようにする。
    glMatrixMode( GL_MODELVIEW );
    return true;
}

bool COpenGLTrackView::SetupModelViewMatrix()
{
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    gluLookAt( m_f3RenderingCenter[0], m_f3RenderingCenter[1], m_f3RenderingCenter[2] + 500.0,    // 視点
        m_f3RenderingCenter[0], m_f3RenderingCenter[1], m_f3RenderingCenter[2],            // 注視点
        0.0, 1.0, 0.0 );    // 上ベクトル

    glMultMatrixf( (GLfloat*)m_f16ViewingTransformMatrix );    // 表示回転（＝モデル回転）

    return true;
}

// COpenGLTrackView メッセージ ハンドラー


void COpenGLTrackView::OnLButtonDown( UINT nFlags, CPoint point )
{
    // TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
    BeginTracking( point, TM_ROTATION );

    COpenGLBaseView::OnLButtonDown( nFlags, point );
}

void COpenGLTrackView::OnLButtonUp( UINT nFlags, CPoint point )
{
    // TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
    EndTracking();

    COpenGLBaseView::OnLButtonUp( nFlags, point );
}

void COpenGLTrackView::OnMButtonDown( UINT nFlags, CPoint point )
{
    // TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
    BeginTracking( point, TM_PAN );

    COpenGLBaseView::OnMButtonDown( nFlags, point );
}

void COpenGLTrackView::OnMButtonUp( UINT nFlags, CPoint point )
{
    // TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
    EndTracking();

    COpenGLBaseView::OnMButtonUp( nFlags, point );
}

void COpenGLTrackView::OnMouseMove( UINT nFlags, CPoint point )
{
    // TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
    DoTracking( point );

    COpenGLBaseView::OnMouseMove( nFlags, point );
}

BOOL COpenGLTrackView::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
    // TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
    BeginTracking( pt, TM_ZOOM );
    CPoint    pt_delta( pt.x - zDelta / 3, pt.y - zDelta / 3 );
    DoTracking( pt_delta );
    EndTracking();

    return COpenGLBaseView::OnMouseWheel( nFlags, zDelta, pt );
}

