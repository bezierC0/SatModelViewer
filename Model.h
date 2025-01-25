//
// Model.h
//
#pragma once

#include <vector>

class CVector
{
public:
    float    x, y, z;
public:
    CVector( float a = 0.0, float b = 0.0, float c = 0.0 ) : x(a), y(b), z(c) {}
};

class CIndexedTriangle
{
public:
    unsigned int m_ui3IndexVertex[3];            // 頂点番号
public:
    CIndexedTriangle( unsigned int a = 0, unsigned int b = 0, unsigned int c = 0 )
    {
        m_ui3IndexVertex[0] = a;
        m_ui3IndexVertex[1] = b;
        m_ui3IndexVertex[2] = c;
    }
};

class CModel
{
public:
    std::vector<CVector>                m_vVertex;                // 頂点配列
    std::vector<CIndexedTriangle>    m_vIndexedTriangle;        // 三角形配列
};
