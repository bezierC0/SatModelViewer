//
// LoadStlFile.cpp
//
#include "pch.h"
#include "kernapi.hxx"
#include "Model.h"

#define    READSTRINGBUFFERLEN    256

bool LoadStlFile( const char* pszFileName, CModel& model )
{
    if( NULL == pszFileName )
    {
        return false;
    }

    FILE* pFILE;
    if( 0 != fopen_s( &pFILE, pszFileName, "rt" ) ) 
    {
        return false;
    }

    // �v�f�̐��𐔂���i�O�p�`�̐��j
    unsigned int uiCountTriangle = 0;
    while( 1 )
    {
        char    szReadString[READSTRINGBUFFERLEN];
        if( NULL == fgets( szReadString, READSTRINGBUFFERLEN, pFILE ) )
        {
            break;
        }
        const char     cszDelimiter[] = ", \r\n\t";
        char* pszContext;
        char* pszToken = strtok_s( szReadString, cszDelimiter, &pszContext );
        if( NULL == pszToken )
        {
            continue;
        }
        if( 0 == _stricmp( pszToken, "facet" ) )
        {
            uiCountTriangle++;
            continue;
        }
    }
    rewind( pFILE );

    if( 0 == uiCountTriangle )
    {
        fclose( pFILE );
        return false;
    }

    model.m_vVertex.assign( uiCountTriangle * 3, CVector() );
    model.m_vIndexedTriangle.assign( uiCountTriangle, CIndexedTriangle() );

    unsigned int ui3 = 0;
    unsigned int uiIndexTriangle = 0;
    unsigned int uiIndexVertex = 0;

    while( 1 )
    {
        char szReadString[READSTRINGBUFFERLEN];

        if( NULL == fgets( szReadString, READSTRINGBUFFERLEN, pFILE ) )
        {
            break;
        }
        const char     cszDelimiter[] = ", \r\n\t";
        char* pszContext;
        char* pszToken = strtok_s( szReadString, cszDelimiter, &pszContext );
        if( NULL == pszToken )
        {
            continue;
        }

        if(      0 == _stricmp( pszToken, "vertex" ) )
        {
            if( 3 <= ui3 )
            {
                continue;
            }
            pszToken = strtok_s( NULL, cszDelimiter, &pszContext );
            model.m_vVertex[uiIndexVertex].x = (float)atof( pszToken );
            pszToken = strtok_s( NULL, cszDelimiter, &pszContext );
            model.m_vVertex[uiIndexVertex].y = (float)atof( pszToken );
            pszToken = strtok_s( NULL, cszDelimiter, &pszContext );
            model.m_vVertex[uiIndexVertex].z = (float)atof( pszToken );
            model.m_vIndexedTriangle[uiIndexTriangle].m_ui3IndexVertex[ui3] = uiIndexVertex;
            uiIndexVertex++;
            ui3++;
            continue;
        }
        else if( 0 == _stricmp( pszToken, "facet" ) )
        {    // �ʖ@���x�N�g��
            ui3 = 0;
            continue;
        }
        else if( 0 == _stricmp( pszToken, "endfacet" ) )
        {
            uiIndexTriangle++;
            continue;
        }
        else if( 0 == _stricmp( pszToken, "solid" ) )
        {    //    �\���b�h��
            continue;
        }
    }

    fclose( pFILE );

    if( 0 == model.m_vVertex.size()
     || 0 == model.m_vIndexedTriangle.size() )
    {
        return false;
    }

    return true;
}