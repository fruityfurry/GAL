#ifndef GAL_STATE_HPP
#define GAL_STATE_HPP

#include <thread>

namespace gal
{
	namespace detail
	{
		GAL_INLINE double lastProgramTime = 0.0;
		GAL_INLINE double programTime = 0.0;

		GAL_INLINE void updateState()
		{
			lastProgramTime = programTime;
			programTime = glfwGetTime();
		}
	}

	/// @brief Get current program time. 
	template<typename T = double>
	GAL_INLINE T getTime()
	{
		return static_cast<T>(detail::programTime);
	}

	/// @brief Get time elapsed since last call to updateState(). 
	template<typename T = double>
	GAL_INLINE T getDeltaTime()
	{
		return static_cast<T>(detail::programTime - detail::lastProgramTime);
	}
}

#endif
