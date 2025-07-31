#ifndef GAL_ROTATION_HPP
#define GAL_ROTATION_HPP

namespace gal
{
	/// @brief Holds rotation information and provides a myriad of functions to work with that information.
	/// Euler angles are given in the order pitch (x-axis), yaw (y-axis), roll (z-axis).
	struct Rotation
	{
	public:
		/// @brief Initalize with no rotation.
		GAL_INLINE Rotation() { setQuat(); }

		/// @brief Initialize with a rotation quaternion. 
		GAL_INLINE Rotation(const glm::quat& rotationQuaternion) { setQuat(rotationQuaternion); }

		/// @brief Initialize with euler angles. 
		GAL_INLINE Rotation(const glm::vec3& eulerAngles) { setQuat(eulerAngles); }

		/// @brief Initialize with euler angles. 
		GAL_INLINE Rotation(float pitch, float yaw, float roll) { setQuat(glm::vec3(pitch, yaw, roll)); }

		/// @brief Initialize with an angle and an axis. 
		GAL_INLINE Rotation(const glm::vec3& axis, float angle) { setQuat(axis, angle); }

		/// @brief Initialize with a rotation matrix 
		GAL_INLINE Rotation(const glm::mat4& rotationMatrix) { setQuat(rotationMatrix); }

		/// @brief Set rotation to identity rotation. 
		GAL_INLINE void reset() noexcept { setQuat(); }

		/// @brief Set rotation.
		GAL_INLINE void set(const Rotation& rotation) noexcept { setQuat(rotation.quat); }
		GAL_INLINE void set(const glm::quat& rotationQuat) noexcept { setQuat(rotationQuat); }
		GAL_INLINE void set(const glm::vec3& eulerAngles) noexcept { setQuat(eulerAngles); }
		GAL_INLINE void set(float pitch, float yaw, float roll) noexcept { setQuat(glm::vec3(pitch, yaw, roll)); }
		GAL_INLINE void set(const glm::vec3& axis, float angle) noexcept { setQuat(axis, angle); }

		/// @brief Add rotation.
		GAL_INLINE void add(const Rotation& delta) { quat = glm::normalize(delta.quat * quat); }
		GAL_INLINE void add(const glm::quat& delta) { quat = glm::normalize(delta * quat); }
		GAL_INLINE void add(const glm::vec3& eulerAngles) { add(glm::quat(eulerAngles)); }
		GAL_INLINE void add(float pitch, float yaw, float roll) { add(glm::vec3(pitch, yaw, roll)); }
		GAL_INLINE void add(const glm::vec3& axis, float angle) { add(glm::angleAxis(angle, axis)); }

		GAL_NODISCARD GAL_INLINE Rotation inverse() const noexcept { return Rotation(glm::inverse(quat)); }
		GAL_NODISCARD GAL_INLINE Rotation conjugate() const noexcept { return Rotation(glm::conjugate(quat)); }

		GAL_NODISCARD GAL_INLINE glm::quat asQuat() const { return quat; }
		GAL_NODISCARD GAL_INLINE glm::vec3 asEulerAngles() const { return glm::eulerAngles(quat); }
		GAL_NODISCARD GAL_INLINE glm::mat4 asMat4() const { return glm::mat4_cast(quat); }

		/// @brief Rotate a point. 
		GAL_NODISCARD GAL_INLINE glm::vec3 rotate(const glm::vec3& point) const noexcept { return quat * point; }

		/// @brief Spherically interpolate between another rotation. 
		GAL_NODISCARD GAL_INLINE Rotation slerp(const Rotation& other, float t) const { return Rotation(glm::slerp(quat, other.quat, t)); }

		/// @brief Get a reference to the internal quaternion used by this object. You may alter this quaternion freely.
		GAL_NODISCARD GAL_INLINE glm::quat& quatRef() noexcept { return quat; }

	private:
		glm::quat quat;

		GAL_INLINE void setQuat() noexcept { quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); }
		GAL_INLINE void setQuat(const glm::quat& rotationQuaternion) noexcept { quat = rotationQuaternion; }
		GAL_INLINE void setQuat(const glm::vec3& eulerAngles) noexcept { quat = glm::quat(eulerAngles); }
		GAL_INLINE void setQuat(const glm::vec3& axis, float angle) noexcept { quat = glm::angleAxis(angle, axis); }
		GAL_INLINE void setQuat(const glm::mat4& rotationMatrix) noexcept { quat = glm::quat(rotationMatrix); }
	};
}

#endif
