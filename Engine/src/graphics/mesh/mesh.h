#pragma once

#include "core/resource/resource.h"
#include "math/vector/vector4.h"
#include "math/vector/vector3.h"
#include "math/matrix/mat4.h"
#include <vector>

//struct Vertex
//{
//	Vec3 p;
//	Vec3 n;
//	//Vec2 t;
//	//Color c;
//};

/**
* 
* 
* Index buffer layout (analogous to VAO in OpenGL):
* v1 n1 v2 n2 v3 n3 ...
* 6 indices correspond to a triangle
* 
*/
class Mesh : public Resource
{
public:
	Mesh(const std::string& path, bool isRHS = true);
	~Mesh() override;

	const std::vector<Vec3>& GetVertexBuffer() const;
	const std::vector<Vec3>& GetNormalBuffer() const;
	const std::vector<unsigned int>& GetIndexBuffer() const;

private:
	std::vector<Vec3> m_vertexBuffer;
	std::vector<Vec3> m_normalBuffer;
	std::vector<unsigned int> m_indexBuffer;

};