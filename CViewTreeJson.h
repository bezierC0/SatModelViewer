#pragma once
#include <afxcmn.h>
class CViewTreeJson : public CTreeCtrl
{
    // �R���X�g���N�V����
public:
    CViewTreeJson() noexcept;

    // �I�[�o�[���C�h
protected:
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

    // ����
public:
    virtual ~CViewTreeJson();

protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnTvnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
};



