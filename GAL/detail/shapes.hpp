#ifndef GAL_SHAPES_HPP
#define GAL_SHAPES_HPP

#include "vertex.hpp"

namespace gal
{
	namespace detail
	{
		GAL_INLINE void deduplicateVertices()
		{

		}
	}

	namespace shapes
	{
		GAL_INLINE void generateCubeVertices(
			int subdivisions,
			std::vector<VertexP3>* positionsPtr,
			std::vector<VertexN3>* normalsPtr = nullptr,
			std::vector<VertexT2>* texCoordsPtr = nullptr
		)
		{
			std::vector<VertexP3>& positions = *positionsPtr;
			std::vector<VertexN3>& normals = *normalsPtr;
			std::vector<VertexT2>& texCoords = *texCoordsPtr;

			const size_t vertexCount = 6 * (static_cast<size_t>(subdivisions) + 2) * (static_cast<size_t>(subdivisions) + 2);

			positions.reserve(vertexCount);
			normals.reserve(vertexCount);
			texCoords.reserve(vertexCount);

			for (float u = -1.0f; u <= 1.0f; u += 2.0f / (subdivisions + 1))
				for (float v = -1.0f; v <= 1.0f; v += 2.0f / (subdivisions + 1))
				{
					const float texU = (u + 1.0f) / 2.0f;
					const float texV = (v + 1.0f) / 2.0f;

					texCoords.push_back({ texU, texV });

					positions.push_back({ -1.0f,     u,     v });
					positions.push_back({  1.0f,     u,     v });
					positions.push_back({     u, -1.0f,     v });
					positions.push_back({     u,  1.0f,     v });
					positions.push_back({     u,     v, -1.0f });
					positions.push_back({     u,     v,  1.0f });

					normals.push_back({ -1.0f,  0.0f,  0.0f });
					normals.push_back({  1.0f,  0.0f,  0.0f });
					normals.push_back({  0.0f, -1.0f,  0.0f });
					normals.push_back({  0.0f,  1.0f,  0.0f });
					normals.push_back({  0.0f,  0.0f, -1.0f });
					normals.push_back({  0.0f,  0.0f,  1.0f });
				}

			//deduplicateVertices();


		}
	}
}

#endif
