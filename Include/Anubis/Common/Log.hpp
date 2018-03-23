#ifndef ANUBIS_COMMON_LOG_HPP
#define ANUBIS_COMMON_LOG_HPP

#include "Misc.hpp"
#include "CircularQueue.hpp"

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
     Anubis::Common::Log::get().write(level, ss.str()); \
}

#define ANUBIS_LOG_FATAL_ERROR(msg) \
  ANUBIS_WRITE_LOG(Anubis::Common::Log::Levels::FatalError, "FATAL ERROR", msg)

#define ANUBIS_LOG_ERROR(msg) \
  ANUBIS_WRITE_LOG(Anubis::Common::Log::Levels::Error, "ERROR", msg)

#define ANUBIS_LOG_WARN(msg) \
  ANUBIS_WRITE_LOG(Anubis::Common::Log::Levels::Warning, "WARN", msg)

#define ANUBIS_LOG_INFO(msg) \
{\
  std::stringstream ss; \
  ss << " INFO | " << msg; \
     Anubis::Common::Log::get().write(Anubis::Common::Log::Levels::Info, \
                                      ss.str()); \
}

//  ANUBIS_WRITE_LOG(Anubis::Common::Log::Levels::Info, "INFO", msg)

#define ANUBIS_LOG_DEBUG(msg) \
  ANUBIS_WRITE_LOG(Anubis::Common::Log::Levels::Debug, "DEBUG", msg)

namespace Anubis
{
  namespace Common
  {
    class Log final
    {
    public:

      /*********************************************************************//**
       * The various log levels that are available to the engine. The engine
       * must be compile to support all log levels for them to be available.
       ************************************************************************/
      enum Levels : int
      {
        /** An unrecoverable error or exception that occured. */
        FatalError = 0,

        /** A recoverable error or exception that occured. */
        Error,

        /** Something seems fishy, but may or may not cause problems. */
        Warning,

        /** Usable information the application user. */
        Info,

        /** Usable information for the application developer. */
        Debug
      };

      /*********************************************************************//**
       * Retrieve the singleton instance of the class.
       *
       * @return  The singleton instance of the class.
       ************************************************************************/
      static Log & get();

      /*********************************************************************//**
       * Set the maximum message level to record (the maximum level is
       * inclusive). I.e. the smallest max level is FatalError meaning that
       * there is no way of blocking Fatal Errors from being logged. The scale
       * for min to max is:
       *
       *  Fatal Error (Min)
       *  Error
       *  Warning
       *  Info
       *  Log (Max)
       *
       * So to log:
       *  Fatal Errors: setMaxLevel(FatalError)
       *  Fatal Errors, Errors: setMaxLevel(Error)
       *  Fatal Errors, Errors, Warnings: setMaxLevel(Warning)
       *  etc ...
       *
       * @param max The maximum level of message logging.
       ************************************************************************/
      void setMaxLevel(Levels max);

      /*********************************************************************//**
       * Queue a log message for writting to the log file and console.
       *
       * @param msg The message to be queued.
       ************************************************************************/
      void write(Levels level, const std::string & msg);

    private:

      struct Entry
      {
        Levels fLevel;
        std::string fMessage;

        Entry() {}
        Entry(Levels level, const std::string & msg) : fLevel(level),
          fMessage(msg) {}
      };

      /** The singleton instance of the log class. */
      static Log fInstance;

      /** The maximum log level. */
      std::atomic_int fMaxLevel;

      /** Indicate whether we should write to the comman line interface (CLI)
       * or not. */
      std::atomic_bool fWriteToCLI;

      /** Flag to track if the thread should keep runing. */
      std::atomic_bool fIsExecuting;

      /** The mutex to control access to the message queue. */
      std::mutex fMsgMutex;

      /** The mutex to indicate that no messages are ready. */
      std::mutex fMsgReadyMutex;

      /** The condition variable to track whether a log message has been added
       * to the queue. */
      std::condition_variable fMsgReadyCV;

      /** The thread to write the messages in the queue to file / cout etc. */
      std::thread fThread;

      /** The message queue. */
      CircularQueue<Entry, 4096> fMessages;

      /** The entry point for the write thread. */
      void threadEntry();

      Log(const Log &) = delete;
      Log & operator = (const Log &) = delete;

      Log();
      ~Log();
    };
  }
}

#endif /* ANUBIS_COMMON_LOG_HPP */
