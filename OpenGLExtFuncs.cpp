//
// OpenGLExtFuncs.cpp
//
#include "pch.h"

#define OPENGLEXTFUNCS_C
#include "OpenGLExtFuncs.h"

bool InitOpenGLExtFuncs()
{
    // Framebuffer
    glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress( "glBindFramebuffer" );
    if( NULL == glBindFramebuffer ) { return false; }

    glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress( "glCheckFramebufferStatus" );
    if( NULL == glCheckFramebufferStatus ) { return false; }

    glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress( "glDeleteFramebuffers" );
    if( NULL == glDeleteFramebuffers ) { return false; }

    glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress( "glFramebufferTexture2D" );
    if( NULL == glFramebufferTexture2D ) { return false; }

    glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress( "glFramebufferRenderbuffer" );
    if( NULL == glFramebufferRenderbuffer ) { return false; }

    glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress( "glGenFramebuffers" );
    if( NULL == glGenFramebuffers ) { return false; }

    // Renderbuffer
    glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress( "glBindRenderbuffer" );
    if( NULL == glBindRenderbuffer ) { return false; }

    glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress( "glDeleteRenderbuffers" );
    if( NULL == glDeleteRenderbuffers ) { return false; }

    glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress( "glGenRenderbuffers" );
    if( NULL == glGenRenderbuffers ) { return false; }

    glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress( "glRenderbufferStorage" );
    if( NULL == glRenderbufferStorage ) { return false; }

    // Buffer
    glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
    if( NULL == glGenBuffers ) { return false; }

    glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
    if( NULL == glBindBuffer ) { return false; }

    glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
    if( NULL == glBufferData ) { return false; }

    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
    if( NULL == glDeleteBuffers ) { return false; }

    return true;
}
