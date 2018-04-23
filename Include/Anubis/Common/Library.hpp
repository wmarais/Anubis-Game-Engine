#ifndef ANUBIS_COMMON_LIBRARY_HPP
#define ANUBIS_COMMON_LIBRARY_HPP

#include "Misc.hpp"
#include "UUID.hpp"
#include "SubObj.hpp"

namespace Anubis
{
  namespace Common
  {
    class Library
    {
//      /** Indicate if the library is sorted or not. */
//      bool fSorted;

//      /** The list of items in the library. */
//      std::vector<std::unique_ptr<SubObj>> fItems;

//      void sort()
//      {
//        /* Sort the items in ascending order. */
//        std::sort(fItems.begin(), fItems.end(),
//        [](const std::unique_ptr<SubObj> & lhs,
//           const std::unique_ptr<SubObj> & rhs) -> bool
//        {
//          return lhs->getID() < rhs->getID();
//        });

//        /* Mark the library as sorted. */
//        fSorted = true;
//      }

    public:

      /*********************************************************************//**
       * Add a resource path to the library. This can pointer to either a sub
       * directory in "./Data" or can point to a data pack inside the "./Data"
       * directory. If the specified path contains both a directory of the same
       * name as a well as a data pack, i.e.:
       *
       *    ./Data/Level0/DataPack/
       *    ./Data/Level0/DataPack.anpkg
       *
       * Then the library will first load the contents of the directory:
       *
       *    ./Data/Level0/DataPack/
       *
       * Then it will try to load the contents of the data pack archive:
       *
       *    ./Data/Level0/DataPack.anpkg
       *
       * Since all library items must be uniqueue, it means that any duplicates
       * that exist in the DataPack.anpkg will not be loaded.
       *
       * Thus during development, all contents can simply be placed in the
       * unpackaged directory with the same name of the packaged data pack. Then
       * once the directory contents is packaged, and the directory removed
       * (i.e. when shipping the game), the game can still refer to the same
       * location and resources without needing code modifications.
       *
       * @param path  The path to the directory or data pack to load.
       ************************************************************************/
      void addPath(const std::string & path);



//      /*********************************************************************//**
//       * Reserves capacity to store items in the library.
//       *
//       * @param capacity  The number of items to reserve space for.
//       ************************************************************************/
//      ANUBIS_INLINE void reserver(size_t capacity)
//      {
////        fItems.reserve(capacity);
//      }

//      ANUBIS_INLINE void add(std::unique_ptr<T> & item)
//      {
////        fItems.push_back(std::move(item));
//      }

      /*********************************************************************//**
       * Find all the sub components that belong to the specified component.
       *
       * @param uuid
       * @param subComps
       ************************************************************************/
//      ANUBIS_INLINE void findAllSubComps(const UUID & uuid,
//                                         std::vector<T*> & subComps)
//      {
////        /* Iterate through all the items. */
////        for(auto item : fItems)
////        {
////          /* Check if the uuid matches. */
////          if(item->getCompID() == uuid)
////          {
////            /* Move on to the next item. */
////            subComps.push_back(item.get());
////          }
////        }
//      }

//      /*********************************************************************//**
//       * Find the object in the library based on it's UUID.
//       *
//       * @param uuid
//       * @return
//       ************************************************************************/
//      ANUBIS_INLINE T find(const UUID & uuid)
//      {
////        /* Check if the library needs to be sorted (since we use a binary search
////         * the library must be sorted in ascending order). */
////        if(!fSorted)
////        {
////          sort();
////        }

////        /* The range to search over. */
////        size_t low = 0, high = fItems.size() - 1;

////        /* Search until the item is found or we ran out of items. */
////        while(low <= high)
////        {
////          /* Calculate the mid point in the current range. */
////          size_t mid = low + (high - low) / 2;

////          /* Check if the mid point item is the target. */
////          if(fItems[mid]->getID() == uuid)
////          {
////            return fItems[mid].get();
////          }
////          /* Check if the item is above the mid point. */
////          else if(fItems[mid]->getID() < uuid)
////          {
////            low = mid + 1;
////          }
////          /* Else the item is below the mid point. */
////          else
////          {
////            high = mid - 1;
////          }
////        }

////        /* The item was not found. */
////        return nullptr;
//      }
    };
  }
}


#endif /* ANUBIS_COMMON_LIBRARY_HPP */
