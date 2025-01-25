#include "pch.h"
#include "CDialogAvroProperty.h"

CDialogAvroProperty::CDialogAvroProperty() noexcept :
    CDialogEx(IDD_DIALOG_AVRO_PROPERTY)
{
    m_propertyEdit = "Null";
}

void CDialogAvroProperty::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_PROPERTY, m_propertyEdit);
}

BEGIN_MESSAGE_MAP(CDialogAvroProperty, CDialogEx)

END_MESSAGE_MAP()