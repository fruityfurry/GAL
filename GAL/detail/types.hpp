#ifndef GAL_TYPES_HPP
#define GAL_TYPES_HPP

#include <cstdint>

/// @brief Types used by GAL.
namespace gal
{
	namespace type
	{
		// Simple typedefs.
		using GALBool = uint8_t;

		// Type used for shader, buffer, etc. IDs.
		using GALIDType = GLuint;

		// ID Types for semantic clarity.
		using GALShaderProgramID = GALIDType;
		using GALBufferID = GALIDType;
		using GALVertexArrayID = GALIDType;
		using GALTextureID = GALIDType;

		GAL_STATIC GAL_CONSTEXPR GLsizeiptr NullSize = static_cast<GLsizeiptr>(-1);
	}
}

#endif
