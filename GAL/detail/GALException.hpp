#ifndef GAL_GALEXCEPTION_HPP
#define GAL_GALEXCEPTION_HPP

#include <exception>

#include "attributes.hpp"
#include "logging.hpp"

namespace gal
{
	/// @brief Class for exceptions in GAL. This should be thrown when an expected error occurs inside GAL. Also contains
	/// an ErrCode (see ErrCode.hpp for that enum).
	class GALException : public std::exception
	{
	public:
		const ErrCode errCode;

		GAL_EXPLICIT GAL_INLINE GALException(ErrCode errCode, const char* msg)
			: errCode(errCode), msg(msg) {}

		GAL_INLINE const char* what() const noexcept override { return msg; }

	private:
		const char* msg;
	};

	namespace detail
	{
		/// @brief Log an error and immediately throw it.
		GAL_INLINE void throwErr(ErrCode errCode, const char* msg)
		{
			detail::logErr(msg);
			throw GALException(errCode, msg);
		}
	}
}

#endif
