#pragma once
#include <afxdialogex.h>
#include <string>
#include "resource.h"
class CDialogAvro : public CDialogEx
{
public:
    CDialogAvro() noexcept;
    CString m_schemaEdit;
    CString m_avroEdit;
    BOOL m_isExportEdge;
    std::string m_schema;
    std::string m_avro;
    int m_importMode;

    // �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_OPEN_AVRO };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonSchema();
    afx_msg void OnBnClickedButtonAvro();
};
