#ifndef GAL_DEBUG_HPP
#define GAL_DEBUG_HPP

#include "logging.hpp"

namespace gal
{
	namespace detail
	{
		GAL_INLINE void APIENTRY defaultDebugMessageCallback(GLenum source, GLenum type, GLuint id,
			GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
		{
#ifndef GAL_SUPPRESS_LOGS
			using namespace std;

			cerr << "\u001b[31m";  // Red text.
			cerr << "GAL_LOG: An OpenGL error (" << id << ") occured. Details to follow.\n";

			cerr << "Source: ";
			switch (source)
			{
				case GL_DEBUG_SOURCE_API: cerr << "Call to OpenGL API\n"; break;
				case GL_DEBUG_SOURCE_WINDOW_SYSTEM: cerr << "Call to window system API\n"; break;
				case GL_DEBUG_SOURCE_SHADER_COMPILER: cerr << "Shader compiler\n"; break;
				case GL_DEBUG_SOURCE_THIRD_PARTY: cerr << "An application associated with OpenGL\n"; break;
				case GL_DEBUG_SOURCE_APPLICATION: cerr << "This application\n"; break;

				default: cerr << "Other\n"; break;
			}

			cerr << "Type: ";
			switch (type)
			{
				case GL_DEBUG_TYPE_ERROR: cerr << "Error\n"; break;
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: cerr << "Deprecated behavior\n"; break;
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: cerr << "Undefined behavior\n"; break;
				case GL_DEBUG_TYPE_PORTABILITY: cerr << "Portability\n"; break;
				case GL_DEBUG_TYPE_PERFORMANCE: cerr << "Performance issue\n"; break;
				case GL_DEBUG_TYPE_MARKER: cerr << "Command stream annotation\n"; break;
				case GL_DEBUG_TYPE_PUSH_GROUP: cerr << "Group pushing\n"; break;
				case GL_DEBUG_TYPE_POP_GROUP: cerr << "Group popping\n"; break;

				default: cerr << "Other\n"; break;
			}

			cerr << "Severity: ";
			switch (severity)
			{
				case GL_DEBUG_SEVERITY_HIGH: cerr << "High\n"; break;
				case GL_DEBUG_SEVERITY_MEDIUM: cerr << "Medium\n"; break;
				case GL_DEBUG_SEVERITY_LOW: cerr << "Low\n"; break;
				case GL_DEBUG_SEVERITY_NOTIFICATION: cerr << "Notification\n"; break;
			}

			cerr << "OpenGL Message:\n";
			cerr << message;
			cerr << "\u001b[37m";  // Reset to white text.
#endif
		}
	}
}

#endif
