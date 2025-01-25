#pragma once


// CDialogShapeFacePlane ダイアログ

class CDialogShapeFacePlane : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogShapeFacePlane)

public:
    CDialogShapeFacePlane(CWnd* pParent = nullptr);   // 標準コンストラクター
    virtual ~CDialogShapeFacePlane();

    // ダイアログ データ
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_SHAPE_FACE_PLANE };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

    DECLARE_MESSAGE_MAP()
public:
    CString m_originX;
    CString m_originY;
    CString m_originZ;
    CString m_normalX;
    CString m_normalY;
    CString m_normalZ;
    CString m_width;
    CString m_height;
};
