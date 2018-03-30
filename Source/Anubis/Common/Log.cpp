#include "../../../Include/Anubis/Common/Log.hpp"

using namespace std::chrono;
using namespace Anubis::Common;

Log Log::fInstance;

/******************************************************************************/
Log::Log() : fMaxLevel(Levels::Debug), fWriteToCLI(true), fIsExecuting(true)
{
  /* Start the writer thread. */
  fThread = std::thread(&Log::threadEntry, this);
}

/******************************************************************************/
Log::~Log()
{
  /* Stop the write thread. */
  fIsExecuting = false;

  /* Check if the thread can join, if it cant it will simply lock up the app. */
  if(fThread.joinable())
  {
    /* Wait for the write thread to join. */
    fThread.join();
  }
}

/******************************************************************************/
Log & Log::get()
{
  return fInstance;
}

/******************************************************************************/
void Log::threadEntry()
{
  /* The message to be written to the log. */
  Entry curMsg;

  /* Keep looping while executing. */
  while(fIsExecuting)
  {
    /* Check if the queue is empty. */
    if(fMessages.isEmpty())
    {
      /* The result of the conditional variable wait. */
      std::cv_status result = std::cv_status::timeout;

      /* Check if we should keep waiting for a message to be written. */
      while(fIsExecuting && result == std::cv_status::timeout)
      {
        /* Lock the waiting for message mutex. */
        std::unique_lock<std::mutex> lock(fMsgReadyMutex);

        /* Wait for 100ms. */
        result = fMsgReadyCV.wait_for(lock, milliseconds(100));
      }
    }

    /* Pop a message from the queue. */
    if(fMessages.pop(curMsg))
    {
      /* Check if the message should be written to the cli. */
      if(fWriteToCLI)
      {
        /* For now, just write the message to cout. */
        (curMsg.fLevel <= Levels::Error ? std::cerr : std::cout)
            << curMsg.fMessage << std::endl;
      }
    }
  }

  /* Flush the remaining messages. */
  while(fMessages.pop(curMsg))
  {
    /* Check if the message should be written to the cli. */
    if(fWriteToCLI)
    {
      /* For now, just write the message to cout. */
      (curMsg.fLevel <= Levels::Error ? std::cerr : std::cout)
          << curMsg.fMessage << std::endl;
    }
  }
}

/******************************************************************************/
void Log::setMaxLevel(Levels max)
{
  /* Set the max log level. */
  fMaxLevel = max;
}

/******************************************************************************/
void Log::write(Levels level, const std::string & msg)
{
  /* Check if the message must be logged. */
  if(level > fMaxLevel)
  {
    /* Nothing left to do. */
    return;
  }

  /* Push the message on the queue. */
  fMessages.push(Entry(level, msg));

  /* Indicate that a message arrived. */
  fMsgReadyCV.notify_all();
}
