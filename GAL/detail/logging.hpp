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
		GAL_INLINE void logErr(const char* msg)
		{
#ifndef GAL_SUPPRESS_LOGS
			std::cerr << "\u001b[31m" << "GAL_LOG: " << msg << "\u001b[37m\n";
#else
			(void)msg;
#endif
		}

		/// @brief Print a line with a yellow-colored message to cerr, along with a marker indicating this is a log message from GAL.
		/// Can be suppressed by defining GAL_SUPPRESS_LOG.
		GAL_INLINE void logWarn(const char* msg)
		{
#if defined(GAL_WARNINGS) and not defined(GAL_SUPPRESS_LOGS)
			std::cerr << "\u001b[33m" << "GAL_LOG: " << msg << "\u001b[37m\n";
#else
			(void)msg;
#endif
		}
	}
}

#endif
