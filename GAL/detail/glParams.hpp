#ifndef GAL_GL_PARAMS_HPP
#define GAL_GL_PARAMS_HPP

#include "attributes.hpp"

namespace gal
{
	namespace detail
	{
		GAL_INLINE GLuint maxVertexAttribs;
		GAL_INLINE GLuint maxVertexAttribBindings;
	}

	/// @brief Functions for querying OpenGL parameters (like with glGetIntegerv(), etc.).
	/// Some common ones that stay constant throughout runtime are initialized when creating
	/// a window and can be queried with their own functions.
	namespace glParams
	{
		GAL_INLINE GLboolean queryGLParamBool(GLenum name)
		{
			GLboolean param;
			glGetBooleanv(name, &param);
			return param;
		}

		GAL_INLINE GLdouble queryGLParamDouble(GLenum name)
		{
			GLdouble param;
			glGetDoublev(name, &param);
			return param;
		}

		GAL_INLINE GLfloat queryGLParamFloat(GLenum name)
		{
			GLfloat param;
			glGetFloatv(name, &param);
			return param;
		}

		GAL_INLINE GLint queryGLParamInt(GLenum name)
		{
			GLint param;
			glGetIntegerv(name, &param);
			return param;
		}

		GAL_INLINE GLint64 queryGLParamInt64(GLenum name)
		{
			GLint64 param;
			glGetInteger64v(name, &param);
			return param;
		}

		GAL_INLINE GLboolean queryGLIndexedParamBool(GLenum name, GLuint index)
		{
			GLboolean param;
			glGetBooleani_v(name, index, &param);
			return param;
		}

		GAL_INLINE GLdouble queryGLIndexedParamDouble(GLenum name, GLuint index)
		{
			GLdouble param;
			glGetDoublei_v(name, index, &param);
			return param;
		}

		GAL_INLINE GLfloat queryGLIndexedParamFloat(GLenum name, GLuint index)
		{
			GLfloat param;
			glGetFloati_v(name, index, &param);
			return param;
		}

		GAL_INLINE GLint queryGLIndexedParamInt(GLenum name, GLuint index)
		{
			GLint param;
			glGetIntegeri_v(name, index, &param);
			return param;
		}

		GAL_INLINE GLint64 queryGLIndexedParamInt64(GLenum name, GLuint index)
		{
			GLint64 param;
			glGetInteger64i_v(name, index, &param);
			return param;
		}

		GAL_INLINE GLint getMaxVertexAttribs() { return detail::maxVertexAttribs; }
		GAL_INLINE GLint getMaxVertexAttribBindings() { return detail::maxVertexAttribBindings; }
	}

	namespace detail
	{
		GAL_INLINE void initCommonGLParams()
		{
			maxVertexAttribs = glParams::queryGLParamInt(GL_MAX_VERTEX_ATTRIBS);
			maxVertexAttribBindings = glParams::queryGLParamInt(GL_MAX_VERTEX_ATTRIB_BINDINGS);
		}
	}
}

#endif
