#ifndef GAL_SHAPES_HPP
#define GAL_SHAPES_HPP

#include <array>

namespace gal
{
	namespace detail
	{
		/*template<size_t... sizes>
		GAL_INLINE std::vector<float> interleaveVertexAttributes(const std::vector<std::array<float, sizes>>&... attributes)
		{
			constexpr size_t attributeCount = sizeof...(sizes);
			const size_t vertexCount = std::get<0>(std::tie(attributes...));
			const size_t floatsPerVertex = (sizes + ...);

			std::vector<float> result;
			result.reserve(vertexCount * floatsPerVertex);

			for (std::size_t i = 0; i < vertexCount; ++i)
			{
				([&]
				{
					const auto& attribute = attributes[i];
					result.insert(result.end(), attribute.begin(), attribute.end());
				}(), ...);
			}

			return result;
		}*/

		GAL_INLINE std::vector<float> interleaveVertexAttributes(
			std::vector<std::array<float, 3>>* positions,
			std::vector<std::array<float, 3>>* normals,
			std::vector<std::array<float, 2>>* texCoords
		)
		{
			const size_t vertexCount = positions->size();
			size_t floatsPerVertex = 3;

			if (normals != nullptr)
				floatsPerVertex += 3;
			if (texCoords != nullptr)
				floatsPerVertex += 2;

			std::vector<float> result;
			result.reserve(vertexCount * floatsPerVertex);

			for (size_t i = 0; i < vertexCount; ++i)
			{
				result.insert(result.end(), (*positions)[i].begin(), (*positions)[i].end());

				if (normals != nullptr)
					result.insert(result.end(), (*normals)[i].begin(), (*normals)[i].end());
				if (texCoords != nullptr)
					result.insert(result.end(), (*texCoords)[i].begin(), (*texCoords)[i].end());
			}

			return result;
		}

		GAL_INLINE void deduplicateVertices()
		{

		}
	}

	namespace shapes
	{
		/// @brief Returns a vector of floats, which are the components of the chosen vertex attributes for a cube.
		/// Attributes will be in the order position, [normal, texCoords]. At least positions are always generated.
		/// Subdivisions controls how many times the cube is subdivided, where a subdivision is in the sense of a
		/// single halving slice across each axis. So, one subdivision would turn a regular cube into 4 regular cubes
		/// stacked together.
		GAL_INLINE std::vector<float> generateCubeVertices(int subdivisions, bool generateNormals = false, bool generateTexCoords = false)
		{
			std::vector<std::array<float, 3>> positions;
			std::vector<std::array<float, 3>> normals;
			std::vector<std::array<float, 2>> texCoords;

			const size_t vertexCount = 6 * (static_cast<size_t>(subdivisions) + 2) * (static_cast<size_t>(subdivisions) + 2);

			positions.reserve(vertexCount);

			if (generateNormals)
				normals.reserve(vertexCount);

			if (generateTexCoords)
				texCoords.reserve(vertexCount);

			const float step = 2.0f / (subdivisions + 1);
			const float texStep = step / 2.0f;

			for (float u = -1.0f; u < 1.0f; u += step)
				for (float v = -1.0f; v < 1.0f; v += step)
				{
					const float texU = (u + 1.0f) / 2.0f;
					const float texV = (v + 1.0f) / 2.0f;

					// Vertices are added in BL, BR, TL - BR, TR, TL order.

					if (generateTexCoords)
						for (int n = 0; n < 6; ++n)  // Tex coords are identical for all six faces.
						{
							texCoords.push_back({ texU,           texV });
							texCoords.push_back({ texU + texStep, texV });
							texCoords.push_back({ texU,           texV + texStep });

							texCoords.push_back({ texU + texStep, texV });
							texCoords.push_back({ texU + texStep, texV + texStep });
							texCoords.push_back({ texU,           texV + texStep });
						}

					// X faces.
					positions.push_back({ -1.0f, u,        v });
					positions.push_back({ -1.0f, u + step, v });
					positions.push_back({ -1.0f, u,        v + step });

					positions.push_back({ -1.0f, u + step, v });
					positions.push_back({ -1.0f, u + step, v + step });
					positions.push_back({ -1.0f, u,        v + step });

					positions.push_back({ 1.0f, u,        v });
					positions.push_back({ 1.0f, u + step, v });
					positions.push_back({ 1.0f, u,        v + step });

					positions.push_back({ 1.0f, u + step, v });
					positions.push_back({ 1.0f, u + step, v + step });
					positions.push_back({ 1.0f, u,        v + step });

					// Y faces.
					positions.push_back({ u,        -1.0f, v });
					positions.push_back({ u + step, -1.0f, v });
					positions.push_back({ u,        -1.0f, v + step });

					positions.push_back({ u + step, -1.0f, v });
					positions.push_back({ u + step, -1.0f, v + step });
					positions.push_back({ u,        -1.0f, v + step });

					positions.push_back({ u,        1.0f, v });
					positions.push_back({ u + step, 1.0f, v });
					positions.push_back({ u,        1.0f, v + step });

					positions.push_back({ u + step, 1.0f, v });
					positions.push_back({ u + step, 1.0f, v + step });
					positions.push_back({ u,        1.0f, v + step });

					// Z faces.
					positions.push_back({ u,        v,        -1.0f });
					positions.push_back({ u + step, v,        -1.0f });
					positions.push_back({ u,        v + step, -1.0f });

					positions.push_back({ u + step, v,        -1.0f });
					positions.push_back({ u + step, v + step, -1.0f });
					positions.push_back({ u,        v + step, -1.0f });

					positions.push_back({ u,        v,        1.0f });
					positions.push_back({ u + step, v,        1.0f });
					positions.push_back({ u,        v + step, 1.0f });

					positions.push_back({ u + step, v,        1.0f });
					positions.push_back({ u + step, v + step, 1.0f });
					positions.push_back({ u,        v + step, 1.0f });

					if (generateNormals)
					{
						auto appendNormals = [&](const std::array<float, 3>& normal)
							{
								for (int i = 0; i < 6; ++i)
									normals.push_back(normal);
							};

						appendNormals({ -1.0f,  0.0f,  0.0f });  // -X
						appendNormals({  1.0f,  0.0f,  0.0f });  // +X
						appendNormals({  0.0f, -1.0f,  0.0f });  // -Y
						appendNormals({  0.0f,  1.0f,  0.0f });  // +Y
						appendNormals({  0.0f,  0.0f, -1.0f });  // -Z
						appendNormals({  0.0f,  0.0f,  1.0f });  // +Z
					}
				}

			std::vector<std::array<float, 3>>* positionsPtr = &positions;
			std::vector<std::array<float, 3>>* normalsPtr = generateNormals ? &normals : nullptr;
			std::vector<std::array<float, 2>>* texCoordsPtr = generateTexCoords ? &texCoords : nullptr;

			return detail::interleaveVertexAttributes(positionsPtr, normalsPtr, texCoordsPtr);
		}
	}
}

#endif
