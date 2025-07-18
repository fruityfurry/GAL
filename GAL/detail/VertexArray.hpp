#ifndef GAL_VERTEX_ARRAY_HPP
#define GAL_VERTEX_ARRAY_HPP

namespace gal
{
	namespace detail
	{
		GAL_INLINE void deleteVertexArray(type::OBRVertexArrayID id)
		{
			glDeleteVertexArrays(1, &id);
		}

		GAL_INLINE ResourceTracker<type::OBRVertexArrayID, deleteVertexArray> vertexArrayTracker;
	}

	class VertexArray
	{
	public:
		GAL_INLINE VertexArray()
		{
			glCreateVertexArrays(1, &vertexArrayID);
			detail::vertexArrayTracker.add(vertexArrayID);
		}

		// Forbid copying.
		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		// Allow moving.
		VertexArray(VertexArray&&) noexcept = default;
		VertexArray& operator=(VertexArray&&) noexcept = default;

		GAL_INLINE ~VertexArray()
		{
			detail::vertexArrayTracker.remove(vertexArrayID);
		}

		GAL_NODISCARD GAL_INLINE type::OBRVertexArrayID getID() const noexcept { return vertexArrayID; }

		/// @brief Get a pointer to the vertex buffer at the given binding index.
		/// Returns nullptr if no vertex buffer has been bound.
		GAL_NODISCARD GAL_INLINE Buffer* getVertexBuffer(GLuint bindingIndex) const
		{
			checkBindingIndex(bindingIndex);

			return vertexBuffers[bindingIndex];
		}

		/// @brief Get a reference to the vertex buffer at the given binding index.
		/// Throws an error if no buffer has been bound, so the regular getVertexBuffer function is recommended.
		GAL_NODISCARD GAL_INLINE Buffer& getVertexBufferRef(GLuint bindingIndex)
		{
			checkBindingIndex(bindingIndex);

			Buffer* buffer = vertexBuffers[bindingIndex];

			if (buffer == nullptr)
				detail::throwErr(ErrCode::GotNullBuffer, "Attempted to get a null vertex buffer.");

			return *buffer;
		}

		/// @brief Get a pointer to this VAO's element buffer.
		/// Returns nullptr if no element buffer has been bound.
		GAL_NODISCARD GAL_INLINE Buffer* getElementBuffer() const noexcept { return elementBuffer; }

		/// @brief Get a reference to this VAO's element buffer.
		/// Throws an error if no buffer has been bound, so the regular getElementBuffer function is recommended.
		GAL_NODISCARD GAL_INLINE Buffer& getElementBufferRef() const
		{
			Buffer* buffer = elementBuffer;

			if (buffer == nullptr)
				detail::throwErr(ErrCode::GotNullBuffer, "Attempted to get a null element buffer.");

			return *buffer;
		}

		GAL_NODISCARD GAL_INLINE GLenum getElementBufferIndexType() const
		{
			if (elementBuffer == nullptr)
				detail::throwErr(ErrCode::GotNullBuffer, "Attempted to get the idnex type of a null element buffer.");

			return elementBufferIndexType;
		}

		/// @brief Bind a buffer to be this VAO's vertex buffer for the given index.
		/// Offset and stride define the position and size of data in the buffer.
		/// Throws an error if index > GL_MAX_VERTEX_ATTRIB_BINDINGS - 1 (call obr::glParams::getMaxVertexAttribBindings() to query this).
		GAL_INLINE void bindVertexBuffer(Buffer* buffer, GLuint bindingIndex, GLintptr offset, GLsizei stride)
		{
			checkBindingIndex(bindingIndex);

			glVertexArrayVertexBuffer(vertexArrayID, bindingIndex, buffer->getID(), offset, stride);
			vertexBuffers[bindingIndex] = buffer;
		}

		GAL_INLINE void bindVertexBuffer(Buffer& buffer, GLuint bindingIndex, GLintptr offset, GLsizei stride)
		{
			bindVertexBuffer(&buffer, bindingIndex, offset, stride);
		}

		/// @brief Unbind the vertex buffer bound to the given binding index. 
		GAL_INLINE void unbindVertexBuffer(GLuint bindingIndex)
		{
			if (bindingIndex > detail::maxVertexAttribBindings - 1)
				return;

			glVertexArrayVertexBuffer(vertexArrayID, bindingIndex, 0, 0, 0);
			vertexBuffers[bindingIndex] = nullptr;
		}

		/// @brief Bind a buffer to be this VAO's element buffer. Subsequent function calls needing an element buffer will use this one.
		GAL_INLINE void bindElementBuffer(Buffer* buffer, GLenum indexType)
		{
			glVertexArrayElementBuffer(vertexArrayID, buffer->getID());
			elementBuffer = buffer;
			elementBufferIndexType = indexType;
		}

		GAL_INLINE void bindElementBuffer(Buffer& buffer, GLenum indexType)
		{
			bindElementBuffer(&buffer, indexType);
		}

		GAL_INLINE void unbindElementBuffer() noexcept
		{
			glVertexArrayElementBuffer(vertexArrayID, 0);
			elementBuffer = nullptr;
		}

		/// @brief Bind this VAO for use. DSA is encouraged where possible. 
		GAL_INLINE void bind() const noexcept
		{
			glBindVertexArray(vertexArrayID);
		}

		/// @brief Define a new vertex attribute using data from the VBO at bindingIndex. 
		GAL_INLINE void newVertexAttribute(GLuint attributeIndex, GLuint bindingIndex, GLint size,
			GLenum type, GLboolean normalized, GLuint relativeOffset)
		{
			checkAttributeIndex(attributeIndex);

			glEnableVertexArrayAttrib(vertexArrayID, attributeIndex);
			glVertexArrayAttribFormat(vertexArrayID, attributeIndex, size, type, normalized, relativeOffset);
			glVertexArrayAttribBinding(vertexArrayID, attributeIndex, bindingIndex);
		}

		// TODO: Instanced draw and such.

		/// @brief Binds this VAO and draws it.
		/// You should use drawUnbound() if you have already bound the VAO yourself. 
		GAL_INLINE void draw(GLenum mode, GLint first, GLsizei count) const noexcept
		{
			bind();
			glDrawArrays(mode, first, count);
		}

		/// @brief Call this only if the correct VAO is already bound.
		/// See draw() for the version that binds this VAO automatically.
		GAL_INLINE void drawUnbound(GLenum mode, GLint first, GLsizei count) const noexcept
		{
			glDrawArrays(mode, first, count);
		}

		/// @brief Binds this VAO and draws it using its EBO. 
		/// You should use drawElementsUnbound() if you have already bound the VAO yourself.
		GAL_INLINE void drawElements(GLenum mode, GLintptr offset, GLsizei count) const noexcept
		{
			bind();
			glDrawElements(mode, count, elementBufferIndexType, reinterpret_cast<void*>(offset));
		}

		/// @brief Call this only if the correct VAO is already bound.
		/// See draw() for the version that binds this VAO automatically.
		GAL_INLINE void drawElementsUnbound(GLenum mode, GLintptr offset, GLsizei count) const noexcept
		{
			glDrawElements(mode, count, elementBufferIndexType, reinterpret_cast<void*>(offset));
		}

	private:
		type::OBRVertexArrayID vertexArrayID;
		std::vector<Buffer*> vertexBuffers{ detail::maxVertexAttribBindings, nullptr };
		Buffer* elementBuffer = nullptr;
		GLenum elementBufferIndexType = 0;

		GAL_STATIC GAL_INLINE void checkBindingIndex(GLuint bindingIndex)
		{
			if (bindingIndex > detail::maxVertexAttribBindings - 1)
				detail::throwErr(ErrCode::VertexBufferIndexOutOfRange,
					"Attempted to bind a vertex buffer to an index that was out of range (> GL_MAX_VERTEX_ATTRIB_BINDINGS - 1).");
		}

		GAL_STATIC GAL_INLINE void checkAttributeIndex(GLuint attributeIndex)
		{
			if (attributeIndex > detail::maxVertexAttribs - 1)
				detail::throwErr(ErrCode::VertexAttributeIndexOutOfRange,
					"Attempted to add a vertex attribute with an index that was out of range (> GL_MAX_VERTEX_ATTRIBS - 1).");
		}
	};
}

#endif
