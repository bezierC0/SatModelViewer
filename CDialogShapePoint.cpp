// CDialogShapePoint.cpp : 実装ファイル
//

#include "pch.h"
#include "ModelViewer.h"
#include "CDialogShapePoint.h"
#include "afxdialogex.h"


// CDialogShapePoint ダイアログ

IMPLEMENT_DYNAMIC(CDialogShapePoint, CDialogEx)

CDialogShapePoint::CDialogShapePoint(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_SHAPE_POINT, pParent)
    , m_pointX(0)
    , m_pointY(0)
    , m_pointZ(0)
{

}

CDialogShapePoint::~CDialogShapePoint()
{
}

void CDialogShapePoint::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_POINT_X, m_pointX);
    DDX_Text(pDX, IDC_EDIT_POINT_Y, m_pointY);
    DDX_Text(pDX, IDC_EDIT_POINT_Z, m_pointZ);
}


BEGIN_MESSAGE_MAP(CDialogShapePoint, CDialogEx)
END_MESSAGE_MAP()


// CDialogShapePoint メッセージ ハンドラー
