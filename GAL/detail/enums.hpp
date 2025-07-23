#ifndef GAL_ENUMS_HPP
#define GAL_ENUMS_HPP

namespace gal
{
	/// @brief Enum of all the various errors that can occur in GAL.
	enum class ErrCode
	{
		// Init.
		GLFWInitFailed, // Failed to initialize GLFW.
		GLADInitFailed, // Failed to initialize GLAD.

		// Window.
		GLVersionTooOld, // Attempted to create a window with a version of GLFW that was too low (< 4.5).
		CreateWindowFailed, // Failed to create a window.
		SetOpenGLVersionTwice, // Attempted to set OpenGL version twice.
		OpenGLVersionUnset, // OpenGL Version left unset when creating a window.

		// Shader.
		ShaderReadFailed, // Failed to read shader file.
		ShaderCompFailed, // Failed to compile shader.
		ShaderProgramLinkFailed, // Failed to link shader program.
		NonExistentShaderUniform, // Attempted to set a non-existent shader uniform.
		ShaderProgramUseBeforeLinking, // Attempted to use a shader program before linking it.
		UniformSetBeforeLinking, // Attempted to set a shader uniform before linking the shader program.
		AddShaderAfterLinking, // Attempted to add a shader to a shader program after linking it.
		ShaderProgramDoubleLink, // Attempted to link a shader even though it was already linked.

		// Buffer.
		BufferUseBeforeAllocation,

		// Vertex Array.
		VertexBufferIndexOutOfRange, // Attempted to bind a vertex buffer to an index that was out of range (> GL_MAX_VERTEX_ATTRIB_BINDINGS).
		VertexAttributeIndexOutOfRange, // Attempted to add a vertex attribute with an index that was out of range (> GL_MAX_VERTEX_ATTRIBS - 1).
		GotNullBuffer, // Attempted to get a null buffer.
	};

    /// @brief Convert a GAL error code to a string.
    GAL_NODISCARD GAL_INLINE const char* errCodeToString(ErrCode errCode)
    {
		switch (errCode)
		{
			case ErrCode::GLFWInitFailed: return "GLFWInitFailed";
			case ErrCode::GLADInitFailed: return "GLADInitFailed";

			case ErrCode::GLVersionTooOld: return "GLVersionTooOld";
			case ErrCode::CreateWindowFailed: return "CreateWindowFailed";
			case ErrCode::SetOpenGLVersionTwice: return "SetOpenGLVersionTwice";
			case ErrCode::OpenGLVersionUnset: return "OpenGLVersionUnset";

			case ErrCode::ShaderReadFailed: return "ShaderReadFailed";
			case ErrCode::ShaderCompFailed: return "ShaderCompFailed";
			case ErrCode::ShaderProgramLinkFailed: return "ShaderProgramLinkFailed";
			case ErrCode::NonExistentShaderUniform: return "NonExistentShaderUniform";
			case ErrCode::ShaderProgramUseBeforeLinking: return "ShaderProgramUseBeforeLinking";
			case ErrCode::UniformSetBeforeLinking: return "UniformSetBeforeLinking";
			case ErrCode::AddShaderAfterLinking: return "AddShaderAfterLinking";
			case ErrCode::ShaderProgramDoubleLink: return "ShaderProgramDoubleLink";

			case ErrCode::BufferUseBeforeAllocation: return "BufferUseBeforeAllocation";

			case ErrCode::VertexBufferIndexOutOfRange: return "VertexBufferIndexOutOfRange";
			case ErrCode::VertexAttributeIndexOutOfRange: return "VertexAttributeIndexOutOfRange";
			case ErrCode::GotNullBuffer: return "GotNullBuffer";

			default: return "Unknown";
		}
    }

	/// @brief Enum of all possible shader types.
	/// Values align with GLenums of same names.
	enum class ShaderType : GLenum
	{
		Compute = GL_COMPUTE_SHADER,
		Vertex = GL_VERTEX_SHADER,
		TessControl = GL_TESS_CONTROL_SHADER,
		TessEvaluation = GL_TESS_EVALUATION_SHADER,
		Geometry = GL_GEOMETRY_SHADER,
		Fragment = GL_FRAGMENT_SHADER
	};

	/// @brief Enum of all possible polygon modes.
	/// Values align with GLenums of same names.
	enum class PolygonMode : GLenum
	{
		Point = GL_POINT, // Draw only polygon vertices as points. See GL_POINT_SIZE for further configuration.
		Line = GL_LINE, // Draw only polygon edges as lines. See GL_LINE_WIDTH for further configuration.
		Fill = GL_FILL // Draw the whole polygon.
	};

	/// @brief Enum of all possible buffer types.
	/// Values align with GLenums of same names.
	enum class BufferType : GLenum
	{
		Array             = GL_ARRAY_BUFFER,
		AtomicCounter     = GL_ATOMIC_COUNTER_BUFFER,
		CopyRead          = GL_COPY_READ_BUFFER,
		CopyWrite         = GL_COPY_WRITE_BUFFER,
		DispatchIndirect  = GL_DISPATCH_INDIRECT_BUFFER,
		DrawIndirect      = GL_DRAW_INDIRECT_BUFFER,
		ElementArray      = GL_ELEMENT_ARRAY_BUFFER,
		PixelPack         = GL_PIXEL_PACK_BUFFER,
		PixelUnpack       = GL_PIXEL_UNPACK_BUFFER,
		Query             = GL_QUERY_BUFFER,
		ShaderStorage     = GL_SHADER_STORAGE_BUFFER,
		Texture           = GL_TEXTURE_BUFFER,
		TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,
		Uniform           = GL_UNIFORM_BUFFER
	};

	/// @brief Enum of all possible buffer usage hints.
	/// Values align with GLenums of same names.
	enum class BufferUsageHint : GLenum
	{
		Null = 0,

		StaticDraw = GL_STATIC_DRAW, 
		StaticRead = GL_STATIC_READ, 
		StaticCopy = GL_STATIC_COPY,

		DynamicDraw = GL_DYNAMIC_DRAW,
		DynamicRead = GL_DYNAMIC_READ,
		DynamicCopy = GL_DYNAMIC_COPY,

		StreamDraw = GL_STREAM_DRAW,
		StreamRead = GL_STREAM_READ,
		StreamCopy = GL_STREAM_COPY
	};

	/// @brief Enum of all possible texture types.
	/// Values align with GLenums of same names.
	enum class TextureType
	{
		OneD                 = GL_TEXTURE_1D,
		TwoD                 = GL_TEXTURE_2D,
		ThreeD               = GL_TEXTURE_3D,
		Rectangle            = GL_TEXTURE_RECTANGLE,
		Buffer               = GL_TEXTURE_BUFFER,
		CubeMap              = GL_TEXTURE_CUBE_MAP,
		OneDArray            = GL_TEXTURE_1D_ARRAY,
		TwoDArray            = GL_TEXTURE_2D_ARRAY,
		CubeMapArray         = GL_TEXTURE_CUBE_MAP_ARRAY,
		TwoDMultisample      = GL_TEXTURE_2D_MULTISAMPLE,
		TwoDMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
	};
}

#endif
