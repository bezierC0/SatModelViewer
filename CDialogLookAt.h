#pragma once
#include <afxwin.h>
#include "resource.h"
class CDialogLookAt : public CDialogEx
{
public:
    CDialogLookAt() noexcept;
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
    DECLARE_MESSAGE_MAP()

};
