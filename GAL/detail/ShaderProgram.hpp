#ifndef GAL_SHADER_PROGRAM_HPP
#define GAL_SHADER_PROGRAM_HPP

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "attributes.hpp"
#include "ResourceTracker.hpp"
#include "types.hpp"

namespace gal
{
	namespace detail
	{
		GAL_INLINE void deleteShaderProgram(type::GALShaderProgramID id)
		{
			glDeleteProgram(id);
		}

		GAL_INLINE ResourceTracker<type::GALShaderProgramID, deleteShaderProgram> shaderProgramTracker;
	}

	/// @brief GAL shader program class. Access its program ID via its programID field.
	/// You add shaders one by one to the program, linking once you have added them all.
	/// Member functions other than those to add a shader do not work until you have called ShaderProgram::link() and will throw an error.
	class ShaderProgram
	{
	public:
		/// @brief Create the program, ready and raring to have shaders added to it. 
		GAL_INLINE ShaderProgram()
			: programID(glCreateProgram())
		{
			detail::shaderProgramTracker.add(programID);
		}

		// Forbid copying.
		GAL_INLINE ShaderProgram(const ShaderProgram&) = delete;
		GAL_INLINE ShaderProgram& operator=(const ShaderProgram&) = delete;

		// Allow moving.
		GAL_INLINE ShaderProgram(ShaderProgram&&) noexcept = default;
		GAL_INLINE ShaderProgram& operator=(ShaderProgram&&) noexcept = default;

		GAL_INLINE ~ShaderProgram()
		{
			deleteAllShaders();
			detail::shaderProgramTracker.remove(programID);
		}

		/// @brief Add a shader of the given type to the program, reading the source code from the given filepath. 
		GAL_INLINE ShaderProgram& addShaderFromFile(const std::string& path, ShaderType type)
		{
			std::ifstream file(path);
			if (!file.is_open())
				detail::throwErr(ErrCode::ShaderReadFailed, "Could not open shader file.");

			std::stringstream buffer;
			buffer << file.rdbuf();
			addShaderFromSource(buffer.str(), type);

			return *this;
		}

		/// @brief Add a shader of the given type to the program from the given source code string.
		GAL_INLINE ShaderProgram& addShaderFromSource(const std::string& source, ShaderType type)
		{
			if (linked)
				detail::throwErr(ErrCode::AddShaderAfterLinking, "Attempted to add shader to program after the program had already been linked.");

			type::GALIDType shaderID = glCreateShader(static_cast<GLenum>(type));
			const char* sourceCStr = source.c_str();
			glShaderSource(shaderID, 1, &sourceCStr, nullptr);
			glCompileShader(shaderID);

			GLint success;
			char infoLog[512];
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
				detail::logErr("Failed to compile shader. Error log from OpenGL to follow.");
				detail::throwErr(ErrCode::ShaderCompFailed, infoLog);
			}

			glAttachShader(programID, shaderID);
			shaderIDs.emplace_back(shaderID);

			return *this;
		}

		GAL_NODISCARD GAL_INLINE type::GALShaderProgramID getID() const noexcept
		{
			return programID;
		}

		/// @brief Finalize your shader program and link it. You may not add any more shaders to the program after calling this. 
		GAL_INLINE void link()
		{
			if (linked)
				detail::throwErr(ErrCode::ShaderProgramDoubleLink, "Attempted to link a shader even though it was already linked.");

			glLinkProgram(programID);

			GLint success;
			char infoLog[512];
			glGetProgramiv(programID, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(programID, 512, nullptr, infoLog);
				detail::logErr("Failed to link shader program. Error log from OpenGL to follow.");
				detail::throwErr(ErrCode::ShaderProgramLinkFailed, infoLog);
			}

			linked = true;
			deleteAllShaders();
		}

		/// @brief Same as link() but doesn't throw errors. Instead, returns true if successful and false if unsuccessful.
		GAL_INLINE bool tryLink() noexcept
		{
			if (linked)
				return false;

			glLinkProgram(programID);

			GLint success;
			glGetProgramiv(programID, GL_LINK_STATUS, &success);
			if (!success)
				return false;

			linked = true;
			deleteAllShaders();

			return true;
		}

		/// @brief Query whether or not the program has been linked. 
		GAL_INLINE bool isLinked() const noexcept
		{
			return linked;
		}

		/// @brief Use the shader program so that subsequent OpenGL calls will use this shader program. 
		GAL_INLINE void use() const
		{
			if (!linked)
				detail::throwErr(ErrCode::ShaderProgramUseBeforeLinking, "Attempted to use shader before linking.");

			glUseProgram(programID);
		}

		// ========== float uniform setters ==========

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, float val) const
		{
			glProgramUniform1f(programID, getUniformLocation(name), val);
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::vec1& vec) const
		{
			glProgramUniform1fv(programID, getUniformLocation(name), 1, glm::value_ptr(vec));
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, float val1, float val2) const
		{
			glProgramUniform2f(programID, getUniformLocation(name), val1, val2);
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::vec2& vec) const
		{
			glProgramUniform2fv(programID, getUniformLocation(name), 1, glm::value_ptr(vec));
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, float val1, float val2, float val3) const
		{
			glProgramUniform3f(programID, getUniformLocation(name), val1, val2, val3);
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::vec3& vec) const
		{
			glProgramUniform3fv(programID, getUniformLocation(name), 1, glm::value_ptr(vec));
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, float val1, float val2, float val3, float val4) const
		{
			glProgramUniform4f(programID, getUniformLocation(name), val1, val2, val3, val4);
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::vec4& vec) const
		{
			glProgramUniform4fv(programID, getUniformLocation(name), 1, glm::value_ptr(vec));
			return *this;
		}

		// ========== int uniform setters ==========

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, bool val) const
		{
			glProgramUniform1i(programID, getUniformLocation(name), static_cast<int>(val));
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, int val) const
		{
			glProgramUniform1i(programID, getUniformLocation(name), val);
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::ivec1& vec) const
		{
			glProgramUniform1iv(programID, getUniformLocation(name), 1, glm::value_ptr(vec));
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, int val1, int val2) const
		{
			glProgramUniform2i(programID, getUniformLocation(name), val1, val2);
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::ivec2& vec) const
		{
			glProgramUniform2iv(programID, getUniformLocation(name), 1, glm::value_ptr(vec));
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, int val1, int val2, int val3) const
		{
			glProgramUniform3i(programID, getUniformLocation(name), val1, val2, val3);
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::ivec3& vec) const
		{
			glProgramUniform3iv(programID, getUniformLocation(name), 1, glm::value_ptr(vec));
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, int val1, int val2, int val3, int val4) const
		{
			glProgramUniform4i(programID, getUniformLocation(name), val1, val2, val3, val4);
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::ivec4& vec) const
		{
			glProgramUniform4iv(programID, getUniformLocation(name), 1, glm::value_ptr(vec));
			return *this;
		}

		// ========== uint uniform setters ==========

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, unsigned int val) const
		{
			glProgramUniform1ui(programID, getUniformLocation(name), val);
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::uvec1& vec) const
		{
			glProgramUniform1uiv(programID, getUniformLocation(name), 1, glm::value_ptr(vec));
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, unsigned int val1, unsigned int val2) const
		{
			glProgramUniform2ui(programID, getUniformLocation(name), val1, val2);
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::uvec2& vec) const
		{
			glProgramUniform2uiv(programID, getUniformLocation(name), 1, glm::value_ptr(vec));
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, unsigned int val1, unsigned int val2, unsigned int val3) const
		{
			glProgramUniform3ui(programID, getUniformLocation(name), val1, val2, val3);
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::uvec3& vec) const
		{
			glProgramUniform3uiv(programID, getUniformLocation(name), 1, glm::value_ptr(vec));
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, unsigned int val1, unsigned int val2, unsigned int val3, unsigned int val4) const
		{
			glProgramUniform4ui(programID, getUniformLocation(name), val1, val2, val3, val4);
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::uvec4& vec) const
		{
			glProgramUniform4uiv(programID, getUniformLocation(name), 1, glm::value_ptr(vec));
			return *this;
		}


		// ========== matrix uniform setters ==========

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::mat2& mat, bool transpose = false) const
		{
			glProgramUniformMatrix2fv(programID, getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::mat3& mat, bool transpose = false) const
		{
			glProgramUniformMatrix3fv(programID, getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::mat4& mat, bool transpose = false) const
		{
			glProgramUniformMatrix4fv(programID, getUniformLocation(name), 1, transpose, glm::value_ptr(mat));
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::mat2x3& mat, bool transpose = false) const
		{
			glProgramUniformMatrix2x3fv(programID, getUniformLocation(name), 1, transpose, glm::value_ptr(mat));
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::mat3x2& mat, bool transpose = false) const
		{
			glProgramUniformMatrix3x2fv(programID, getUniformLocation(name), 1, transpose, glm::value_ptr(mat));
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::mat2x4& mat, bool transpose = false) const
		{
			glProgramUniformMatrix2x4fv(programID, getUniformLocation(name), 1, transpose, glm::value_ptr(mat));
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::mat4x2& mat, bool transpose = false) const
		{
			glProgramUniformMatrix4x2fv(programID, getUniformLocation(name), 1, transpose, glm::value_ptr(mat));
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::mat3x4& mat, bool transpose = false) const
		{
			glProgramUniformMatrix3x4fv(programID, getUniformLocation(name), 1, transpose, glm::value_ptr(mat));
			return *this;
		}

		GAL_INLINE const ShaderProgram& setUniform(const std::string& name, const glm::mat4x3& mat, bool transpose = false) const
		{
			glProgramUniformMatrix4x3fv(programID, getUniformLocation(name), 1, transpose, glm::value_ptr(mat));
			return *this;
		}

	private:
		type::GALShaderProgramID programID;
		bool linked = false;
		std::vector<type::GALIDType> shaderIDs; // List of shader IDs that haven't been deleted yet.
		mutable std::unordered_map<std::string, int> uniformLocationss; // Cached locations of uniforms.

		GAL_INLINE void deleteAllShaders()
		{
			for (type::GALIDType id : shaderIDs)
			{
				glDetachShader(programID, id);
				glDeleteShader(id);
			}

			shaderIDs.clear();
		}

		GAL_INLINE GLint getUniformLocation(const std::string& name) const
		{
			if (!linked)
				detail::throwErr(ErrCode::UniformSetBeforeLinking, "Attempted to set shader uniform before linking.");

			if (auto it = uniformLocationss.find(name); it != uniformLocationss.end())
				return it->second;

			const GLint loc = glGetUniformLocation(programID, name.c_str());
			if (loc == -1)
				detail::throwErr(ErrCode::NonExistentShaderUniform, "Attempted to set non-existent shader uniform.");

			uniformLocationss[name] = loc;
			return loc;
		}
	};
}

#endif