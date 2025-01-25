/*!	@file
    @brief	COGLJTextクラスヘッダ
    @author	Nobuki HIRAMINE
    @date	2009/09/21 : 新規作成
    @note	なし*/
#pragma once

#include <gl\gl.h>

    /*!	@brief	OpenGLで日本語テキストを描画するためのクラス
        @date	2009/09/21 : 新規作成
        @note	なし*/
class COGLJText
{
public:
    //! 基点位置
    enum EORIGINALPOS
    {
        OP_LEFTTOP = 0,	//!< 左上
        OP_RIGHTTOP,	//!< 右上
        OP_LEFTBOTTOM,	//!< 左下
        OP_RIGHTBOTTOM,	//!< 右下
    };
    // コンストラクタ、デストラクタ
public:
    COGLJText(const TCHAR* pszFontName, float fFontSize, EORIGINALPOS eOriginPos = OP_LEFTBOTTOM);
    virtual	~COGLJText();

    // メンバ変数
public:
protected:
private:
    TCHAR			m_szFontName[LF_FACESIZE];	//!< フォント名（LOGFONTのlfFaceNameの仕様により、文字列の長さはヌル文字含めてLF_FACESIZE(=32)）
    float			m_fFontSize;				//!< フォントサイズ
    EORIGINALPOS	m_eOriginPos;				//!< 基点位置
    HFONT			m_hFont;					//!< フォントハンドル
    GLuint			m_uiDisplayListIndex;		//!< ディスプレイリストインデックス
    TCHAR* m_pszString;				//!< 描画文字列
// メンバ関数
public:
    //↓↓↓アクセサ↓↓↓
    float	GetFontSize() const { return m_fFontSize; }	//!< フォントサイズの取得
    void	SetFontSize(float fFontSize)
    {
        if (m_fFontSize == fFontSize) { return; }
        DeleteDisplayList();
        if (m_hFont) { DeleteObject(m_hFont); m_hFont = NULL; }
        m_fFontSize = fFontSize;
    }	//!< フォントサイズの設定
    const TCHAR* GetFontName() const { return m_szFontName; }	//!< フォント名の取得
    void	SetFontName(const TCHAR* pszFontName)
    {
        if (NULL != pszFontName && 0 == _tcscmp(m_szFontName, pszFontName)) { return; }
        DeleteDisplayList();
        if (m_hFont) { DeleteObject(m_hFont); m_hFont = NULL; }
        if (NULL == pszFontName
            || 0 == _tcscmp(_T(""), pszFontName))
        {
            _tcsncpy_s(m_szFontName, LF_FACESIZE, _T("ＭＳ ゴシック"), _TRUNCATE);
        }
        else
        {
            _tcsncpy_s(m_szFontName, LF_FACESIZE, pszFontName, _TRUNCATE);
        }
    }	//!< フォント名の設定
    EORIGINALPOS	GetOriginPos() const { return m_eOriginPos; }	//!< 基点位置の取得
    void	SetOriginPos(const EORIGINALPOS eOriginPos) { m_eOriginPos = eOriginPos; }	//!< 基点位置の設定
    //↑↑↑アクセサ↑↑↑
public:
    void		Format(const TCHAR* fmt, ...);
    void		Render();
protected:
private:
    void		DeleteDisplayList();
};
