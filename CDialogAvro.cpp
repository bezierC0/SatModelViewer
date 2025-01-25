#include "pch.h"

#include "CDialogAvro.h"

CDialogAvro::CDialogAvro() noexcept :
    CDialogEx(IDD_DIALOG_OPEN_AVRO)
    , m_importMode(0)
{
    m_schema = "C:\\work\\FeatureRecognition\\bin\\RelWithDebInfo\\RecognizedWeldingResultRaw.json";
    m_schemaEdit = m_schema.c_str();
}

void CDialogAvro::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SCHEMA, m_schemaEdit);
    DDX_Text(pDX, IDC_EDIT_AVRO, m_avroEdit);
    DDX_Check(pDX, IDC_CHECK_EXPORT_EDGE, m_isExportEdge);
    DDX_Radio(pDX, IDC_RADIO_IMPORT_MODE, m_importMode);
}

BEGIN_MESSAGE_MAP(CDialogAvro, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_SCHEMA, &CDialogAvro::OnBnClickedButtonSchema)
    ON_BN_CLICKED(IDC_BUTTON_AVRO, &CDialogAvro::OnBnClickedButtonAvro)
END_MESSAGE_MAP()

void CDialogAvro::OnBnClickedButtonSchema()
{
    CFileDialog file_open_dialog(TRUE);
    file_open_dialog.m_ofn.lpstrFilter = _T("JSON File (*.json)\0*.json\0")
        _T("All Files (*.*)\0*.*\0\0");
    file_open_dialog.m_ofn.nFilterIndex = 1;
    file_open_dialog.m_ofn.Flags |= OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    INT_PTR iResponse = file_open_dialog.DoModal();
    if (iResponse == IDOK)
    {
        char* sat_file_name = nullptr;
        size_t length = wcslen(file_open_dialog.GetPathName().GetBuffer()) + 1;
        sat_file_name = (LPSTR)malloc(length * sizeof(WCHAR));
        WideCharToMultiByte(CP_ACP,    // CODE PAGE: ANSI code page
            0,
            file_open_dialog.GetPathName().GetBuffer(),
            -1,
            sat_file_name,
            (int)length * sizeof(WCHAR),
            NULL,
            NULL);
        if (sat_file_name)
        {
            m_schema = sat_file_name;
            m_schemaEdit = sat_file_name;
            UpdateData(FALSE);
        }
    }
}


void CDialogAvro::OnBnClickedButtonAvro()
{
    CFileDialog file_open_dialog(TRUE);
    file_open_dialog.m_ofn.lpstrFilter = _T("AVRO File (*.avro)\0*.avro\0")
        _T("All Files (*.*)\0*.*\0\0");
    file_open_dialog.m_ofn.nFilterIndex = 1;
    file_open_dialog.m_ofn.Flags |= OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    INT_PTR iResponse = file_open_dialog.DoModal();
    if (iResponse == IDOK)
    {
        char* sat_file_name = nullptr;
        size_t length = wcslen(file_open_dialog.GetPathName().GetBuffer()) + 1;
        sat_file_name = (LPSTR)malloc(length * sizeof(WCHAR));
        WideCharToMultiByte(CP_ACP,    // CODE PAGE: ANSI code page
            0,
            file_open_dialog.GetPathName().GetBuffer(),
            -1,
            sat_file_name,
            (int)length * sizeof(WCHAR),
            NULL,
            NULL);
        if (sat_file_name)
        {
            m_avro = sat_file_name;
            m_avroEdit = sat_file_name;
            UpdateData(FALSE);
        }
    }
}
