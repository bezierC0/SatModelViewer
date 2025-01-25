// CDialogShapeEdgeStraight.cpp : 実装ファイル
//

#include "pch.h"
#include "ModelViewer.h"
#include "CDialogShapeEdgeStraight.h"
#include "afxdialogex.h"


// CDialogShapeEdgeStraight ダイアログ

IMPLEMENT_DYNAMIC(CDialogShapeEdgeStraight, CDialogEx)

CDialogShapeEdgeStraight::CDialogShapeEdgeStraight(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_SHAPE_EDGE_STRAIGHT, pParent)
    , m_startX(0)
    , m_startY(0)
    , m_startZ(0)
    , m_endX(0)
    , m_endY(0)
    , m_endZ(0)
{

}

CDialogShapeEdgeStraight::~CDialogShapeEdgeStraight()
{

}

void CDialogShapeEdgeStraight::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_START_X, m_startX);
    DDX_Text(pDX, IDC_EDIT_START_Y, m_startY);
    DDX_Text(pDX, IDC_EDIT_START_Z, m_startZ);
    DDX_Text(pDX, IDC_EDIT_END_X, m_endX);
    DDX_Text(pDX, IDC_EDIT_END_Y, m_endY);
    DDX_Text(pDX, IDC_EDIT_END_Z, m_endZ);
}


BEGIN_MESSAGE_MAP(CDialogShapeEdgeStraight, CDialogEx)
END_MESSAGE_MAP()


// CDialogShapeEdgeStraight メッセージ ハンドラー
