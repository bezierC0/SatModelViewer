// CDialogEntityProperty.cpp : 実装ファイル
//

#include "pch.h"
#include "ModelViewer.h"
#include "CDialogEntityProperty.h"
#include "afxdialogex.h"


// CDialogEntityProperty ダイアログ

IMPLEMENT_DYNAMIC(CDialogEntityProperty, CDialogEx)

CDialogEntityProperty::CDialogEntityProperty(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_ENTITY_PROPERTY, pParent)
{

}

CDialogEntityProperty::~CDialogEntityProperty()
{
}

void CDialogEntityProperty::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_PROPERTY, m_propertyEdit);
}


BEGIN_MESSAGE_MAP(CDialogEntityProperty, CDialogEx)
END_MESSAGE_MAP()


// CDialogEntityProperty メッセージ ハンドラー
