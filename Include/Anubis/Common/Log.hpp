#ifndef ANUBIS_COMMON_LOG_HPP
#define ANUBIS_COMMON_LOG_HPP

#include "Misc.hpp"

/** Extract only the file name from the full file path. */
#define ANUBIS_FILENAME (strrchr(__FILE__, ANUBIS_DIR_SEPERATOR) ? \
  strrchr(__FILE__, ANUBIS_DIR_SEPERATOR) + 1 : __FILE__)

/** Write the general log message. */
#define ANUBIS_WRITE_LOG(level, levelStr, msg)  \
{\
  std::stringstream ss; \
  ss << levelStr << " | " \
     << Anubis::Common::Timer::nsSinceEpoch() << " | " \
     << std::this_thread::get_id() << " | " \
     << ANUBIS_FILENAME << " | " \
     << __LINE__ << " | " \
     << __PRETTY_FUNCTION__ << " | " \
     << msg; \
     std::cout << ss.str() << std::endl; \
}

#define ANUBIS_LOG_FATAL_ERROR(msg)   ANUBIS_WRITE_LOG(0, "FATAL ERROR", msg)
#define ANUBIS_LOG_ERROR(msg)         ANUBIS_WRITE_LOG(0, "ERROR", msg)
#define ANUBIS_LOG_WARN(msg)          ANUBIS_WRITE_LOG(0, "WARN", msg)

#define ANUBIS_LOG_INFO(msg)          ANUBIS_WRITE_LOG(0, "INFO", msg)
#define ANUBIS_LOG_DEBUG(msg)         ANUBIS_WRITE_LOG(0, "DEBUG", msg)

namespace Anubis
{
  namespace Common
  {
    class Log
    {

    };
  }
}

#endif /* ANUBIS_COMMON_LOG_HPP */
