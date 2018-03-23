#ifndef ANUBIS_COMMON_CIRCULAR_QUEUE_HPP
#define ANUBIS_COMMON_CIRCULAR_QUEUE_HPP

#include "Misc.hpp"

namespace Anubis
{
  namespace Common
  {
    template <typename T, size_t kMaxCount> class CircularQueue final
    {
      size_t fCount;
      size_t fHead;
      size_t fTail;
      std::unique_ptr<T[]> fElements;
    public:

      CircularQueue() : fCount(0), fHead(0), fTail(0)
      {
        /* Alocate the memory for the queue elements. */
        fElements = std::unique_ptr<T[]>(new T[kMaxCount]);
      }

      bool isEmpty()
      {
        return fCount == 0;
      }

      bool push(const T & element)
      {
        /* Check if the queue is full. */
        if(fCount >= kMaxCount)
        {
          /* Return false to indicate the element was not added because the
           * queue is full. */
          return false;
        }

        /* Add the element to the queue. */
        fElements[fHead] = element;

        /* Increment the head position. */
        ++fHead;

        /* Wrap the position to 0 if we overflow. */
        if(fHead >= kMaxCount)
        {
          fHead = 0;
        }

        /* Increment the item count. */
        ++fCount;

        /* Return true to indicate that item was inserted into the queue. */
        return true;
      }

      bool pop(T & element)
      {
        /* Check if the queue is empty. */
        if(fCount == 0)
        {
          return false;
        }

        /* Read the current value. */
        element = fElements[fTail];

        /* Reduce the element count. */
        --fCount;

        ++fTail;
        if(fTail >= kMaxCount)
        {
          fTail = 0;
        }

        return true;
      }

    };
  }
}


#endif /* ANUBIS_COMMON_CIRCULAR_QUEUE_HPP */
