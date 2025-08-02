#ifndef GAL_MESH_INSTANCE_HPP
#define GAL_MESH_INSTANCE_HPP

#include "ShaderProgram.hpp"
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
			: vao(vao), transform() { }

		/// @brief Initialize a mesh instance with a given transform.
		GAL_INLINE MeshInstance(const VertexArray& vao, const Transform& transform)
			: vao(vao), transform(transform) { }

		/// @brief Set the model matrix in the given shader and then bind and draw the VAO.
		GAL_INLINE void drawAB(const ShaderProgram& shader, const std::string& modelMatrixUniformName) const
		{
			shader.setUniform(modelMatrixUniformName, transform.getModelMatrix());
			vao.drawAB();
		}

		/// @brief Set the model matrix in the given shader and then draw the VAO.
		GAL_INLINE void drawNB(const ShaderProgram& shader, const std::string& modelMatrixUniformName) const
		{
			shader.setUniform(modelMatrixUniformName, transform.getModelMatrix());
			vao.drawNB();
		}
	};
}

#endif
