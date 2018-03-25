#ifndef ANUBIS_COMMON_CIRCULAR_QUEUE_HPP
#define ANUBIS_COMMON_CIRCULAR_QUEUE_HPP

#include "Misc.hpp"

namespace Anubis
{
  namespace Common
  {
    /***********************************************************************//**
     * A general purpose Circular Queue class. Once created, the Push(), Pop(),
     * isEmpty() and isFull() functions are thread safe. This this class is
     * primarily used message passing between threads and communication and it
     * made a lot of sense to build the locking into the class.
     **************************************************************************/
    template <typename T, size_t kMaxCount> class CircularQueue final
    {
      /** The number of elements stored in the circular queue. */
      std::atomic_size_t fCount;

      /** The index of the head element. */
      size_t fHead;

      /** The index of the tail element. */
      size_t fTail;

      /** The thread used for controlling access to the push() and pop()
       * functions. */
      std::mutex fMutex;

      /* The memory where all the elements are stored. */
      std::unique_ptr<T[]> fElements;
    public:

      /*********************************************************************//**
       * Creates a circular queue allocating the required memory and
       * initialising all counters and indexes to 0.
       ************************************************************************/
      CircularQueue() : fCount(0), fHead(0), fTail(0)
      {
        /* Alocate the memory for the queue elements. */
        fElements = std::unique_ptr<T[]>(new T[kMaxCount]);
      }

      /*********************************************************************//**
       * Check whether the queue is empty (containing 0 elemenets).
       *
       * @return True if the queue is empty, else false.
       ************************************************************************/
      ANUBIS_FORCE_INLINE bool isEmpty() const
      {
        /* Check if the queue is emtpy or not. */
        return fCount == 0;
      }

      /*********************************************************************//**
       * Checks whether the queue is full (contains kMaxCount elements).
       *
       * @return  True if the queue is full, else false.
       ************************************************************************/
      ANUBIS_FORCE_INLINE bool isFull() const
      {
        /* Check if the queue is full or not. */
        return fCount >= kMaxCount;
      }

      /*********************************************************************//**
       * Push a new element into the head position of the queue.
       *
       * @param element The element to be enqueued.
       * @return        True if the lement was inserted, else false. (This will
       *                only ever be fase if the queue is full.)
       ************************************************************************/
      bool push(const T & element)
      {
        /* Check if the queue is full. */
        if(isFull())
        {
          /* Return false to indicate the element was not added because the
           * queue is full. */
          return false;
        }

        /* Lock the access mutex. */
        std::lock_guard<std::mutex> lock(fMutex);

        /* Add the element to the queue. */
        fElements[fHead] = element;

        /* Increment the head position. */
        ++fHead;

        /* Wrap the index to 0 if overflows. */
        if(fHead >= kMaxCount)
        {
          fHead = 0;
        }

        /* Increment the item count. This must be done right at the end else
         * a race condition can occur with pop(). */
        ++fCount;

        /* Return true to indicate that item was inserted into the queue. */
        return true;
      }

      /*********************************************************************//**
       * Pop an element from the tail position of the queue.
       *
       * @param element The reference where the dequeued object will be stored
       *                too.
       * @return        True if the dequeue operation was sucessful, else false.
       ************************************************************************/
      bool pop(T & element)
      {
        /* Check if the queue is empty. */
        if(isEmpty())
        {
          /* Return false to indicate the queue is empty. */
          return false;
        }

        /* Lock the access mutex. */
        std::lock_guard<std::mutex> lock(fMutex);

        /* Read the current value. */
        element = fElements[fTail];

        /* Advance the tail position. */
        ++fTail;

        /* Wrap the index to 0 if it overlfows. */
        if(fTail >= kMaxCount)
        {
          fTail = 0;
        }

        /* Reduce the element count. */
        --fCount;

        /* Return true to indicate that the item was dequeued. */
        return true;
      }
    };
  }
}


#endif /* ANUBIS_COMMON_CIRCULAR_QUEUE_HPP */
