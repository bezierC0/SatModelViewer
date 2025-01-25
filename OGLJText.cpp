/*!	@file
    @brief	COGLJTextクラス実装
    @author	Nobuki HIRAMINE
    @date	2009/09/21 : 新規作成
    @note	なし*/
#include "pch.h"
#include "OGLJText.h"

#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

    //! 文字列バッファーのサイズ
const unsigned int BUFFERSIZE = 4096;

/*!	@brief	コンストラクタ
    @return	なし
    @date	2009/09/21 : 新規作成
    @note	なし*/
COGLJText::COGLJText(const TCHAR* pszFontName,	//!< [in] フォント名
    float fFontSize,			//!< [in] フォントサイズ
    EORIGINALPOS eOriginPos)	//!< [in] 基点位置
    : m_fFontSize(fFontSize),
    m_eOriginPos(eOriginPos),
    m_hFont(NULL),
    m_uiDisplayListIndex(0),
    m_pszString(NULL)
{
    if (NULL == pszFontName
        || 0 == _tcscmp(_T(""), pszFontName))
    {
        _tcsncpy_s(m_szFontName, LF_FACESIZE, _T("ＭＳ ゴシック"), _TRUNCATE);
    }
    else
    {
        _tcsncpy_s(m_szFontName, LF_FACESIZE, pszFontName, _TRUNCATE);
    }
}

/*!	@brief	デストラクタ
    @return	なし
    @date	2009/09/21 : 新規作成
    @note	なし*/
COGLJText::~COGLJText()
{
    DeleteDisplayList();
    if (m_pszString)
    {
        free(m_pszString);	// 描画文字列の削除
    }
    if (m_hFont)
    {
        DeleteObject(m_hFont);	// フォントオブジェクトの開放
    }
}

/*!	@brief	書式付きデータの書き込み
    @return	なし
    @date	2009/09/21 : 新規作成
    @note	なし*/
void COGLJText::Format(const TCHAR* format,	//!< [in] 書式指定文字列
    ...)
{
    va_list  valist;
    TCHAR buff[BUFFERSIZE];

    va_start(valist, format);
    _vsntprintf_s(buff, BUFFERSIZE, _TRUNCATE, format, valist);
    va_end(valist);

    if (NULL != m_pszString
        && 0 == _tcscmp(buff, m_pszString))
    {	// 文字列は変わってない
        return;
    }

    // 文字列は変わった
    if (NULL != m_pszString)
    {
        free(m_pszString);
    }
    m_pszString = _tcsdup(buff);
    DeleteDisplayList();
}

/*!	@brief	フォントオブジェクトの作成
    @return	なし
    @date	2009/09/21 : 新規作成
    @note	なし*/
static
HFONT CreateJFont(HDC hDC,					//!< [in] デバイスコンテキスト
    const TCHAR* pszFontName,	//!< [in] フォント名
    float fFontSize)			//!< [in] フォントサイズ
{
    if (NULL == pszFontName)
    {
        return NULL;
    }
    HFONT hFont = NULL;
    LOGFONT logfont;
    FillMemory(&logfont, sizeof(LOGFONT), 0);
    logfont.lfHeight = -MulDiv((int)fFontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);	// 文字の高さ
    logfont.lfCharSet = SHIFTJIS_CHARSET;	// キャラクタセット（日本語キャラクタセット）
    logfont.lfWeight = FW_NORMAL;		// 太さ（標準の太さ）
    _tcsncpy_s(logfont.lfFaceName, LF_FACESIZE, pszFontName, _TRUNCATE);
    hFont = CreateFontIndirect(&logfont);
    assert(hFont);
    return hFont;
}

/*!	@brief	BITMAPの初期化
    @return	なし
    @date	2009/09/21 : 新規作成
    @note	なし*/
static
void InitBitmap(BITMAP* pBitmap,	//!< [in/out] 初期化するビットマップ
    LONG lWidth,		//!< [in] ビットマップの幅 
    LONG lHeight)		//!< [in] ビットマップの高さ
{
    if (NULL == pBitmap)
    {
        return;
    }
    FillMemory(pBitmap, sizeof(*pBitmap), 0);
    pBitmap->bmWidth = lWidth;
    pBitmap->bmHeight = lHeight;
    pBitmap->bmWidthBytes = ((lWidth + 7) / 8 + 1) & (~1);
    pBitmap->bmPlanes = 1;
    pBitmap->bmBitsPixel = 1;
    pBitmap->bmBits = malloc(pBitmap->bmWidthBytes * lHeight);
}

/*!	@brief	BITMAPの後処理
    @return	なし
    @date	2009/09/21 : 新規作成
    @note	なし*/
static
void UninitBitmap(BITMAP* pBitmap)
{
    if (NULL == pBitmap)
    {
        return;
    }
    if (pBitmap->bmBits)
    {
        free(pBitmap->bmBits);
    }
}

/*!	@brief	白黒ビット列の作成
    @return	作成されたビットマップのポインタ
    @date	2009/09/21 : 新規作成
    @note	なし*/
static
GLubyte* CreateMonoBits(HDC hDC,			//!< [in] デバイスコンテキスト
    HBITMAP hBitmap,	//!< [in] ビットマップハンドル
    SIZE& rsizeBitmap)	//!< [out] ビットマップの大きさ
{
    // インプットのチェック
    if (NULL == hDC
        || NULL == hBitmap)
    {
        return NULL;
    }

    // ビットマップの情報の取得
    BITMAP bitmap;
    GetObject(hBitmap, sizeof(bitmap), &bitmap);
    rsizeBitmap.cx = ((bitmap.bmWidth + 31) & (~31));	// 32の倍数にする（理由は調査未）
    rsizeBitmap.cy = bitmap.bmHeight;

    // ビット列の領域の確保
    // 1ピクセルあたり1bitの領域を確保する。
    // 1byteは8bitsであり、cy * cx の結果を8で割る必要がある。
    // cxは上の計算の結果であり8の倍数であることが保証されているので、cxを8で割る。
    int uiBitsSize = rsizeBitmap.cy * (rsizeBitmap.cx / 8);
    GLubyte* pubMonoBits = (GLubyte*)malloc(uiBitsSize);
    FillMemory(pubMonoBits, uiBitsSize, 0);

    // ビットマップをビット列にコピー
    struct
    {
        BITMAPINFOHEADER    bmiHeader;
        RGBQUAD             bmiColors[2];	// パレット（色の数(＝2^biBitCount)分のRGBQUADを用意する必要がある）
    } monobitmapinfo;
    BITMAPINFO* pBitmapinfo = (BITMAPINFO*)&monobitmapinfo;

    pBitmapinfo->bmiHeader.biSize = sizeof(pBitmapinfo->bmiHeader);
    pBitmapinfo->bmiHeader.biWidth = bitmap.bmWidth;
    pBitmapinfo->bmiHeader.biHeight = bitmap.bmHeight;
    pBitmapinfo->bmiHeader.biPlanes = 1;
    pBitmapinfo->bmiHeader.biBitCount = 1;	// 1ピクセルあたりのビット数（1ピクセルあたりの1ビット＝２色ビットマップ）
    pBitmapinfo->bmiHeader.biCompression = BI_RGB;
    pBitmapinfo->bmiHeader.biSizeImage = uiBitsSize;
    pBitmapinfo->bmiHeader.biXPelsPerMeter = 1;
    pBitmapinfo->bmiHeader.biYPelsPerMeter = 1;
    pBitmapinfo->bmiHeader.biClrUsed = 0;
    pBitmapinfo->bmiHeader.biClrImportant = 0;

    GetDIBits(hDC, hBitmap, 0, bitmap.bmHeight, pubMonoBits, pBitmapinfo, DIB_RGB_COLORS);

    return pubMonoBits;
}

/*!	@brief	文字列白黒ビット列の作成
    @return	作成されたビットマップのポインタ
    @date	2009/09/21 : 新規作成
    @note	なし*/
static
GLubyte* CreateStringMonoBits(HDC hDC,				//!< [in] デバイスコンテキスト
    HFONT hFont,			//!< [in] フォントハンドル
    const TCHAR* pszString,	//!< [in] 文字列
    SIZE& rsizeBitmap)		//!< [out] ビットマップの大きさ
{
    // アウトプットの宣言、初期化
    GLubyte* pubStringMonoBits = NULL;

    // インプットのチェック
    if (NULL == hDC
        || NULL == hFont
        || NULL == pszString)
    {
        assert(!"CreateStringMonoBits");
        return NULL;
    }

    // 文字列の文字数
    unsigned int uiStringLength = (unsigned int)_tcslen(pszString);

    HFONT   hFont_old = (HFONT)SelectObject(hDC, hFont);

    // テキストの幅と高さを計算します。
    SIZE sizeText;
    GetTextExtentPoint32(hDC, pszString, uiStringLength, &sizeText);

    //指定された幅、高さ、色形式を持つビットマップの作成
    BITMAP	bitmap;
    InitBitmap(&bitmap, sizeText.cx, sizeText.cy);	// BITMAPの初期化
    HBITMAP hBitmap = CreateBitmapIndirect(&bitmap);
    assert(hBitmap);
    UninitBitmap(&bitmap);	// BITMAPの後処理

    if (hBitmap)
    {
        // 指定されたデバイスと互換性のあるメモリデバイスコンテキストの作成
        HDC hMemDC = CreateCompatibleDC(hDC);
        assert(hMemDC);
        if (hMemDC)
        {
            // MemDCのビットマップ、フォント、テキスト色、背景色、背景モードの指定
            HBITMAP hBitmap_memdc_old = (HBITMAP)SelectObject(hMemDC, hBitmap);
            HFONT hFont_memdc_old = (HFONT)SelectObject(hMemDC, hFont);
            SetTextColor(hMemDC, RGB(255, 255, 255));
            SetBkColor(hMemDC, RGB(0, 0, 0));
            SetBkMode(hMemDC, OPAQUE);

            // MemDC上にテキストを出力
            TextOut(hMemDC, 0, 0, pszString, uiStringLength);

#ifdef	_DEBUG
#if 0
            if (OpenClipboard(NULL))
            {
                EmptyClipboard();
                SetClipboardData(CF_BITMAP, hBitmap);
                CloseClipboard();
            }
#endif
#endif

            // MonoBitsデータの作成、構築
            pubStringMonoBits = CreateMonoBits(hMemDC, hBitmap, rsizeBitmap);

            SelectObject(hMemDC, hFont_memdc_old);
            SelectObject(hMemDC, hBitmap_memdc_old);

            DeleteDC(hMemDC);
        }
        DeleteObject(hBitmap);
    }
    SelectObject(hDC, hFont_old);

    return pubStringMonoBits;
}

/*!	@brief	改行を含む文字列を改行で分割し、改行を含まない文字列の配列に変換
    @return	なし
    @date	2009/09/21 : 新規作成
    @note	なし*/
static
bool Split(const TCHAR* pszString_source,	//!< [in] 分割する文字列
    TCHAR*& rpszString_dest,		//!< [out] 分割する文字列の\n,\rが\0に置き換わった文字列
    unsigned int& ruiCountLine,		//!< [out] 行数
    TCHAR**& rapszLineString)		//!< [out] 一行文字列の先頭文字列のアドレスの配列
{
    if (NULL == pszString_source)
    {
        return false;
    }

    rpszString_dest = _tcsdup(pszString_source);

    // 行数を求める。
    ruiCountLine = 0;
    TCHAR* pszString_work = rpszString_dest;
    while (1)
    {
        TCHAR* pszString_strchr = _tcschr(pszString_work, _T('\n'));
        if (NULL == pszString_strchr)
        {	// もう「\n」はない
            ++ruiCountLine;
            break;
        }
        else
        {
            ++ruiCountLine;
            pszString_work = &(pszString_strchr[1]);	// ワーク文字列の開始文字を、見つけた「\n」の次の文字にする。
            if (_T('\0') == pszString_work[0])
            {	// 文字列終了
                break;
            }
        }
    }
    assert(ruiCountLine);

    // 一行文字列の先頭文字列のアドレスの配列の領域の確保
    rapszLineString = (TCHAR**)malloc(ruiCountLine * sizeof(char*));
    assert(rapszLineString);

    // 文字列の解析（行数分析）
    unsigned int uiIndexLine = 0;
    pszString_work = rpszString_dest;
    while (1)
    {
        TCHAR* pszString_strchr = _tcschr(pszString_work, _T('\n'));
        if (NULL == pszString_strchr)
        {	// もう「\n」はない
            rapszLineString[uiIndexLine] = pszString_work;
            ++uiIndexLine;
            break;
        }
        else
        {
            pszString_strchr[0] = _T('\0');	// 見つけた「\n」を「\0」に置換。
            if (_T('\r') == pszString_work[_tcslen(pszString_work) - 1])
            {	// \r\n対応
                pszString_work[_tcslen(pszString_work) - 1] = _T('\0');
            }
            rapszLineString[uiIndexLine] = pszString_work;
            ++uiIndexLine;
            pszString_work = &(pszString_strchr[1]);	// ワーク文字列の開始文字を、見つけた「\n」の次の文字にする。
            if (_T('\0') == pszString_work[0])
            {	// 文字列終了
                break;
            }
        }
    }
    assert(uiIndexLine == ruiCountLine);

    return true;
}

/*!	@brief	描画
    @return	なし
    @date	2009/09/21 : 新規作成
    @note	なし*/
void COGLJText::Render()
{
    if (0 != m_uiDisplayListIndex)
    {	// ディスプレイリスト化済みならば、ディスプレイリスト呼び出して終了。
        glCallList(m_uiDisplayListIndex);
        return;
    }

    if (NULL == m_pszString)
    {	// 描画文字列なし
        return;
    }

    // デバイスコンテキストの取得
    HDC hDC = wglGetCurrentDC();
    assert(hDC);

    // フォントオブジェクトの作成
    if (NULL == m_hFont)
    {
        m_hFont = CreateJFont(hDC, m_szFontName, m_fFontSize);
        assert(m_hFont);
    }

    // 文字列を改行で分割
    TCHAR* pszString_split = NULL;
    unsigned int uiCountLine = 0;
    TCHAR** apszLineString = NULL;	// 一行文字列の先頭文字列のアドレスの配列
    if (!Split(m_pszString,
        pszString_split,
        uiCountLine,
        apszLineString))
    {	// 失敗
        return;
    }
    assert(pszString_split);
    assert(uiCountLine);
    assert(apszLineString);

    // ディスプレイリスト化と描画
    m_uiDisplayListIndex = glGenLists(1);
    glNewList(m_uiDisplayListIndex, GL_COMPILE_AND_EXECUTE);
    for (unsigned int uiIndexLine = 0; uiIndexLine < uiCountLine; ++uiIndexLine)
    {
        if (0 == _tcscmp(_T(""), apszLineString[uiIndexLine]))
        {	// 空行
            continue;	// コンティニューすると空行ができる。
        }
        // 文字列白黒ビット列の作成
        SIZE sizeBitmap;
        GLubyte* pubBits = CreateStringMonoBits(hDC, m_hFont, apszLineString[uiIndexLine], sizeBitmap);
        if (NULL == pubBits)
        {	// ビットマップ作成失敗したときは、空行にする。
            assert(!"CreateStringMonoBits()");
            continue;	// コンティニューすると空行ができる。
        }
        GLfloat	xmove = 0.0;
        GLfloat	ymove = 0.0;
        switch (m_eOriginPos)
        {
        case OP_LEFTTOP:	// 文字列原点を、左上とする場合
            ymove = (GLfloat)((int)(uiIndexLine + 1) * sizeBitmap.cy);
            break;
        case OP_RIGHTTOP:	// 文字列原点を、右上とする場合
            xmove = (GLfloat)sizeBitmap.cx;
            ymove = (GLfloat)((int)(uiIndexLine + 1) * sizeBitmap.cy);
            break;
        case OP_LEFTBOTTOM:	// 文字列原点を、左下とする場合
            ymove = (GLfloat)((int)(uiIndexLine + 1 - uiCountLine) * sizeBitmap.cy);
            break;
        case OP_RIGHTBOTTOM:// 文字列原点を、右下とする場合
            xmove = (GLfloat)sizeBitmap.cx;
            ymove = (GLfloat)((int)(uiIndexLine + 1 - uiCountLine) * sizeBitmap.cy);
            break;
        default:
            assert(!"未対応");
            ymove = (GLfloat)((uiIndexLine + 1) * sizeBitmap.cy);
            break;
        }
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glBitmap(sizeBitmap.cx,	// width
            sizeBitmap.cy,	// height
            xmove,		// ビットマップ原点をどこにするか（ビットマップの左下基準。軸は、Ｘ軸は右向き、Ｙ軸は上向き）
            ymove,		// ビットマップ原点をどこにするか（ビットマップの左下基準。軸は、Ｘ軸は右向き、Ｙ軸は上向き）
            0.0,
            0.0,
            pubBits);
        free(pubBits);
    }
    glEndList();

    free(apszLineString);
    free(pszString_split);
}

/*!	@brief	ディスプレイリストの削除
    @return	なし
    @date	2009/09/21 : 新規作成
    @note	なし*/
void COGLJText::DeleteDisplayList()
{
    if (m_uiDisplayListIndex)
    {
        glDeleteLists(m_uiDisplayListIndex, 1);
        m_uiDisplayListIndex = 0;
    }
}

