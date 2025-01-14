#include "pch.h"
#include "mesh.h"

#include "core/debug/assert.h"
#include <fstream>
#include <sstream>

void Mesh::Load(const char* path)
{
	// Load OBJ
	std::ifstream file(path);
	if (!file.is_open())
	{
		Logger::Error("Could not find path for mesh @ %s", path);
		return;
	}

	while (!file.eof())
	{
		char line[128];
		file.getline(line, 128);

		std::stringstream stream;
		stream << line;

		/* .obj files are formatted as:
		* '#' for comments at the start
		* 'v x y z' for vertices. x, y, z are floats
		* 'vn x y z' for normals, similarly to above
		* 'f a/b/c e/f/g h/i/j' faces. aeh are vertex idxs, bfi are
		* texture coord idxs (unused), and cgj are normal idxs
		*/

		char junk;
		if (line[0] == 'v' && line[1] == ' ')
		{
			// Parse vertex position
			Vec3 v;
			stream >> junk >> v.x >> v.y >> v.z;
			m_vertexBuffer.push_back(v);
		}
		else if (line[0] == 'v' && line[1] == 'n')
		{
			// Parse vertex normal
			Vec3 v;
			stream >> junk >> junk >> v.x >> v.y >> v.z;
			m_normalBuffer.push_back(v);
		}
		else if (line[0] == 'f' && line[1] == ' ')
		{
			// Parse face indices
			// The token will be 'a/b/c' since they are space-separated
			std::string token;
			stream >> junk;
			while (stream >> token)
			{
				// Grab the first num before the '/'
				size_t pos1 = token.find('/');
				unsigned int vIdx = std::stoi(token.substr(0, pos1));

				// Grab the third num after the 2nd '/'
				std::string remainder = token.substr(pos1 + 1, std::string::npos);
				size_t pos2 = remainder.find('/');
				unsigned int nIdx = std::stoi(remainder.substr(pos2 + 1, std::string::npos));

				// Continue to next group, -1 b/c idxs start at 1 in obj
				m_indexBuffer.push_back(vIdx - 1);
				m_indexBuffer.push_back(nIdx - 1);
			}
		}
		else
		{
			stream >> junk;
		}
	}

	// Per triangle, there should be 3 verts, 3 norms
	// Therefore index buffer size should be multiple of 6
	ASSERT_ERROR(m_indexBuffer.size() % 6 == 0, "Mesh %s loaded wrong, index buffer size = %d", path, m_indexBuffer.size());

	//for (auto i : m_indexBuffer)
	//	std::cout << i << " ";
}

void Mesh::Unload()
{

}

const std::vector<Vec3>& Mesh::GetVertexBuffer() const
{
	return m_vertexBuffer;
}

const std::vector<Vec3>& Mesh::GetNormalBuffer() const
{
	return m_normalBuffer;
}

const std::vector<unsigned int>& Mesh::GetIndexBuffer() const
{
	return m_indexBuffer;
}
