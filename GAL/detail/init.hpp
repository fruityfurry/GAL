#ifndef GAL_INIT_HPP
#define GAL_INIT_HPP

#include "attributes.hpp"
#include "GALException.hpp"
#include "ShaderProgram.hpp"
#include "VertexArray.hpp"
#include "window.hpp"

namespace gal
{
	/// @brief Initialize everything GAL needs to run.
	GAL_INLINE void init()
	{
		if (!glfwInit())
		{
			detail::throwErr(ErrCode::GLFWInitFailed, "Failed to initialize GLFW.");
		}
	}

	/// @brief Terminate GAL and clean up everything it had allocated.
	GAL_INLINE void terminate()
	{
		destroyMainWindow();

		detail::shaderProgramTracker.clear();
		detail::bufferTracker.clear();
		detail::vertexArrayTracker.clear();

		glfwTerminate();
	}
}

#endif
