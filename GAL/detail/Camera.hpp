#ifndef GAL_CAMERA_HPP
#define GAL_CAMERA_HPP

#include "Rotation.hpp"

namespace gal
{
	class Camera
	{
	public:
		glm::vec3 position;
		Rotation rotation;  // Default rotation is towards -Z.

		/// @brief Initialize with a position and default direction (towards -Z).
		GAL_INLINE Camera(const glm::vec3& position)
			: position(position), rotation() { }

		/// @brief Initialize with a position and rotation from default direction (towards -Z).
		GAL_INLINE Camera(const glm::vec3& position, const Rotation& rotation)
			: position(position), rotation(rotation) { }

		/// @brief Initialize with a position and direction. Direction is automatically normalized.
		GAL_INLINE Camera(const glm::vec3& position, const glm::vec3& direction)
			: position(position), rotation(glm::vec3(0.0f, 0.0f, -1.0f), direction) { }

		GAL_INLINE glm::mat4 getViewMatrix() const noexcept
		{
			const glm::mat4 rotationMatrix = rotation.asMat4();
			const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -position);

			return rotationMatrix * translationMatrix;
		}

		GAL_INLINE glm::vec3 getForward() const noexcept { return rotation.rotate(glm::vec3(0.0f, 0.0f, -1.0f)); }
		GAL_INLINE glm::vec3 getRight() const noexcept { return rotation.rotate(glm::vec3(1.0f, 0.0f, 0.0f)); }
		GAL_INLINE glm::vec3 getUp() const noexcept { return rotation.rotate(glm::vec3(0.0f, 1.0f, 0.0f)); }

		GAL_INLINE void move(const glm::vec3& delta) noexcept { position += delta; }

		GAL_INLINE void moveLocal(const glm::vec3& delta) noexcept
		{
			position += getRight() * delta.x;
			position += getUp() * delta.y;
			position += getForward() * delta.z;
		}

		GAL_INLINE void rotate(const Rotation& rotation) noexcept { this->rotation.followWith(rotation); }
	};
}

#endif
