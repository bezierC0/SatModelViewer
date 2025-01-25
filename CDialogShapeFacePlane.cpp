// CDialogShapeFacePlane.cpp : 実装ファイル
//

#include "pch.h"
#include "ModelViewer.h"
#include "CDialogShapeFacePlane.h"
#include "afxdialogex.h"


// CDialogShapeFacePlane ダイアログ

IMPLEMENT_DYNAMIC(CDialogShapeFacePlane, CDialogEx)

CDialogShapeFacePlane::CDialogShapeFacePlane(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_SHAPE_FACE_PLANE, pParent)
    , m_originX(_T(""))
    , m_originY(_T(""))
    , m_originZ(_T(""))
    , m_normalX(_T(""))
    , m_normalY(_T(""))
    , m_normalZ(_T(""))
    , m_width(_T(""))
{
    m_originX = "-100";
    m_originY = "-100";
    m_originZ = "5";
    m_normalX = "0";
    m_normalY = "0";
    m_normalZ = "1";
    m_width = "200";
    m_height = "200";
}

CDialogShapeFacePlane::~CDialogShapeFacePlane()
{

}

void CDialogShapeFacePlane::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_ORIGIN_X, m_originX);
    DDX_Text(pDX, IDC_EDIT_ORIGIN_Y, m_originY);
    DDX_Text(pDX, IDC_EDIT_ORIGIN_Z, m_originZ);
    DDX_Text(pDX, IDC_EDIT_NORMAL_X, m_normalX);
    DDX_Text(pDX, IDC_EDIT_NORMAL_Y, m_normalY);
    DDX_Text(pDX, IDC_EDIT_NORMAL_Z, m_normalZ);
    DDX_Text(pDX, IDC_EDIT_WIDTH, m_width);
    DDX_Text(pDX, IDC_EDIT_HEIGHT, m_height);
    
}


BEGIN_MESSAGE_MAP(CDialogShapeFacePlane, CDialogEx)
END_MESSAGE_MAP()


// CDialogShapeFacePlane メッセージ ハンドラー
