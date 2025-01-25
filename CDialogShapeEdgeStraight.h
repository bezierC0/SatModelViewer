#pragma once


// CDialogShapeEdgeStraight ダイアログ

class CDialogShapeEdgeStraight : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogShapeEdgeStraight)

public:
    CDialogShapeEdgeStraight(CWnd* pParent = nullptr);   // 標準コンストラクター
    virtual ~CDialogShapeEdgeStraight();

    // ダイアログ データ
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_SHAPE_EDGE_STRAIGHT };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

    DECLARE_MESSAGE_MAP()
public:
    double m_startX;
    double m_startY;
    double m_startZ;
    double m_endX;
    double m_endY;
    double m_endZ;
};
