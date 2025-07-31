#ifndef GAL_MESH_INSTANCE_HPP
#define GAL_MESH_INSTANCE_HPP

#include "Transform.hpp"
#include "VertexArray.hpp"

namespace gal
{
	/// @brief Holds a reference to a VAO and transform data about a single instance of a mesh.
	struct MeshInstance
	{
	public:
		const VertexArray& vao;
		Transform transform;

		/// @brief Initialize a mesh instance with the default transform. 
		GAL_INLINE MeshInstance(const VertexArray& vao)
			: vao(vao), transform(Transform()) { }

		/// @brief Initialize a mesh instance with a given transform.
		GAL_INLINE MeshInstance(const VertexArray& vao, const Transform& transform)
			: vao(vao), transform(transform) { }
	};
}

#endif
