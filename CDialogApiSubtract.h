#pragma once


// CDialogApiSubtract ダイアログ

class CDialogApiSubtract : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogApiSubtract)

public:
    CDialogApiSubtract(CWnd* pParent = nullptr);   // 標準コンストラクター
    virtual ~CDialogApiSubtract();

    // ダイアログ データ
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_API_SUBTRACT };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

    DECLARE_MESSAGE_MAP()
public:
    CString m_targetEdit;
    CString m_faceEdit;
    afx_msg void OnBnClickedTargetPickButton();
    afx_msg void OnBnClickedFacePickButton();
};
