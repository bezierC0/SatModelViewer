#pragma once
#include <afxdialogex.h>
#include <string>
#include "resource.h"
class CDialogAvroProperty : public CDialogEx
{
public:
    CDialogAvroProperty() noexcept;

    // メンバー変数
    CString m_propertyEdit;

    // ダイアログ データ
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_AVRO_PROPERTY };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
    DECLARE_MESSAGE_MAP()

};

