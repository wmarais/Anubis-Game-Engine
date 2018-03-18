#include "../../../Include/Anubis/Physics/PhysicsContext.hpp"

using namespace Anubis::Physics;

/******************************************************************************/
PhysicsContext::PhysicsContext(float tickRate) : kTickRate(tickRate),
  fIsExecuting(true), fException(nullptr)
{

}

/******************************************************************************/
bool PhysicsContext::sync()
{
  /* Check if the back scene is in use by checking if the access mutex can
   * be locked. */
  if(!fFrontSceneMutex.try_lock())
  {
    /* If the lock failed, then there is nothing to sync, just return now. */
    return false;
  }

  /* Sync the front scene to the back scene. */
  fFrontScene->sync(fBackScene.get());

  /* Unlock the mutex. The only time that this will not be called is if an
   * exception occur during the synchronisation. In that case the whole program
   * is expected to terminate. Thus no special RII precations are taken. */
  fFrontSceneMutex.unlock();

  /* Return true to indicate that the back scene was synchronised to the front
   * scene. */
  return true;
}

/******************************************************************************/
void PhysicsContext::threadEntry()
{
  try
  {
    /* The start and end time of the updates. */
    std::chrono::high_resolution_clock::time_point startTime, endTime;

    /* The time diffirence. */
    std::chrono::duration<float> timeDiff(kTickRate);

    /* Update the physics scene. */
    update(timeDiff.count());

    /* Keep updating the physics while the thread is executing. */
    while(fIsExecuting)
    {
      /* Clear the synced flag. */
      bool synced = false;

      /* Record the start time. */
      startTime = std::chrono::high_resolution_clock::now();

      while(1)
      {
        /* Check if the tee has been synced. */
        if(!synced)
        {
          /* If not, sync the two scenes. */
          synced = sync();
        }

        /* Measure the end time. */
        endTime =  std::chrono::high_resolution_clock::now();

        /* Calculate the time diffirence. */
        timeDiff = endTime - startTime;

        /* Check if the wait time expired and another update should be
         * performed.*/
        if(timeDiff.count() > kTickRate)
        {
          break;
        }
        /* Yield the thread for now. */
        else
        {
          std::this_thread::yield();
        }
      }
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

/******************************************************************************/
void PhysicsContext::update(float dt)
{

}


