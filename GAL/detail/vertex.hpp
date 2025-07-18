#ifndef GAL_VERTEX_HPP
#define GAL_VERTEX_HPP

namespace gal
{
	/// @brief Simple built-in vertex struct.
	/// 2-component position.
	struct VertexP2
	{
		float position[2];
	};

	/// @brief Simple built-in vertex struct.
	/// 2-component position.
	/// 3-component color.
	struct VertexP2C3
	{
		float position[2];
		float color[3];
	};

	/// @brief Simple built-in vertex struct.
	/// 2-component position.
	/// 4-component color.
	struct VertexP2C4
	{
		float position[2];
		float color[4];
	};

	/// @brief Simple built-in vertex struct.
	/// 2-component position.
	/// 2-component texture coordinates.
	struct VertexP2T2
	{
		float position[2];
		float texCoords[2];
	};

	/// @brief Simple built-in vertex struct.
	/// 2-component position.
	/// 3-component texture coordinates.
	struct VertexP2T3
	{
		float position[2];
		float texCoords[3];
	};

	/// @brief Simple built-in vertex struct.
	/// 3-component position.
	struct VertexP3
	{
		float position[3];
	};

	/// @brief Simple built-in vertex struct.
	/// 3-component position.
	/// 3-component color.
	struct VertexP3C3
	{
		float position[3];
		float color[3];
	};

	/// @brief Simple built-in vertex struct.
	/// 3-component position.
	/// 4-component color.
	struct VertexP3C4
	{
		float position[3];
		float color[4];
	};

	/// @brief Simple built-in vertex struct.
	/// 3-component position.
	/// 2-component texture coordinates.
	struct VertexP3T2
	{
		float position[3];
		float texCoords[2];
	};

	/// @brief Simple built-in vertex struct.
	/// 3-component position.
	/// 3-component texture coordinates.
	struct VertexP3T3
	{
		float position[3];
		float texCoords[3];
	};

	/// @brief Simple built-in vertex struct.
	/// 3-component position.
	/// 3-component normal.
	struct VertexP3N3
	{
		float position[3];
		float normal[3];
	};

	/// @brief Simple built-in vertex struct.
	/// 3-component position.
	/// 3-component normal.
	/// 3-component color.
	struct VertexP3N3C3
	{
		float position[3];
		float normal[3];
		float color[3];
	};

	/// @brief Simple built-in vertex struct.
	/// 3-component position.
	/// 3-component normal.
	/// 4-component color.
	struct VertexP3N3C4
	{
		float position[3];
		float normal[3];
		float color[4];
	};

	/// @brief Simple built-in vertex struct.
	/// 3-component position.
	/// 3-component normal.
	/// 2-component texture coordinates.
	struct VertexP3N3T2
	{
		float position[3];
		float normal[3];
		float texCoords[2];
	};
}

#endif