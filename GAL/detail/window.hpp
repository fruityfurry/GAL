#ifndef GAL_WINDOW_HPP
#define GAL_WINDOW_HPP

#include "attributes.hpp"
#include "GALException.hpp"
namespace gal::detail { void updateKeyStates(); }

namespace gal
{
	namespace detail
	{
		inline GLFWwindow* mainWindow = nullptr;

		GAL_INLINE void defaultResizeCallback(GLFWwindow*, int width, int height)
		{
			glViewport(0, 0, width, height);
		}
	}

	/// @brief Set GAL's main window to this window. All subsequent GAL function calls that need a window will operate on this window.
	GAL_INLINE void setMainWindow(GLFWwindow* window)
	{
		detail::mainWindow = window;
	}

	/// @brief Get GAL's main window.
	GAL_NODISCARD GAL_INLINE GLFWwindow* getMainWindow()
	{
		return detail::mainWindow;
	}

	/// @brief Create a simple window, make its context current, and set the internal main window of GAL to this new window.
	/// You won't need to pass the window pointer to any of GAL's functions, but you should still keep it handy for calling glfw functions yourself.
	/// Throws an error if a window has already been created. Call destroyMainWindow() to clear this and allow creation of another window.
	/// Setting resizable to true gives the window a simple default resize callback. You'll probably want to change this.
	GAL_INLINE GLFWwindow* createMainWindow(int glfwVersionMajor, int glfwVersionMinor, bool useCore, int windowWidth, int windowHeight,
		const char* windowTitle, bool resizable, bool vsync, GLFWmonitor* monitor = nullptr)
	{
		if (detail::mainWindow != nullptr)
			detail::throwErr(ErrCode::WindowDuplication, "Tried creating a window when mainWindow was already set.");

#ifndef GAL_DISABLE_MIN_GLFW_VERSION
		if (glfwVersionMajor < 4 || glfwVersionMinor < 5)
			detail::throwErr(ErrCode::GLFWVersionTooOld, "Attempted to create a window with a version of GLFW that was too low (< 4.5). "
														 "Define GAL_DISABLE_MIN_GLFW_VERSION if you REALLY need to get around this.");
#endif

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glfwVersionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glfwVersionMinor);
		glfwWindowHint(GLFW_RESIZABLE, resizable);

		if (useCore)
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, monitor, nullptr);
		if (!window)
			detail::throwErr(ErrCode::CreateWindowFailed, "Failed to create window.");

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			detail::throwErr(ErrCode::GLADInitFailed, "Failed to initialize GLAD.");

		detail::initCommonGLParams();

		glViewport(0, 0, windowWidth, windowHeight);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (resizable)
			glfwSetFramebufferSizeCallback(window, detail::defaultResizeCallback);
		if (vsync)
			glfwSwapInterval(1);

		setMainWindow(window);
		return window;
	}

	/// @brief Destroy GAL's main window and make way for a new one to be created. 
	GAL_INLINE void destroyMainWindow()
	{
		if (detail::mainWindow)
		{
			glfwDestroyWindow(detail::mainWindow);
			detail::mainWindow = nullptr;
		}
	}

	/// @brief Set the main window's clear color. Subsequent calls to clearBackground will now use this color.
	GAL_INLINE void setClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	/// @brief Clear the background of the main window with the background color set by setClearColor(). Default is black.
	GAL_INLINE void clearBackground()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	/// @brief Wrapper around GLFW function of same name for homogeneity. 
	GAL_INLINE bool windowShouldClose()
	{
		return glfwWindowShouldClose(detail::mainWindow);
	}

	/// @brief Wrapper around GLFW function of same name for homogeneity. 
	GAL_INLINE void setWindowShouldClose(bool val)
	{
		return glfwSetWindowShouldClose(detail::mainWindow, val);
	}

	/// @brief Wrapper around GLFW function of same name for homogeneity. 
	GAL_INLINE void swapBuffers()
	{
		glfwSwapBuffers(detail::mainWindow);
	}

	/// @brief Poll for events for this window and update internal state.
	/// Should be called at the top of the update loop for responsive controls. 
	GAL_INLINE void pollEvents()
	{
		if (!detail::mainWindow)
			detail::throwErr(ErrCode::NoMainWindow, "pollEvents() called with no main window set.");

		glfwPollEvents();
		detail::updateKeyStates();
	}

	/// @brief Set the polygon mode of the main window, which affects how polygons are drawn. 
	GAL_INLINE void setPolygonMode(PolygonMode mode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(mode));
	}
}

#endif