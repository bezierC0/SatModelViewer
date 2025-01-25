#pragma once
#include <afxcmn.h>
class CViewTreeJson : public CTreeCtrl
{
    // コンストラクション
public:
    CViewTreeJson() noexcept;

    // オーバーライド
protected:
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

    // 実装
public:
    virtual ~CViewTreeJson();

protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnTvnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
};



