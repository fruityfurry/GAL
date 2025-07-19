#ifndef GAL_WINDOW_HPP
#define GAL_WINDOW_HPP

#include "attributes.hpp"
#include "GALException.hpp"
namespace gal::detail { void updateKeyStates(GLFWwindow*); }

namespace gal
{
	namespace detail
	{
		GAL_INLINE void deleteWindow(GLFWwindow* window)
		{
			glfwDestroyWindow(window);
		}

		GAL_INLINE ResourceTracker<GLFWwindow*, deleteWindow> windowTracker;

		GAL_INLINE int openGLVersionMajor = -1;
		GAL_INLINE int openGLVersionMinor = -1;

		GAL_INLINE bool postGLInitialized = false;

		GAL_INLINE void defaultResizeCallback(GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}
	}

	/// @brief Set the version of OpenGL to be used from now on. Cannot be called more than once unless terminate() is called.
	GAL_INLINE void setOpenGLVersion(int major, int minor)
	{
		if (detail::openGLVersionMajor != -1 || detail::openGLVersionMinor != -1)
			detail::throwErr(ErrCode::SetOpenGLVersionTwice, "Attempted to set OpenGL version twice.");

#ifndef GAL_DISABLE_MIN_GL_VERSION
		if (major < 4 || minor < 5)
			detail::throwErr(ErrCode::GLVersionTooOld, "Attempted to create a window with a version of GLFW that was too low (< 4.5). "
														 "Define GAL_DISABLE_MIN_GLFW_VERSION if you REALLY need to get around this.");

		detail::openGLVersionMajor = major;
		detail::openGLVersionMinor = minor;
#endif
	}

	class Window
	{
	public:
		GAL_INLINE Window(int windowWidth, int windowHeight, const char* windowTitle, bool useCoreProfile = true,
			bool resizable = false, bool vsync = false, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr)
			: width(width), height(height)
		{
			if (detail::openGLVersionMajor == -1 || detail::openGLVersionMinor == -1)
				detail::throwErr(ErrCode::OpenGLVersionUnset, "OpenGL Version left unset when creating a window.");

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, detail::openGLVersionMajor);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, detail::openGLVersionMinor);
			glfwWindowHint(GLFW_RESIZABLE, resizable);

			if (useCoreProfile)
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, monitor, share);
			if (!window)
				detail::throwErr(ErrCode::CreateWindowFailed, "Failed to create window.");

			detail::windowTracker.add(window);

			glfwMakeContextCurrent(window);

			if (!detail::postGLInitialized)
			{
				if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
					detail::throwErr(ErrCode::GLADInitFailed, "Failed to initialize GLAD.");

				detail::initCommonGLParams();

				detail::postGLInitialized = true;
			}

			glViewport(0, 0, windowWidth, windowHeight);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			if (resizable)
				glfwSetFramebufferSizeCallback(window, detail::defaultResizeCallback);
			if (vsync)
				glfwSwapInterval(1);
		}

		// Forbid copying.
		GAL_INLINE Window(const Window&) = delete;
		GAL_INLINE Window& operator=(const Window&) = delete;

		// Allow moving.
		GAL_INLINE Window(Window&&) noexcept = default;
		GAL_INLINE Window& operator=(Window&&) noexcept = default;

		GAL_INLINE ~Window()
		{
			detail::windowTracker.remove(window);
		}

		GAL_NODISCARD GAL_INLINE int getWidth() const noexcept
		{
			return width;
		}

		GAL_NODISCARD GAL_INLINE int getHeight() const noexcept
		{
			return height;
		}

		GAL_INLINE void setSize(int width, int height) const
		{
			glfwSetWindowSize(window, width, height);
		}

		GAL_INLINE void pollEvents() const noexcept
		{
			glfwPollEvents();
			detail::updateKeyStates(window);
		}

		GAL_NODISCARD GAL_INLINE bool shouldClose() const noexcept
		{
			return glfwWindowShouldClose(window);
		}

		GAL_INLINE void setShouldClose(bool val) const noexcept
		{
			return glfwSetWindowShouldClose(window, val);
		}

		GAL_INLINE void swapBuffers() const noexcept
		{
			glfwSwapBuffers(window);
		}

		/// @brief Set window's clear color. Subsequent calls to clearBackground, if not overriden, will now use this color.
		GAL_INLINE void setClearColor(float r, float g, float b, float a)
		{
			glClearColor(r, g, b, a);
		}

		/// @brief Clear the background of the window with the background color set by setClearColor(). Default is black.
		GAL_INLINE void clearBackground()
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}

	private:
		GLFWwindow* window;
		int width;
		int height;
	};
}

#endif