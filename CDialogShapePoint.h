#pragma once


// CDialogShapePoint ダイアログ

class CDialogShapePoint : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogShapePoint)

public:
    CDialogShapePoint(CWnd* pParent = nullptr);   // 標準コンストラクター
    virtual ~CDialogShapePoint();

    // ダイアログ データ
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_SHAPE_POINT };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

    DECLARE_MESSAGE_MAP()
public:
    double m_pointX;
    double m_pointY;
    double m_pointZ;
};
