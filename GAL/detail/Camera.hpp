#ifndef GAL_CAMERA_HPP
#define GAL_CAMERA_HPP

#include "Rotation.hpp"

namespace gal
{
	/// @brief Camera class with view matrix caching.
	class Camera
	{
	public:
		/// @brief Initialize with a position and default direction (towards -Z).
		GAL_INLINE Camera(const glm::vec3& position)
			: position(position), rotation() { cachedViewMatrix = calculateViewMatrix(); }

		/// @brief Initialize with a position and rotation from default direction (towards -Z).
		GAL_INLINE Camera(const glm::vec3& position, const Rotation& rotation)
			: position(position), rotation(rotation) { cachedViewMatrix = calculateViewMatrix(); }

		/// @brief Initialize with a position and direction. Direction is automatically normalized.
		GAL_INLINE Camera(const glm::vec3& position, const glm::vec3& direction)
			: position(position), rotation(glm::vec3(0.0f, 0.0f, -1.0f), direction) { cachedViewMatrix = calculateViewMatrix(); }
		
		/// @brief Move the camera in global space. 
		GAL_INLINE void moveGlobal(const glm::vec3& delta) noexcept { position += delta; dirty = true; }

		/// @brief Move the camera in local space. 
		GAL_INLINE void moveLocal(const glm::vec3& delta) noexcept
		{
			position += getRight() * delta.x;
			position += getUp() * delta.y;
			position += getForward() * delta.z;

			dirty = true;
		}

		/// @brief Rotate the camera in global space. 
		GAL_INLINE void rotateGlobal(const Rotation& delta) noexcept { rotation.rotateGlobal(delta); dirty = true; }

		/// @brief Rotate the camera in local space. 
		GAL_INLINE void rotateLocal(const Rotation& delta) noexcept { rotation.rotateLocal(delta); dirty = true; }

		GAL_NODISCARD GAL_INLINE glm::vec3 getPosition() const noexcept { return position; }
		GAL_NODISCARD GAL_INLINE Rotation getRotation() const noexcept { return rotation; }

		GAL_NODISCARD GAL_INLINE glm::vec3 getForward() const noexcept { return rotation.rotate(glm::vec3(0.0f, 0.0f, -1.0f)); }
		GAL_NODISCARD GAL_INLINE glm::vec3 getRight() const noexcept { return rotation.rotate(glm::vec3(1.0f, 0.0f, 0.0f)); }
		GAL_NODISCARD GAL_INLINE glm::vec3 getUp() const noexcept { return rotation.rotate(glm::vec3(0.0f, 1.0f, 0.0f)); }

		GAL_INLINE void lookAt(const glm::vec3& target, const glm::vec3& up) noexcept
		{
			rotation = glm::quatLookAt(glm::normalize(target - position), up);
			dirty = true;
		}

		GAL_INLINE void lookAt(const glm::vec3& target) { lookAt(target, getUp()); }

		/// @brief Get the view matrix for this camera. Automatically cached for your performance convenience.
		GAL_NODISCARD GAL_INLINE glm::mat4 getViewMatrix() const noexcept
		{
			if (dirty)
			{
				cachedViewMatrix = calculateViewMatrix();
				dirty = false;
			}

			return cachedViewMatrix;
		}

	private:
		glm::vec3 position;
		Rotation rotation;

		mutable glm::mat4 cachedViewMatrix;
		mutable bool dirty = false;

		GAL_INLINE glm::mat4 calculateViewMatrix() const noexcept
		{
			const glm::mat4 rotationMatrix = rotation.inverse().asMat4();
			const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -position);

			return rotationMatrix * translationMatrix;
		}
	};
}

#endif
