#include "../../../Include/Anubis/Simulation/Context.hpp"

using namespace Anubis::Simulation;

/******************************************************************************/
Context::Context(const std::chrono::nanoseconds & updateRate) :
  fIsExecuting(true), kUpdateRate(updateRate),
  fNetworkException(nullptr), fPhysicsExceptions(nullptr),
  fAIException(nullptr)
{
  try
  {
    /* The barrier object to use for syncrhonising all the threads before
     * invoking their update functions. */
    fUpdateBarrier = std::make_unique<Common::Barrier>(3);

    /* The barrier object for synchronising the threads before invoking their
     * sync functions. */
    fSyncBarrier = std::make_unique<Common::Barrier>(3);


    /* Create the network thread. */
    fNetworkThread = std::thread(&Context::threadEntry,
                                 this,
                                 this,
                                 &Context::updateNetwork,
                                 &Context::syncNetwork,
                                 &fNetworkException);

    /* Create the physics thread. */
    fPhysicsThread = std::thread(&Context::threadEntry,
                                 this,
                                 this,
                                 &Context::updatePhysics,
                                 &Context::syncPhysics,
                                 &fPhysicsExceptions);

    /* Create the AI thread. */
    fAIThread = std::thread(&Context::threadEntry,
                            this,
                            this,
                            &Context::updateAI,
                            &Context::syncAI,
                            &fAIException);
  }
  catch(...)
  {
    /* Set the executing bit to false to kill any running threads. */
    fIsExecuting = false;

    /* Rethrow the exception. */
    throw std::current_exception();
  }
}

/******************************************************************************/
Context::~Context()
{
  /* Clear the execution flag. */
  fIsExecuting = false;

  /* Check if the network thread is still running. */
  if(fNetworkThread.joinable())
  {
    /* Wait for the network thread to finish. */
    fNetworkThread.join();
  }

  /* Check if the physics thread is running. */
  if(fPhysicsThread.joinable())
  {
    /* Wait for the physics thread to finish. */
    fPhysicsThread.join();
  }

  /* Check if the AI thread is running. */
  if(fAIThread.joinable())
  {
    /* Wait for the physics thread to finish. */
    fAIThread.join();
  }
}

/******************************************************************************/
float Context::calcFrameTime(std::chrono::steady_clock::time_point & curTime,
                             std::chrono::steady_clock::time_point & prevTime)
{
  /* Save the previous time. */
  prevTime = curTime;

  /* Get the new time. */
  curTime = std::chrono::steady_clock::now();

  /* Return the previous frame time. */
  return static_cast<float>(std::chrono::duration_cast<
                            std::chrono::nanoseconds>(
                              curTime - prevTime).count())/1e9f;
}


/******************************************************************************/
void Context::threadEntry(Context * obj,
                          UpdateFuncPtr updateFuncPtr,
                          SyncFunPtr syncFuncPtr,
                          std::exception_ptr * exPtr)
{
  try
  {
    /* Indicate if this is the first frame. */
    bool firstFrame = true;

    /* The current frame start time. */
    auto curFrameStart = std::chrono::steady_clock::now();

    /* The previous frame start time. */
    auto prevFrameStart = curFrameStart;

    /* Keep looping while there is something to do. */
    while(fIsExecuting)
    {
      /* Sync all the threads before invoking their update functions. */
      fUpdateBarrier->wait(fIsExecuting);

      /* Check that the update function is valid. */
      if(updateFuncPtr)
      {
        /* Invoke the update function. */
        (obj->*updateFuncPtr)(calcFrameTime(curFrameStart, prevFrameStart),
                             firstFrame);
      }

      /* Sync all the threads before invoking their sync functions. */
      fSyncBarrier->wait(fIsExecuting);

      /* Check that the sync function is valid. */
      if(syncFuncPtr)
      {
        /* Invoke the sync function. */
        (obj->*syncFuncPtr)(firstFrame);
      }

      /* Clear the first frame flag. */
      firstFrame = false;
    }
  }
  /* Catch any exceptions that are thrown. */
  catch(...)
  {
    /* Save the exception. */
    *exPtr = std::current_exception();

    /* Stop the thread. */
    fIsExecuting = false;
  }
}

/******************************************************************************/
void Context::syncNetwork(bool isFirstFrame) {}

/******************************************************************************/
void Context::updateNetwork(float dt, bool isFirstFrame) {}

/******************************************************************************/
void Context::syncPhysics(bool isFirstFrame) {}

/******************************************************************************/
void Context::updatePhysics(float dt, bool isFirstFrame) {}

/******************************************************************************/
void Context::syncAI(bool isFirstFrame) {}

/******************************************************************************/
void Context::updateAI(float dt, bool isFirstFrame){}

