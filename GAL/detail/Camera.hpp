#ifndef GAL_CAMERA_HPP
#define GAL_CAMERA_HPP

#include "Rotation.hpp"

namespace gal
{
	/// @brief Camera struct.
	struct Camera
	{
		glm::vec3 position;
		Rotation rotation;

		/// @brief Initialize with a position and default direction (towards -Z).
		GAL_INLINE Camera(const glm::vec3& position)
			: position(position), rotation() { }

		/// @brief Initialize with a position and rotation from default direction (towards -Z).
		GAL_INLINE Camera(const glm::vec3& position, const Rotation& rotation)
			: position(position), rotation(rotation) { }

		/// @brief Initialize with a position and direction. Direction is automatically normalized.
		GAL_INLINE Camera(const glm::vec3& position, const glm::vec3& direction)
			: position(position), rotation(glm::vec3(0.0f, 0.0f, -1.0f), direction) { }
		
		/// @brief Move the camera in global space. 
		GAL_INLINE void moveGlobal(const glm::vec3& delta) noexcept { position += delta; }

		/// @brief Move the camera in local space. 
		GAL_INLINE void moveLocal(const glm::vec3& delta) noexcept
		{
			position += getRight() * delta.x;
			position += getUp() * delta.y;
			position += getForward() * delta.z;
		}

		/// @brief Rotate the camera in global space. 
		GAL_INLINE void rotateGlobal(const Rotation& delta) noexcept { rotation.rotateGlobal(delta); }

		/// @brief Rotate the camera in local space. 
		GAL_INLINE void rotateLocal(const Rotation& delta) noexcept { rotation.rotateLocal(delta); }

		GAL_NODISCARD GAL_INLINE glm::vec3 getForward() const noexcept { return rotation.rotate(glm::vec3(0.0f, 0.0f, -1.0f)); }
		GAL_NODISCARD GAL_INLINE glm::vec3 getRight() const noexcept { return rotation.rotate(glm::vec3(1.0f, 0.0f, 0.0f)); }
		GAL_NODISCARD GAL_INLINE glm::vec3 getUp() const noexcept { return rotation.rotate(glm::vec3(0.0f, 1.0f, 0.0f)); }

		/// @brief Make the camera look at a target point in world space, with the given up vector. 
		GAL_INLINE void lookAt(const glm::vec3& target, const glm::vec3& up) noexcept
		{
			rotation = glm::quatLookAt(glm::normalize(target - position), up);
		}

		/// @brief  Make the camera look at a target point in world space, with the camera's current up vector. 
		GAL_INLINE void lookAt(const glm::vec3& target) { lookAt(target, getUp()); }

		/// @brief Get the view matrix for this camera.
		GAL_NODISCARD GAL_INLINE glm::mat4 getViewMatrix() const noexcept
		{
			const glm::mat4 rotationMatrix = rotation.inverse().asMat4();
			const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -position);

			return rotationMatrix * translationMatrix;
		}
	};
}

#endif
