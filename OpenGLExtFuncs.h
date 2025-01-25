//
// OpenGLExtFuncs.h
//
#pragma once

#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glext.h>

#ifdef OPENGLEXTFUNCS_C    // OpenGLExtFuncs.cからIncludeされるときは、変数定義（実体を定義する）
#    define EXTERN
#else                    // OpenGLExtFuncs.c以外からIncludeされるときは、変数宣言（実体を定義しない）
#    define EXTERN    extern
#endif

// OpenGL拡張機能関数

// Framebuffer
EXTERN    PFNGLBINDFRAMEBUFFERPROC            glBindFramebuffer;
EXTERN    PFNGLCHECKFRAMEBUFFERSTATUSPROC        glCheckFramebufferStatus;
EXTERN    PFNGLDELETEFRAMEBUFFERSPROC            glDeleteFramebuffers;
EXTERN    PFNGLFRAMEBUFFERTEXTURE2DPROC        glFramebufferTexture2D;
EXTERN    PFNGLFRAMEBUFFERRENDERBUFFERPROC    glFramebufferRenderbuffer;
EXTERN    PFNGLGENFRAMEBUFFERSPROC            glGenFramebuffers;

// Renderbuffer
EXTERN    PFNGLBINDRENDERBUFFERPROC            glBindRenderbuffer;
EXTERN    PFNGLDELETERENDERBUFFERSPROC        glDeleteRenderbuffers;
EXTERN    PFNGLGENRENDERBUFFERSPROC            glGenRenderbuffers;
EXTERN    PFNGLRENDERBUFFERSTORAGEPROC        glRenderbufferStorage;

// VertexBuffer
EXTERN    PFNGLGENBUFFERSPROC                    glGenBuffers;
EXTERN    PFNGLBINDBUFFERPROC                    glBindBuffer;
EXTERN    PFNGLBUFFERDATAPROC                    glBufferData;
EXTERN    PFNGLDELETEBUFFERSPROC                glDeleteBuffers;

// 初期化処理（関数ポインタと関数の紐づけ）
bool InitOpenGLExtFuncs();
