#pragma once
#include <afxdialogex.h>
#include <string>
#include "resource.h"
class CDialogAvroProperty : public CDialogEx
{
public:
    CDialogAvroProperty() noexcept;

    // �����o�[�ϐ�
    CString m_propertyEdit;

    // �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_AVRO_PROPERTY };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
    DECLARE_MESSAGE_MAP()

};

