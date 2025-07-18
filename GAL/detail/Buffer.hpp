#ifndef GAL_BUFFER_HPP
#define GAL_BUFFER_HPP

// TODO: implement an ImmutableBuffer class.

#include <vector>

#include "attributes.hpp"
#include "enums.hpp"
#include "GALException.hpp"
#include "ResourceTracker.hpp"
#include "types.hpp"

namespace gal
{
	namespace detail
	{
		GAL_INLINE void deleteBuffer(type::OBRBufferID id)
		{
			glDeleteBuffers(1, &id);
		}

		GAL_INLINE ResourceTracker<type::OBRBufferID, deleteBuffer> bufferTracker;
	}

	class Buffer
	{
	public:
		GAL_INLINE Buffer(BufferType type)
			: type(static_cast<GLenum>(type))
		{
			glCreateBuffers(1, &bufferID);
			detail::bufferTracker.add(bufferID);
		}

		// Forbid copying.
		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

		// Allow moving.
		Buffer(Buffer&&) noexcept = default;
		Buffer& operator=(Buffer&&) noexcept = default;

		GAL_INLINE ~Buffer()
		{
			detail::bufferTracker.remove(bufferID);
		}

		GAL_NODISCARD GAL_INLINE type::OBRBufferID getID() const noexcept { return bufferID; }

		GAL_NODISCARD GAL_INLINE BufferType getBufferType() const noexcept { return static_cast<BufferType>(type); }

		GAL_NODISCARD GAL_INLINE GLenum getGLenumBufferType() const noexcept { return type; }

		/// @brief Get the current size of this buffer. Returns OBRSizeNull if the buffer has not been allocated yet. 
		GAL_NODISCARD GAL_INLINE GLsizeiptr getSize() const noexcept { return size; }

		/// @brief Get the usage hint applied to this buffer. Returns BufferUsageHint::Null if the buffer has not been allocated yet. 
		GAL_NODISCARD GAL_INLINE BufferUsageHint getUsageHint() const noexcept { return usageHint; }

		/// @brief Query whether this buffer has been allocated or not. If not, many functions will fail.
		GAL_NODISCARD GAL_INLINE bool isAllocated() const noexcept { return allocated; }

		/// @brief Bind this buffer for use. DSA is encouraged where possible. 
		GAL_INLINE void bind() const noexcept
		{
			glBindBuffer(type, bufferID);
		}

		/// @brief Allocate given space in VRAM for this buffer with the given usage hint but don't fill it,
		/// leaving the contents undefined.
		GAL_INLINE void allocate(GLsizeiptr size, BufferUsageHint usageHint)
		{
			glNamedBufferData(bufferID, size, nullptr, static_cast<GLenum>(usageHint));

			this->size = size;
			this->usageHint = usageHint;
			allocated = true;
		}

		/// @brief Semantic sugar. Equivalent to allocate(). 
		GAL_INLINE void reallocate(GLsizeiptr size, BufferUsageHint usageHint)
		{
			allocate(size, usageHint);
		}

		/// @brief Allocate given space in VRAM for this buffer with the given usage hint and fill it with the given data.
		GAL_INLINE void allocateAndWrite(GLsizeiptr size, const void* data, BufferUsageHint usageHint)
		{
			glNamedBufferData(bufferID, size, data, static_cast<GLenum>(usageHint));

			this->size = size;
			this->usageHint = usageHint;
			allocated = true;
		}

		template<typename T>
		GAL_INLINE void allocateAndWrite(const std::vector<T>& data, BufferUsageHint usageHint)
		{
			allocateAndWrite(sizeof(T) * data.size(), data.data(), usageHint);
		}

		/// @brief Semantic sugar. Equivalent to allocateAndWrite(). 
		GAL_INLINE void reallocateAndWrite(GLsizeiptr size, const void* data, BufferUsageHint usageHint)
		{
			allocateAndWrite(size, data, usageHint);
		}

		/// @brief Update the entire contents of the buffer with the given data.
		/// You better make sure your data is the same size as the buffer! (See Buffer::getSize() for that)
		GAL_INLINE void writeAll(const void* data)
		{
			throwIfUnallocated();
			glNamedBufferSubData(bufferID, 0, size, data);
		}

		template<typename T>
		GAL_INLINE void writeAll(const std::vector<T>& data)
		{
			throwIfUnallocated();
			glNamedBufferSubData(bufferID, 0, size, data.data());
		}

		/// @brief Update a subsection of the buffer with the given data.
		/// See Buffer::writeRange() for an index-based option.
		GAL_INLINE void writeSub(GLintptr offset, GLsizeiptr size, const void* data)
		{
			throwIfUnallocated();
			glNamedBufferSubData(bufferID, offset, size, data);
		}

		/// @brief Update the data in the buffer between two indices. Make sure the size of `data` is
		/// equal to endIndex - startIndex.
		GAL_INLINE void writeRange(GLintptr startIndex, GLintptr endIndex, const void* data)
		{
			throwIfUnallocated();
			glNamedBufferSubData(bufferID, startIndex, endIndex - startIndex, data);
		}

		// TODO: Clearing and mapping functions.

		/// @brief Invalidate all contents of the buffer, leaving them undefined. 
		GAL_INLINE void invalidateAll() noexcept
		{
			glInvalidateBufferData(bufferID);
		}

		/// @brief Invalidate a subsection of the buffer, leaving it undefined.
		/// See Buffer::invalidateRange for an index-based option.
		GAL_INLINE void invalidateSub(GLintptr offset, GLsizeiptr length) noexcept
		{
			glInvalidateBufferSubData(bufferID, offset, length);
		}

		/// @brief Invalidate the buffer between two indices, leaving that area undefined.
		GAL_INLINE void invalidateRange(GLintptr startIndex, GLintptr endIndex) noexcept
		{
			glInvalidateBufferSubData(bufferID, startIndex, endIndex - startIndex);
		}

	private:
		type::OBRBufferID bufferID;
		GLenum type;
		GLsizeiptr size = type::OBRSizeNull;
		BufferUsageHint usageHint = BufferUsageHint::Null;

		bool allocated = false;

		GAL_INLINE void throwIfUnallocated() const
		{
			if (!allocated)
				detail::throwErr(ErrCode::BufferUseBeforeAllocation, "Attempted to use buffer before it was allocated");
		}
	};
}

#endif
