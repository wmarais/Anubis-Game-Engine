#ifndef ANUBIS_COMMON_BARRIER_HPP
#define ANUBIS_COMMON_BARRIER_HPP

#include "Misc.hpp"

namespace Anubis
{
  namespace Common
  {
    class Barrier
    {
      /** The mutex to use for locking. */
      std::mutex fLockMutex;

      /** The conditional variable to use for waiting. */
      std::condition_variable fLockCV;

      /** The number of threads to wait for before unlocking. */
      const size_t kThreadCount;

      /** The current number of threads waited on. */
      std::atomic_size_t fCount;

      /** The number of threads currently waiting. */
      std::atomic_size_t fWaiting;

    public:

      Barrier(size_t threadCount) : kThreadCount(threadCount) {}

      void wait(std::atomic_bool & running)
      {
        /* The count or waiting should never exceed the maximum thread count,
         * this would indicate more threads are used than intended and thus
         * synchronisation can not be guarenteed. */
        assert(fCount < kThreadCount && fWaiting < kThreadCount &&
               "Barrier - More threads are used than budgeted for!");

        /* Lock the mutex. */
        std::unique_lock<std::mutex> lock(fLockMutex);

        /* Increment the thread count. */
        ++fCount;

        /* Increment the current number of threads waiting. */
        ++fWaiting;

        /* Block until all the threads are synced up. */
        while(!fLockCV.wait_for(lock, std::chrono::milliseconds(1),
          [&]() { return running ? fCount >= kThreadCount : true; }));

        /* Check if a notification must be send. */
        if(fWaiting >= kThreadCount)
        {
          /* Notify all the threads to resume. */
          fLockCV.notify_all();
        }

        /* Decrement the waiting counter. */
        --fWaiting;

        /* Check if the counter must be reset. */
        if(fWaiting == 0)
        {
          /* Reset the count. */
          fCount = 0;
        }
      }
    };
  }
}


#endif /* ANUBIS_COMMON_BARRIER_HPP */
