#ifndef GAL_TYPES_HPP
#define GAL_TYPES_HPP

#include <cstdint>

/// @brief Types used by GAL.
namespace gal
{
	namespace type
	{
		// Simple typedefs.
		using OBRBool = uint8_t;

		// Type used for shader, buffer, etc. IDs.
		using OBRIDType = GLuint;

		// ID Types for semantic clarity.
		using OBRShaderProgramID = OBRIDType;
		using OBRBufferID = OBRIDType;
		using OBRVertexArrayID = OBRIDType;

		GAL_STATIC GAL_CONSTEXPR GLsizeiptr OBRSizeNull = static_cast<GLsizeiptr>(-1);
		GAL_STATIC GAL_CONSTEXPR OBRIDType OBRIDNull = static_cast<OBRIDType>(-1);
	}
}

#endif
