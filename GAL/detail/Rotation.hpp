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
		GAL_INLINE Rotation() { reset(); }

		/// @brief Initialize with a rotation quaternion. 
		GAL_INLINE Rotation(const glm::quat& rotationQuaternion) { set(rotationQuaternion); }

		/// @brief Initialize with euler angles. 
		GAL_INLINE Rotation(const glm::vec3& eulerAngles) { set(eulerAngles); }

		/// @brief Initialize with an angle and an axis. 
		GAL_INLINE Rotation(const glm::vec3& axis, float angle) { set(axis, angle); }

		/// @brief Initialize with a rotation matrix 
		GAL_INLINE Rotation(const glm::mat4& rotationMat) { set(rotationMat); }

		/// @brief Initialize with the rotation from one vector to another. Automatically normalizes both vectors.
		GAL_INLINE Rotation(const glm::vec3& from, const glm::vec3& to) { set(from, to); }

		/// @brief Set rotation to identity rotation. 
		GAL_INLINE void reset() noexcept { quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); }

		/// @brief Set rotation.
		GAL_INLINE void set(const Rotation& rotation) noexcept { quat = rotation.quat; }
		GAL_INLINE void set(const glm::quat& rotationQuat) noexcept { quat = rotationQuat; }
		GAL_INLINE void set(const glm::vec3& eulerAngles) noexcept { quat = glm::quat(eulerAngles); }
		GAL_INLINE void set(const glm::vec3& axis, float angle) noexcept { quat = glm::angleAxis(angle, axis); }
		GAL_INLINE void set(const glm::mat4& rotationMat) noexcept { quat = glm::quat(rotationMat); }
		GAL_INLINE void set(const glm::vec3& from, const glm::vec3& to) noexcept
		{
			const glm::vec3 normFrom = glm::normalize(from);
			const glm::vec3 normTo = glm::normalize(to);

			const float dotProduct = glm::dot(normFrom, normTo);
			const float angle = glm::acos(dotProduct);
			const glm::vec3 axis = glm::normalize(glm::cross(normFrom, normTo));

			set(axis, angle);
		}

		/// @brief Follow this rotation with another, storing the result in this object.
		GAL_INLINE void followWith(const Rotation& delta) noexcept { quat = glm::normalize(delta.quat * quat); }
		/// @brief Precede this rotation with another, storing the result in this object.
		GAL_INLINE void precedeWith(const Rotation& delta) noexcept { quat = glm::normalize(quat * delta.quat); }

		/// @brief Get this rotation followed by another.
		GAL_NODISCARD GAL_INLINE Rotation followedWith(const Rotation& delta) const noexcept { return glm::normalize(delta.quat * quat); }
		/// @brief Get this rotation preceded by another.
		GAL_NODISCARD GAL_INLINE Rotation precededWith(const Rotation& delta) const noexcept { return glm::normalize(quat * delta.quat); }

		GAL_NODISCARD GAL_INLINE Rotation inverse() const noexcept { return Rotation(glm::inverse(quat)); }
		GAL_NODISCARD GAL_INLINE Rotation conjugate() const noexcept { return Rotation(glm::conjugate(quat)); }

		GAL_NODISCARD GAL_INLINE glm::quat asQuat() const noexcept { return quat; }
		GAL_NODISCARD GAL_INLINE glm::vec3 asEulerAngles() const noexcept { return glm::eulerAngles(quat); }
		GAL_NODISCARD GAL_INLINE glm::mat4 asMat4() const noexcept { return glm::mat4_cast(quat); }

		/// @brief Rotate a point according to this rotation.
		GAL_NODISCARD GAL_INLINE glm::vec3 rotate(const glm::vec3& point) const noexcept { return quat * point; }

		/// @brief Spherically interpolate between another rotation. 
		GAL_NODISCARD GAL_INLINE Rotation slerp(const Rotation& other, float t) const noexcept { return Rotation(glm::slerp(quat, other.quat, t)); }

		/// @brief Get a reference to the internal quaternion used by this object. You may alter this quaternion freely.
		GAL_NODISCARD GAL_INLINE glm::quat& quatRef() noexcept { return quat; }

	private:
		glm::quat quat;
	};
}

#endif
