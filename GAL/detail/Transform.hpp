#ifndef GAL_TRANSFORM_HPP
#define GAL_TRANSFORM_HPP

#include "Rotation.hpp"

namespace gal
{
	/// @brief Transform struct with model matrix caching.
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

		GAL_INLINE void setPosition(const glm::vec3& position) noexcept { this->position = position; }
		GAL_INLINE void setRotation(const Rotation& rotation) noexcept { this->rotation = rotation; }
		GAL_INLINE void setScale(const glm::vec3& scale) noexcept { this->scale = scale; }
 
		GAL_INLINE void applyTranslationGlobal(const glm::vec3& delta) noexcept { position += delta; }
		GAL_INLINE void applyRotationGlobal(const Rotation& rotation) noexcept { this->rotation.rotateGlobal(rotation); }
		GAL_INLINE void applyScale(const glm::vec3& scaleFactors) noexcept { scale *= scaleFactors; }

		GAL_INLINE void applyTranslationLocal(const glm::vec3& delta) noexcept { position += rotation.rotate(delta); }
		GAL_INLINE void applyRotationLocal(const Rotation& rotation) noexcept { this->rotation.rotateLocal(rotation); }

		GAL_NODISCARD GAL_INLINE glm::vec3 getPosition() const noexcept { return position; }
		GAL_NODISCARD GAL_INLINE Rotation getRotation() const noexcept { return rotation; }
		GAL_NODISCARD GAL_INLINE glm::vec3 getScale() const noexcept { return scale; }

		/// @brief Get the model matrix represented by this Transform.
		GAL_NODISCARD GAL_INLINE glm::mat4 getModelMatrix() const noexcept
		{
			return glm::translate(glm::mat4(1.0f), position)
				* rotation.asMat4()
				* glm::scale(glm::mat4(1.0f), scale);
		}
	};
}

#endif
