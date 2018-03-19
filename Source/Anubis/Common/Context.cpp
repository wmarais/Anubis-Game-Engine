#include "../../../Include/Anubis/Common.hpp"

using namespace Anubis::Common;

/******************************************************************************/
Context::Context(const std::chrono::nanoseconds & updateRate) :
  fIsExecuting(false), kUpdateRate(updateRate)
{
  /* Start the physics thread. */
  fThread = std::thread(&Context::threadEntry, this);

  /* Set the executing flag here since this point wont be reached if an
   * exception is thrown. */
  fIsExecuting = true;
}

/******************************************************************************/
Context::~Context()
{
  /* Stop the main thread. */
  if(fIsExecuting)
  {
    /* Set the executing flag to false. */
    fIsExecuting = false;

    /* Wait for the thread to join. */
    fThread.join();
  }
}

/******************************************************************************/
void Context::threadEntry()
{
  try
  {
    /* The time diffirence for the very first pass. */
    std::chrono::nanoseconds updateTime(0);

    /* Keep updating the physics while the thread is executing. */
    while(fIsExecuting)
    {
      /* Record the start time. */
      auto startTime = std::chrono::high_resolution_clock::now();

      /* Update the context. */
      update(static_cast<float>(updateTime.count())/1000000000.0f);

      /* Calculate the time diffirence. */
      updateTime = std::chrono::high_resolution_clock::now() - startTime;

      /* Check how long to sleep for. */
      std::chrono::nanoseconds sleepTime = kUpdateRate - updateTime;

      /* Check if the context should sleep. */
      if(sleepTime > std::chrono::nanoseconds(0))
      {
        /* Sleep the thread. */
        std::this_thread::sleep_for(sleepTime);
      }

      /* Refresh the time diffirence to ensure the best possible time
       * accuracy for updates. */
      updateTime = std::chrono::high_resolution_clock::now() - startTime;
    }
  }
  /* Catch any exceptions that are thrown. */
  catch(...)
  {
    /* Save the exception. */
    fException = std::current_exception();

    /* Stop the thread. */
    fIsExecuting = false;
  }
}
