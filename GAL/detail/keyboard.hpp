#ifndef GAL_KEYBOARD_HPP
#define GAL_KEYBOARD_HPP

#include <algorithm>
#include <array>

#include "attributes.hpp"
#include "types.hpp"
#include "window.hpp"

namespace gal
{
	namespace detail
	{
		GAL_CONSTEXPR uint16_t maxKeys = GLFW_KEY_LAST + 1;

		GAL_INLINE std::array<type::GALBool, maxKeys> prevKeyStates = {};
		GAL_INLINE std::array<type::GALBool, maxKeys> currKeyStates = {};

		/// @brief Update the key states for this window. Required for keyboard-related functions like isKeyDown to work. 
		GAL_INLINE void updateKeyStates(GLFWwindow* window)
		{
			std::copy(currKeyStates.begin(), currKeyStates.end(), prevKeyStates.begin());

			for (int key = 0; key < maxKeys; ++key)
			{
				currKeyStates[key] = glfwGetKey(window, key) == GLFW_PRESS;
			}
		}

		/// @brief Return true if the given key code is valid.
		GAL_INLINE bool isValidKey(int key)
		{
			return key >= 0 && key < maxKeys;
		}
	}

	namespace keyboard
	{
		/// @brief Returns true once when a key is first pressed.
		/// Remember to call Window::updateState() to keep the internal key states updated.
		GAL_NODISCARD GAL_INLINE bool isKeyPressed(int key)
		{
			if (!detail::isValidKey(key))
				return false;

			return detail::currKeyStates[key] && !detail::prevKeyStates[key];
		}

		/// @brief Returns true once when a key is first released.
		/// Remember to call Window::updateState() to keep the internal key states updated.
		GAL_NODISCARD GAL_INLINE bool isKeyReleased(int key)
		{
			if (!detail::isValidKey(key))
				return false;

			return !detail::currKeyStates[key] && detail::prevKeyStates[key];
		}

		/// @brief Returns true if the key is currently held down.
		/// Remember to call Window::updateState() to keep the internal key states updated.
		GAL_NODISCARD GAL_INLINE bool isKeyDown(int key)
		{
			if (!detail::isValidKey(key))
				return false;

			return detail::currKeyStates[key];
		}

		/// @brief Returns true if the key is not currently held.
		/// Remember to call Window::updateState() to keep the internal key states updated.
		GAL_NODISCARD GAL_INLINE bool isKeyUp(int key)
		{
			if (!detail::isValidKey(key))
				return false;

			return !detail::currKeyStates[key];
		}
	}
}

#endif
