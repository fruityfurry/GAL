#ifndef GAL_LOGGING_HPP
#define GAL_LOGGING_HPP

#include "attributes.hpp"
#include <iostream>

namespace gal
{
	/// @brief Internal functions used by GAL. Not intended for your use!
	namespace detail
	{
		/// @brief Print a line with a red-colored message to cerr, along with a marker indicating this is a log message from GAL.
		/// Can be suppressed by defining GAL_SUPPRESS_LOG.
		template<typename T>
		GAL_INLINE void logErr(T obj)
		{
#ifndef GAL_SUPPRESS_LOGS
			std::cerr << "\u001b[31m" << "GAL_LOG: " << obj << "\u001b[37m\n";
#else
			(void)msg;
#endif
		}

		/// @brief Print a message with a red-colored message to cerr, but with no marker and no newline.
		/// Can be suppressed by defining GAL_SUPPRESS_LOG.
		template<typename T>
		GAL_INLINE void logErrPart(T obj)
		{
#ifndef GAL_SUPPRESS_LOGS
			std::cerr << "\u001b[31m" << obj << "\u001b[37m";
#else
			(void)msg;
#endif
		}
	}
}

#endif
