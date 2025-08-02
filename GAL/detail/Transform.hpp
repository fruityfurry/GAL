#ifndef GAL_TRANSFORM_HPP
#define GAL_TRANSFORM_HPP

#include "Rotation.hpp"

namespace gal
{
	/// @brief Transform struct with model matrix caching.
	struct Transform
	{
		/// @brief Initialize with given transformation. 
		GAL_INLINE Transform(const glm::vec3& position = glm::vec3(0.0f), const Rotation& rotation = Rotation(),
			const glm::vec3& scale = glm::vec3(1.0f))
			: position(position), rotation(rotation), scale(scale)
		{
			cachedModelMatrix = calculateModelMatrix();
		}

		/// @brief Set entire transform. 
		GAL_INLINE void set(const glm::vec3& position, const Rotation& rotation, const glm::vec3& scale) noexcept
		{
			this->position = position;
			this->rotation = rotation;
			this->scale = scale;

			dirty = true;
		}

		/// @brief Reset entire transform to default values. 
		GAL_INLINE void reset() noexcept
		{
			position = glm::vec3(0.0f);
			rotation = Rotation();
			scale = glm::vec3(1.0f);

			dirty = true;
		}

		GAL_INLINE void setPosition(const glm::vec3& position) noexcept { this->position = position; dirty = true; }
		GAL_INLINE void setRotation(const Rotation& rotation) noexcept { this->rotation = rotation; dirty = true; }
		GAL_INLINE void setScale(const glm::vec3& scale) noexcept { this->scale = scale; dirty = true; }
 
		GAL_INLINE void applyTranslation(const glm::vec3& delta) noexcept { position += delta; dirty = true; }
		GAL_INLINE void applyRotation(const Rotation& rotation) noexcept { this->rotation.rotateLocal(rotation); dirty = true; }
		GAL_INLINE void applyScale(const glm::vec3& scaleFactors) noexcept { scale *= scaleFactors; dirty = true; }

		GAL_NODISCARD GAL_INLINE glm::vec3 getPosition() const noexcept { return position; }
		GAL_NODISCARD GAL_INLINE Rotation getRotation() const noexcept { return rotation; }
		GAL_NODISCARD GAL_INLINE glm::vec3 getScale() const noexcept { return scale; }

		/// @brief Get the model matrix represented by this object. Automatically cached for your performance convenience.
		GAL_NODISCARD GAL_INLINE glm::mat4 getModelMatrix() const noexcept
		{
			if (dirty)
			{
				cachedModelMatrix = calculateModelMatrix();
				dirty = false;
			}

			return cachedModelMatrix;
		}

	private:
		glm::vec3 position;
		Rotation rotation;
		glm::vec3 scale;

		mutable glm::mat4 cachedModelMatrix;
		mutable bool dirty = false;

		GAL_INLINE glm::mat4 calculateModelMatrix() const noexcept
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
