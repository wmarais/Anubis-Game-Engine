#ifndef ANUBIS_SIMULATION_CONTEXT_HPP
#define ANUBIS_SIMULATION_CONTEXT_HPP

#include "../Common.hpp"

namespace Anubis
{
  namespace Simulation
  {
    class Context
    {
      /** Define the function pointer type for the update function. */
      typedef void (Context::*UpdateFuncPtr)(float, bool);

      /** Define the function pointer type for the sync function. */
      typedef void (Context::*SyncFunPtr)(bool);

      /** Indicate whether the sim context should keep running. */
      std::atomic_bool fIsExecuting;

      /** The maximum rate at which frames must be updated. (I.e. the period in
       * nanoseconds, 120Hz = 8333333ns, etc. */
      const std::chrono::nanoseconds kUpdateRate;

      /** The network thread. */
      std::thread fNetworkThread;

      /** The physics thread. */
      std::thread fPhysicsThread;

      /** The AI thread. */
      std::thread fAIThread;

      /** The barried object used to synchronise all three threads before
       * invoking their update functions. */
      std::unique_ptr<Common::Barrier> fUpdateBarrier;

      /** The barrier object used to synchronise all three threads before
       * invoking their sync functions. */
      std::unique_ptr<Common::Barrier> fSyncBarrier;

      /** The exception that was thrown in the network thread (if any was
       * thrown). */
      std::exception_ptr fNetworkException;

      /** The exception that was thrown in the physics thread (if any was
       * thrown). */
      std::exception_ptr fPhysicsExceptions;

      /** The exception that was thrown in the ai thread (if any was
       * thrown). */
      std::exception_ptr fAIException;

      /*********************************************************************//**
       * Calculate the amount of time elapsed to process the previous frame
       * update and sync. The time will never be shorted than the configured
       * update rate.
       *
       * @param curTime   The current time variable. This variable is updated
       *                  by the function.
       * @param prevTime  The previous time variable. This variable is updated
       *                  by the function.
       * @return          The time in seconds that was required to process the
       *                  last frame update and sync.
       ************************************************************************/
      float calcFrameTime(std::chrono::steady_clock::time_point & curTime,
                          std::chrono::steady_clock::time_point & prevTime);

      /*********************************************************************//**
       * The function that is invoked when each thread is created. It is a
       * generic function that encapsulate the functionality of each thread
       * since the syncrhonisation mechanisms are near identical between each.
       *
       * @param obj           The context object that contains the sync and
       *                      update function for the thread.
       * @param updateFuncPtr The function pointer to the update function that
       *                      must be invoked by the thread.
       * @param syncFuncPtr   The function pointer to the sync function that
       *                      must be invoked to sync this thread to the other
       *                      threads.
       * @param statusA       The status variable of this thread.
       * @param statusB       The status variable of either of the other threads
       *                      that this thread must sync up too.
       * @param statusC       The status variable of either of the other threads
       *                      that this thread must sync up too.
       ************************************************************************/
      void threadEntry(Context * obj,
                       UpdateFuncPtr updateFuncPtr,
                       SyncFunPtr syncFuncPtr,
                       std::exception_ptr * exPtr);

    protected:

      virtual void syncNetwork(bool isFirstFrame);
      virtual void updateNetwork(float dt, bool isFirstFrame);

      virtual void syncPhysics(bool isFirstFrame);
      virtual void updatePhysics(float dt, bool isFirstFrame);

      virtual void syncAI(bool isFirstFrame);
      virtual void updateAI(float dt, bool isFirstFrame);

    public:
      Context(const std::chrono::nanoseconds & updateRate);

      virtual ~Context();

      /*********************************************************************//**
       * Return the execution state of the simulation.
       * @return
       ************************************************************************/
      ANUBIS_INLINE bool isExecuting()
      {
        return fIsExecuting;
      }

    };
  }
}


#endif /* ANUBIS_SIMULATION_CONTEXT_HPP */
