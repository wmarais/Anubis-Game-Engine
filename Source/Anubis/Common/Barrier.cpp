#include "../../../Include/Anubis/Common/Barrier.hpp"

using namespace Anubis::Common;

/******************************************************************************/
Barrier::Barrier(size_t threadCount, std::chrono::milliseconds pollInterval) :
  kThreadCount(threadCount), kPollInterval(pollInterval), fCount(0),
  fWaiting(0) {}

/******************************************************************************/
void Barrier::wait(std::atomic_bool & keepWaiting)
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
    [&]() { return keepWaiting ? fCount >= kThreadCount : true; }));

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
