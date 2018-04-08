#ifndef ANUBIS_COMMON_BARRIER_HPP
#define ANUBIS_COMMON_BARRIER_HPP

#include "Misc.hpp"

namespace Anubis
{
  namespace Common
  {
    /***********************************************************************//**
     * A class used to synchronise N number of threads. This should eventually
     * be replaced by C++20's barried class, but for now in C++14 the option
     * does not exist.
     *
     * The class works on the basis that the user specify the number of threads
     * that must be syncrhonised. Two counters are tracked, the number of
     * threads in total that called wait() (fCount) and the number of threads
     * that is still waiting to be woken up (fWait). Once fCount reaches the
     * nominated thread count (kThreadCount), the conditional variable emmits
     * the notify_all() signal. In order to prevent a race condition, (i.e. the
     * predicate evaluating to false before all the threads wake up) the fWait
     * counter is then reduced until it hits zero, at which point the fCount
     * variable can be reset.
     *
     * It should be noted that the wait function will occasionally wake up to
     * check the status of the thread to determine if the barries must pass
     * the thread so it can shut down properly.
     *
     * @warning The glaring limitation of this class is that two Barrier objects
     *          must always be used for repeated synchronisations, i.e. like a
     *          repeating loops etc.
     **************************************************************************/
    class Barrier
    {
      /** The mutex to use for locking. */
      std::mutex fLockMutex;

      /** The conditional variable to use for waiting. */
      std::condition_variable fLockCV;

      /** The number of threads to wait for before unlocking. */
      const size_t kThreadCount;

      /** The interval at which to poll the keepWaiting flag. */
      const std::chrono::milliseconds kPollInterval;

      /** The current number of threads waited on. */
      std::atomic_size_t fCount;

      /** The number of threads currently waiting. */
      std::atomic_size_t fWaiting;

      /** Delete the copy constructor. */
      Barrier(const Barrier &) = delete;

      /** Delete the assignment operator. */
      Barrier & operator = (const Barrier &) = delete;

    public:

      /*********************************************************************//**
       * Create the barrier object and specify the number of threads that it
       * will synchronise.
       *
       * @param threadCount   The number of threads to syncrhonise.
       * @param pollInterval  How often to poll the keepWaiting flag.
       ************************************************************************/
      Barrier(size_t threadCount, std::chrono::milliseconds pollInterval
              = std::chrono::milliseconds(500));

      /*********************************************************************//**
       * Wait for all the threads to be synchronised.
       *
       * @param keepWaiting   A boolean flag to indicate if the thread must
       *                      continue to wait or unblock. (This allows clean
       *                      termination of threads.)
       ************************************************************************/
      void wait(std::atomic_bool & keepWaiting);
    };
  }
}


#endif /* ANUBIS_COMMON_BARRIER_HPP */
