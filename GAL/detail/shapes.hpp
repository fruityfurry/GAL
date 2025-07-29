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
			std::vector<VertexP3>* positions,
			std::vector<VertexN3>* normals = nullptr,
			std::vector<VertexT2>* texCoords = nullptr
		)
		{

			const size_t vertexCount = 6 * (static_cast<size_t>(subdivisions) + 2) * (static_cast<size_t>(subdivisions) + 2);

			positions->reserve(vertexCount);

			if (normals != nullptr)
				normals->reserve(vertexCount);

			if (texCoords != nullptr)
				texCoords->reserve(vertexCount);

			const float step = 2.0f / (subdivisions + 1);
			const float texStep = step / 2.0f;

			for (float u = -1.0f; u < 1.0f; u += step)
				for (float v = -1.0f; v < 1.0f; v += step)
				{
					const float texU = (u + 1.0f) / 2.0f;
					const float texV = (v + 1.0f) / 2.0f;

					// Vertices are added in BL, BR, TL - BR, TL, TR order.

					if (texCoords != nullptr)
						for (int n = 0; n < 6; ++n)  // Tex coords are identical for all six faces.
						{
							texCoords->push_back({ texU,           texV });
							texCoords->push_back({ texU + texStep, texV });
							texCoords->push_back({ texU,           texV + texStep });

							texCoords->push_back({ texU + texStep, texV });
							texCoords->push_back({ texU,           texV + texStep });
							texCoords->push_back({ texU + texStep, texV + texStep });
						}

					// X faces.
					positions->push_back({ -1.0f, u,        v });
					positions->push_back({ -1.0f, u + step, v });
					positions->push_back({ -1.0f, u,        v + step });

					positions->push_back({ -1.0f, u + step, v });
					positions->push_back({ -1.0f, u,        v + step });
					positions->push_back({ -1.0f, u + step, v + step });

					positions->push_back({ 1.0f, u,        v });
					positions->push_back({ 1.0f, u + step, v });
					positions->push_back({ 1.0f, u,        v + step });

					positions->push_back({ 1.0f, u + step, v });
					positions->push_back({ 1.0f, u,        v + step });
					positions->push_back({ 1.0f, u + step, v + step });

					// Y faces.
					positions->push_back({ u,        -1.0f, v });
					positions->push_back({ u + step, -1.0f, v });
					positions->push_back({ u,        -1.0f, v + step });

					positions->push_back({ u + step, -1.0f, v });
					positions->push_back({ u,        -1.0f, v + step });
					positions->push_back({ u + step, -1.0f, v + step });

					positions->push_back({ u,        1.0f, v });
					positions->push_back({ u + step, 1.0f, v });
					positions->push_back({ u,        1.0f, v + step });

					positions->push_back({ u + step, 1.0f, v });
					positions->push_back({ u,        1.0f, v + step });
					positions->push_back({ u + step, 1.0f, v + step });

					// Z faces.
					positions->push_back({ u,        v,        -1.0f });
					positions->push_back({ u + step, v,        -1.0f });
					positions->push_back({ u,        v + step, -1.0f });

					positions->push_back({ u + step, v,        -1.0f });
					positions->push_back({ u,        v + step, -1.0f });
					positions->push_back({ u + step, v + step, -1.0f });

					positions->push_back({ u,        v,        1.0f });
					positions->push_back({ u + step, v,        1.0f });
					positions->push_back({ u,        v + step, 1.0f });

					positions->push_back({ u + step, v,        1.0f });
					positions->push_back({ u,        v + step, 1.0f });
					positions->push_back({ u + step, v + step, 1.0f });

					if (normals != nullptr)
					{
						normals->push_back({ -1.0f,  0.0f,  0.0f });
						normals->push_back({ -1.0f,  0.0f,  0.0f });
						normals->push_back({ -1.0f,  0.0f,  0.0f });
						normals->push_back({ -1.0f,  0.0f,  0.0f });
						normals->push_back({ -1.0f,  0.0f,  0.0f });
						normals->push_back({ -1.0f,  0.0f,  0.0f });

						normals->push_back({ 1.0f,  0.0f,  0.0f });
						normals->push_back({ 1.0f,  0.0f,  0.0f });
						normals->push_back({ 1.0f,  0.0f,  0.0f });
						normals->push_back({ 1.0f,  0.0f,  0.0f });
						normals->push_back({ 1.0f,  0.0f,  0.0f });
						normals->push_back({ 1.0f,  0.0f,  0.0f });

						normals->push_back({ 0.0f, -1.0f,  0.0f });
						normals->push_back({ 0.0f, -1.0f,  0.0f });
						normals->push_back({ 0.0f, -1.0f,  0.0f });
						normals->push_back({ 0.0f, -1.0f,  0.0f });
						normals->push_back({ 0.0f, -1.0f,  0.0f });
						normals->push_back({ 0.0f, -1.0f,  0.0f });

						normals->push_back({ 0.0f,  1.0f,  0.0f });
						normals->push_back({ 0.0f,  1.0f,  0.0f });
						normals->push_back({ 0.0f,  1.0f,  0.0f });
						normals->push_back({ 0.0f,  1.0f,  0.0f });
						normals->push_back({ 0.0f,  1.0f,  0.0f });
						normals->push_back({ 0.0f,  1.0f,  0.0f });

						normals->push_back({ 0.0f,  0.0f, -1.0f });
						normals->push_back({ 0.0f,  0.0f, -1.0f });
						normals->push_back({ 0.0f,  0.0f, -1.0f });
						normals->push_back({ 0.0f,  0.0f, -1.0f });
						normals->push_back({ 0.0f,  0.0f, -1.0f });
						normals->push_back({ 0.0f,  0.0f, -1.0f });

						normals->push_back({ 0.0f,  0.0f,  1.0f });
						normals->push_back({ 0.0f,  0.0f,  1.0f });
						normals->push_back({ 0.0f,  0.0f,  1.0f });
						normals->push_back({ 0.0f,  0.0f,  1.0f });
						normals->push_back({ 0.0f,  0.0f,  1.0f });
						normals->push_back({ 0.0f,  0.0f,  1.0f });
					}
				}

			//deduplicateVertices();


		}
	}
}

#endif
