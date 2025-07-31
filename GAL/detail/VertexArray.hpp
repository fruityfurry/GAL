#ifndef GAL_VERTEX_ARRAY_HPP
#define GAL_VERTEX_ARRAY_HPP

namespace gal
{
	namespace detail
	{
		GAL_INLINE void deleteVertexArray(type::GALVertexArrayID id)
		{
			glDeleteVertexArrays(1, &id);
		}

		GAL_INLINE ResourceTracker<type::GALVertexArrayID, deleteVertexArray> vertexArrayTracker;
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
		GAL_INLINE VertexArray(const VertexArray&) = delete;
		GAL_INLINE VertexArray& operator=(const VertexArray&) = delete;

		// Allow moving.
		GAL_INLINE VertexArray(VertexArray&&) noexcept = default;
		GAL_INLINE VertexArray& operator=(VertexArray&&) noexcept = default;

		GAL_INLINE ~VertexArray()
		{
			detail::vertexArrayTracker.remove(vertexArrayID);
		}

		GAL_NODISCARD GAL_INLINE type::GALVertexArrayID getID() const noexcept { return vertexArrayID; }

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

		/// @brief Get the type of the indices in the currently bound element buffer.
		/// Throws if no element buffer has been bound.
		GAL_NODISCARD GAL_INLINE GLenum getElementBufferIndexType() const
		{
			if (elementBuffer == nullptr)
				detail::throwErr(ErrCode::GotNullBuffer, "Attempted to get the idnex type of a null element buffer.");

			return elementBufferIndexType;
		}

		/// @brief Get the polygon mode set by setDrawSettings(). Throws if unset. 
		GAL_NODISCARD GAL_INLINE GLenum getDrawPolygonMode() const
		{
			checkDrawSettingSet();
			return drawPolygonMode;
		}

		/// @brief Get the offset/first value set by setDrawSettings(). Throws if unset. 
		GAL_NODISCARD GAL_INLINE GLintptr getDrawOffsetFirst() const
		{
			checkDrawSettingSet();
			return drawOffsetFirst;
		}

		/// @brief Get the polygon mode set by setDrawSettings(). Throws if unset. 
		GAL_NODISCARD GAL_INLINE GLsizei getDrawCount() const
		{
			checkDrawSettingSet();
			return drawCount;
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

		/// @brief Set the settings that will be used for drawing when calling drawAB() or drawNB().
		/// The offsetFirst parameter corresponds either to the indices parameter of glDDrawElements
		/// or the first parameter of glDrawArrays.
		GAL_INLINE void setDrawSettings(GLenum polygonMode, GLintptr offsetFirst, GLsizei count) noexcept
		{
			drawPolygonMode = polygonMode;
			drawOffsetFirst = offsetFirst;
			drawCount = count;

			drawSettingsSet = true;
		}

		/// @brief Bind this VAO and draws it using the draw settings set by setDrawSettings(). Throws if unset.
		/// Picks between glDrawArrays and glDrawElements depending on whether or not an element buffer is bound.
		GAL_INLINE void drawAB() const
		{
			checkDrawSettingSet();

			if (elementBuffer != nullptr)
				drawElementsAB(drawPolygonMode, drawOffsetFirst, drawCount);
			else
				drawArraysAB(drawPolygonMode, static_cast<GLint>(drawOffsetFirst), drawCount);
		}

		/// @brief Draws this VAO using the draw settings set by setDrawSettings(). Throws if unset.
		/// Picks between glDrawArrays and glDrawElements depending on whether or not an element buffer is bound.
		GAL_INLINE void drawNB() const
		{
			checkDrawSettingSet();

			if (elementBuffer != nullptr)
				drawElementsNB(drawPolygonMode, drawOffsetFirst, drawCount);
			else
				drawArraysNB(drawPolygonMode, static_cast<GLint>(drawOffsetFirst), drawCount);
		}

		/// @brief Binds this VAO and draws it.
		GAL_INLINE void drawArraysAB(GLenum polygonMode, GLint first, GLsizei count) const noexcept
		{
			bind();
			glDrawArrays(polygonMode, first, count);
		}

		/// @brief Draws this VAO.
		GAL_INLINE void drawArraysNB(GLenum polygonMode, GLint first, GLsizei count) const noexcept
		{
			glDrawArrays(polygonMode, first, count);
		}

		/// @brief Binds this VAO and draws it using its EBO.
		GAL_INLINE void drawElementsAB(GLenum polygonMode, GLintptr offset, GLsizei count) const noexcept
		{
			bind();
			glDrawElements(polygonMode, count, elementBufferIndexType, reinterpret_cast<void*>(offset));
		}

		/// @brief Draws this VAO using its EBO.
		GAL_INLINE void drawElementsNB(GLenum polygonMode, GLintptr offset, GLsizei count) const noexcept
		{
			glDrawElements(polygonMode, count, elementBufferIndexType, reinterpret_cast<void*>(offset));
		}

	private:
		type::GALVertexArrayID vertexArrayID;
		std::vector<Buffer*> vertexBuffers{ detail::maxVertexAttribBindings, nullptr };
		Buffer* elementBuffer = nullptr;
		GLenum elementBufferIndexType = 0;

		bool drawSettingsSet = false;

		GLenum drawPolygonMode = 0;
		GLintptr drawOffsetFirst = 0;
		GLsizei drawCount = 0;

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

		GAL_INLINE void checkDrawSettingSet() const
		{
			if (!drawSettingsSet)
				detail::throwErr(ErrCode::DrawSettingsUnset,
					"Attempted to perform an operation requiring VAO draw settings to be set, without them being set.");
		}
	};
}

#endif
