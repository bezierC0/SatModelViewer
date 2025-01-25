
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CViewTreeEntity ウィンドウ

class CViewTreeEntity : public CTreeCtrl
{
    // コンストラクション
public:
    CViewTreeEntity() noexcept;

    // オーバーライド
protected:
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

    // 実装
public:
    virtual ~CViewTreeEntity();

protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnTvnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
};
