#pragma once


// CDialogEntityProperty ダイアログ

class CDialogEntityProperty : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogEntityProperty)

public:
    CDialogEntityProperty(CWnd* pParent = nullptr);   // 標準コンストラクター
    virtual ~CDialogEntityProperty();
    // メンバー変数
    CString m_propertyEdit;

    // ダイアログ データ
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_ENTITY_PROPERTY };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

    DECLARE_MESSAGE_MAP()
};
