#ifndef GAL_TRANSFORM_HPP
#define GAL_TRANSFORM_HPP

#include "Rotation.hpp"

namespace gal
{
	struct Transform
	{
		glm::vec3 position;
		Rotation rotation;
		glm::vec3 scale;

		/// @brief Initialize with given transformation. 
		GAL_INLINE Transform(const glm::vec3& position = glm::vec3(0.0f), const Rotation& rotation = Rotation(),
			const glm::vec3& scale = glm::vec3(1.0f))
			: position(position), rotation(rotation), scale(scale) { }

		/// @brief Set entire transform. 
		GAL_INLINE void set(const glm::vec3& position, const Rotation& rotation, const glm::vec3& scale) noexcept
		{
			this->position = position;
			this->rotation = rotation;
			this->scale = scale;
		}

		/// @brief Reset entire transform to default values. 
		GAL_INLINE void reset() noexcept
		{
			position = glm::vec3(0.0f);
			rotation = Rotation();
			scale = glm::vec3(1.0f);
		}
 
		GAL_INLINE void applyTranslation(const glm::vec3& delta) { position += delta; }
		GAL_INLINE void applyRotation(const Rotation& rotation) { this->rotation.followWith(rotation); }
		GAL_INLINE void applyScale(const glm::vec3& scaleFactors) { scale *= scaleFactors; }

		/// @brief Get the model matrix represented by this object. 
		GAL_NODISCARD GAL_INLINE glm::mat4 getModelMatrix() const noexcept
		{
			glm::mat4 model = glm::mat4(1.0f);

			model = glm::scale(model, scale);
			model = rotation.asMat4() * model;
			model = glm::translate(model, position);

			return model;
		}
	};
}

#endif
