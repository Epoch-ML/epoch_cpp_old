#ifndef VERTEX_H_
#define VERTEX_H_

#include "core/ehm/result.h"

// epoch HAL vertex
// epoch/src/hal/vertex.h

// A vertex is a HAL object now - but might want to revisit 
// this, or explore ways to define custom vertex formats 

#include "core/math/point.h"
#include "core/math/color.h"

template <typename TValue, int dimension>
class vertex {
public:
	point<TValue, dimension> m_ptPosition;
	color m_cColor;
	point<float, 2> m_uvcoord;
	//vector<TValue, dimension> m_vNormal;
	//vector<TValue, dimension> m_vTangent;
	//vector<TValue, dimension> m_vBitangent;

public:
	vertex() :
		m_ptPosition(),
		m_cColor(1.0f, 1.0f, 1.0f, 1.0f),
		m_uvcoord(0.0f, 0.0f)
	{
		// 
	}



};

#endif // !VERTEX_H_