#ifndef GAL_INIT_HPP
#define GAL_INIT_HPP

#include "attributes.hpp"
#include "GALException.hpp"
#include "ShaderProgram.hpp"
#include "VertexArray.hpp"
#include "window.hpp"

namespace gal
{
	namespace detail
	{
		/// @brief Initialization that can only be done after an OpenGL context has been created. 
		GAL_INLINE void postGLInit()
		{
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
				detail::throwErr(ErrCode::GLADInitFailed, "Failed to initialize GLAD.");

			detail::initCommonGLParams();

			detail::postGLInitialized = true;
		}
	}

	/// @brief Initialize everything GAL needs to run.
	GAL_INLINE void init()
	{
		if (!glfwInit())
		{
			detail::throwErr(ErrCode::GLFWInitFailed, "Failed to initialize GLFW.");
		}
	}

	/// @brief Terminate GAL, clean up everything it had allocated, and reset all internal state variables to their defaults.
	GAL_INLINE void terminate()
	{
		detail::openGLVersionMajor = -1;
		detail::openGLVersionMinor = -1;

		detail::postGLInitialized = false;

		detail::windowTracker.clear();
		detail::shaderProgramTracker.clear();
		detail::bufferTracker.clear();
		detail::vertexArrayTracker.clear();

		glfwTerminate();
	}
}

#endif
