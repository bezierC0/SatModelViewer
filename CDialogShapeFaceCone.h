#pragma once


// CDialogShapeFaceCone ダイアログ

class CDialogShapeFaceCone : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogShapeFaceCone)

public:
	CDialogShapeFaceCone(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CDialogShapeFaceCone();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SHAPE_FACE_CONE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
