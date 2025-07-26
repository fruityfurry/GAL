#ifndef GAL_TEXTURE_HPP
#define GAL_TEXTURE_HPP

#include "attributes.hpp"

namespace gal
{
	namespace detail
	{
		GAL_INLINE void deleteTexture(type::GALTextureID id)
		{
			glDeleteTextures(1, &id);
		}

		GAL_INLINE ResourceTracker<type::GALTextureID, deleteTexture> textureTracker;
	}

	class Texture
	{
	public:
		GAL_INLINE Texture(TextureType type)
			: type(static_cast<GLenum>(type))
		{
			glCreateTextures(static_cast<GLenum>(type), 1, &textureID);
			detail::textureTracker.add(textureID);
		}

		// Forbid copying.
		GAL_INLINE Texture(const Texture&) = delete;
		GAL_INLINE Texture& operator=(const Texture&) = delete;

		// Allow moving.
		GAL_INLINE Texture(Texture&&) noexcept = default;
		GAL_INLINE Texture& operator=(Texture&&) noexcept = default;

		GAL_INLINE ~Texture()
		{
			detail::textureTracker.remove(textureID);
		}

		GAL_NODISCARD GAL_INLINE type::GALTextureID getID() const noexcept { return textureID; }

		GAL_NODISCARD GAL_INLINE TextureType getTextureType() const noexcept { return static_cast<TextureType>(type); }
		GAL_NODISCARD GAL_INLINE GLenum getGLEnumTextureType() const noexcept { return type; }

		GAL_NODISCARD GAL_INLINE GLsizei getWidth() const noexcept { return width; }
		GAL_NODISCARD GAL_INLINE GLsizei getHeight() const noexcept { return height; }
		GAL_NODISCARD GAL_INLINE GLsizei getDepth() const noexcept { return depth; }

		/// @brief Bind this texture for use. DSA is encouraged where possible. 
		GAL_INLINE void bind() const noexcept { glBindTexture(type, textureID); }

		/// @brief Bind this texture to the given texture unit.
		GAL_INLINE void bindTextureUnit(int unit) const noexcept { glBindTextureUnit(unit, textureID); }

		/// @brief Set a parameter of the texture. 
		GAL_INLINE void setParameter(GLenum parameterName, float val) noexcept { glTextureParameterf(textureID, parameterName, val); }
		GAL_INLINE void setParameter(GLenum parameterName, int val) noexcept { glTextureParameteri(textureID, parameterName, val); }
		GAL_INLINE void setParameter(GLenum parameterName, float* vals) noexcept { glTextureParameterfv(textureID, parameterName, vals); }
		GAL_INLINE void setParameter(GLenum parameterName, int* vals) noexcept { glTextureParameteriv(textureID, parameterName, vals); }

		/// @brief Allocate immutable storage for this texture.
		GAL_INLINE void storage(GLint mipmapLevels, GLenum internalFormat, GLsizei width, GLsizei height = 0, GLsizei depth = 0) noexcept
		{
			if (height == 0 && depth == 0)
				glTextureStorage1D(textureID, mipmapLevels, internalFormat, width);
			else if (depth == 0)
				glTextureStorage2D(textureID, mipmapLevels, internalFormat, width, height);
			else
				glTextureStorage3D(textureID, mipmapLevels, internalFormat, width, height, depth);

			this->width = width;
			this->height = height;
			this->depth = depth;
		}

		/// @brief Bind this texture, then allocate and write to mutable storage for it. 
		GAL_INLINE void imageAB(GLenum mipmapLevel, GLenum internalFormat, GLenum format, GLenum type, const void* data,
			GLsizei width, GLsizei height = 0, GLsizei depth = 0) noexcept
		{
			bind();
			imageNB(mipmapLevel, internalFormat, format, type, data, width, height, depth);
		}

		/// @brief Allocate and write to mutable storage for this texture. 
		GAL_INLINE void imageNB(GLenum mipmapLevel, GLenum internalFormat, GLenum format, GLenum type, const void* data, 
			GLsizei width, GLsizei height = 0, GLsizei depth = 0) noexcept
		{
			if (height == 0 && depth == 0)
				glTexImage1D(this->type, mipmapLevel, internalFormat, width, 0, format, type, data);
			else if (depth == 0)
				glTexImage2D(this->type, mipmapLevel, internalFormat, width, height, 0, format, type, data);
			else
				glTexImage3D(this->type, mipmapLevel, internalFormat, width, height, depth, 0, format, type, data);

			this->width = width;
			this->height = height;
			this->depth = depth;
		}

		/// @brief Binds this texture and generates its mipmap. 
		GAL_INLINE void generateMipmapAB() noexcept
		{
			bind();
			glGenerateMipmap(type);
		}

		/// @brief Generates this texture's mipmap. 
		GAL_INLINE void generateMipmapNB() noexcept
		{
			glGenerateMipmap(type);
		}

	private:
		type::GALTextureID textureID;
		GLenum type;

		GLsizei width = 0;
		GLsizei height = 0;
		GLsizei depth = 0;
	};
}

#endif
