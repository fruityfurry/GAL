#ifndef GAL_SHAPES_HPP
#define GAL_SHAPES_HPP

#include <array>
#include <unordered_map>
#include <utility>

namespace std
{
	template<std::size_t N>
	struct hash<std::array<float, N>>
	{
		GAL_INLINE std::size_t operator()(const std::array<float, N>& arr) const
		{
			std::size_t seed = 0;

			for (float val : arr)
			{
				int intCastVal;
				memcpy(&intCastVal, &val, sizeof(float));
				std::size_t hashVal = std::hash<int>{}(intCastVal);
				seed ^= hashVal + 0x9e3779b9 + (seed << 6) + (seed >> 2);  // Apparently the Boost libraries do this.
			}

			return seed;
		}
	};
}

namespace gal
{
	namespace detail
	{
		GAL_INLINE std::vector<float> interleaveVertexAttributes(
			const std::vector<std::array<float, 3>>* positions,
			const std::vector<std::array<float, 3>>* normals,
			const std::vector<std::array<float, 2>>* texCoords
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

		template<int floatsPerVertex>
		GAL_INLINE std::pair<std::vector<float>, std::vector<unsigned int>> deduplicateVertices(const std::vector<float>& vertexData)
		{
			if (vertexData.size() % floatsPerVertex != 0)
				throw std::runtime_error("Incorrect number of vertex data points given the value of floatsPerVertex given. "
					"This is an internal problem with GAL, not your fault!");

			std::vector<float> vertexDataOut;
			std::vector<unsigned int> indicesOut;
			std::unordered_map<std::array<float, floatsPerVertex>, unsigned int> vertexMap;

			unsigned int index = 0;

			for (unsigned int i = 0; i < vertexData.size(); i += floatsPerVertex)
			{
				std::array<float, floatsPerVertex> vertex;

				for (unsigned int j = 0; j < floatsPerVertex; ++j)
					vertex[j] = std::round(vertexData[i + j] * 1e5f) / 1e5f;

				if (auto it = vertexMap.find(vertex); it != vertexMap.end())
					indicesOut.push_back(it->second);
				else
				{
					vertexMap[vertex] = index;
					indicesOut.push_back(index++);

					for (float val : vertex)
						vertexDataOut.push_back(val);
				}
			}

			return { vertexDataOut, indicesOut };
		}
	}

	namespace shapes
	{
		/// @brief Returns a pair of two vectors, the first one containing vertex data and the second containing indices.
		/// Attributes will be in the order position, [normal, texCoords]. At least positions are always generated.
		/// Subdivisions controls how many times the cube is subdivided, where a subdivision is in the sense of a
		/// single halving slice across each axis. So, one subdivision would turn a regular cube into 4 regular cubes
		/// stacked together.
		GAL_INLINE std::pair<std::vector<float>, std::vector<unsigned int>> generateCubeVertices(
			int subdivisions, bool generateNormals = false, bool generateTexCoords = false)
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

			std::vector<float> duplicatedVertices = detail::interleaveVertexAttributes(positionsPtr, normalsPtr, texCoordsPtr);

			size_t floatsPerVertex = 3;

			if (generateNormals)
				floatsPerVertex += 3;
			if (generateTexCoords)
				floatsPerVertex += 2;

			switch (floatsPerVertex)
			{
				case 3:
					return detail::deduplicateVertices<3>(duplicatedVertices);
				case 5:
					return detail::deduplicateVertices<5>(duplicatedVertices);
				case 6:
					return detail::deduplicateVertices<6>(duplicatedVertices);
				case 8:
					return detail::deduplicateVertices<8>(duplicatedVertices);

				default:
					throw std::runtime_error("Unsupported vertex format when deduplicating vertices. "
						"This is an internal problem with GAL, not your fault!");
			}
		}
	}
}

#endif
