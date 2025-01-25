
// OpenGLPickView.h : COpenGLPickView �N���X�̃C���^�[�t�F�C�X
//

#pragma once

#include "OpenGLTrackView.h"

#include "OGLJText.h"

#define NAMEARRAYSIZE           (10)        // ���O��̑傫��
#define PICKREGIONOFFSET        (5)         // �s�b�N�̈�̃I�t�Z�b�g��
#define PICKREGIONPIXELCOUNT    ( 1 + 2 * PICKREGIONOFFSET ) * ( 1 + 2 * PICKREGIONOFFSET )    // �s�b�N�̈�̃s�N�Z����

// Render Mode
enum ERENDERMODE
{
    RM_UNDEFINED = 0,
    RM_RENDER,            // �ʏ�`��
    RM_PICK_ELEMENTTYPE,// �s�b�N�`��i�v�f�^�C�v�j
    RM_PICK_ELEMENTID,    // �s�b�N�`��i�v�f�ԍ��j
};

enum ERENDERELEMENTTYPE
{
    RET_UNDEFINED = 0,
    RET_POINT,
    RET_LINE,
    RET_FACE,
};

class COpenGLPickView : public COpenGLTrackView
{
    // �����o�ϐ�
private:
    GLuint            m_uiTextureID;
    GLuint            m_uiRenderBufferID;
    GLuint            m_uiFrameBufferID;
protected:
    unsigned int    m_auiName_picked[NAMEARRAYSIZE];    // �s�b�N���ꂽ�v�f�̖��O��
    COGLJText*      m_pOGLJText_x = nullptr;
    COGLJText*      m_pOGLJText_y = nullptr;
    COGLJText*      m_pOGLJText_z = nullptr;
    COGLJText*      m_pOGLJText_drawrate = nullptr;

    // �����o�֐�
private:
    void    CreateFrameBuffer( int iWidth, int iHeight );
    void    DestroyFrameBuffer();
    void    UninitializeOpenGL();
    bool    RenderScene();
    int     DoPicking( int x, int y );
protected:
    virtual void    RenderModel( ERENDERMODE eRenderMode );
    virtual void    IdentifyPickRegionPixels( unsigned int uiCountPickRegionPixel, unsigned int  aauiNamePickRegionPixel[][NAMEARRAYSIZE] );

protected: // �V���A��������̂ݍ쐬���܂��B
    COpenGLPickView();
    DECLARE_DYNCREATE(COpenGLPickView)

// ����
public:
    CDocument* GetDocument() const;

// ����
public:

// �I�[�o�[���C�h
public:
//    virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ����
public:
    virtual ~COpenGLPickView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnSize( UINT nType, int cx, int cy );
    afx_msg void OnRButtonUp( UINT nFlags, CPoint point );
};

#ifndef _DEBUG  // OpenGLPickView.cpp �̃f�o�b�O �o�[�W����
inline CDocument* COpenGLPickView::GetDocument() const
   { return reinterpret_cast<CDocument*>(m_pDocument); }
#endif

