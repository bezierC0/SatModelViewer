// CDialogApiSubtract.cpp : 実装ファイル
//

#include "pch.h"
#include "ModelViewer.h"
#include "CDialogApiSubtract.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ModelViewerView.h"
#include <body.hxx>
#include <kernapi.hxx>
#include <cstrapi.hxx>

// CDialogApiSubtract ダイアログ

IMPLEMENT_DYNAMIC(CDialogApiSubtract, CDialogEx)

CDialogApiSubtract::CDialogApiSubtract(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_API_SUBTRACT, pParent)
    , m_targetEdit(_T(""))
    , m_faceEdit(_T(""))
{

}

CDialogApiSubtract::~CDialogApiSubtract()
{
}

void CDialogApiSubtract::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_TARGET_PICK, m_targetEdit);
    DDX_Text(pDX, IDC_EDIT_FACE_PICK, m_faceEdit);
}


BEGIN_MESSAGE_MAP(CDialogApiSubtract, CDialogEx)
    ON_BN_CLICKED(IDC_TARGET_PICK_BUTTON, &CDialogApiSubtract::OnBnClickedTargetPickButton)
    ON_BN_CLICKED(IDC_FACE_PICK_BUTTON, &CDialogApiSubtract::OnBnClickedFacePickButton)
END_MESSAGE_MAP()


// CDialogApiSubtract メッセージ ハンドラー


void CDialogApiSubtract::OnBnClickedTargetPickButton()
{
    CMainFrame* p_frame = (CMainFrame*)AfxGetMainWnd();
    if (!p_frame)
    {
        return;
    }
    HTREEITEM selected_item = p_frame->m_wndEntityView.m_wndEntityView.GetSelectedItem();
    ENTITY* selectItem = (ENTITY*)p_frame->m_wndEntityView.m_wndEntityView.GetItemData(selected_item);
    if (!selectItem)
    {
        return;
    }
    std::string uid = find_attribute(selectItem, "uid");
    if (uid.empty())
    {
        return;
    }
    m_targetEdit = uid.c_str();
    UpdateData(FALSE);
}


void CDialogApiSubtract::OnBnClickedFacePickButton()
{
    CMainFrame* p_frame = (CMainFrame*)AfxGetMainWnd();
    if (!p_frame)
    {
        return;
    }
    HTREEITEM selected_item = p_frame->m_wndEntityView.m_wndEntityView.GetSelectedItem();
    ENTITY* selectItem = (ENTITY*)p_frame->m_wndEntityView.m_wndEntityView.GetItemData(selected_item);
    if (!selectItem)
    {
        return;
    }
    std::string uid = find_attribute(selectItem, "uid");
    if (uid.empty())
    {
        return;
    }
    m_faceEdit = uid.c_str();
    UpdateData(FALSE);
}
