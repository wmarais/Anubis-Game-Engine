#ifndef ANUBIS_GRAPHICS_MIXED_ATTRIBUTE_ARRAY_HPP
#define ANUBIS_GRAPHICS_MIXED_ATTRIBUTE_ARRAY_HPP

#include "../Common.hpp"
#include "AttributeArray.hpp"

namespace Anubis
{
  namespace Graphics
  {
    /***********************************************************************//**
     * A class used to interleave attribute arrays, however if a single
     * attribute array is supplied, it simply stores the attribute as a tightly
     * packed array.
     **************************************************************************/
    class MixedAttributeArray
    {
      /*********************************************************************//**
       * Store the mapping data for each attribute array into the interleaved
       * memory.
       ************************************************************************/
      struct Mapping
      {
        /** The name of the attribute array. */
        std::string fName;

        /** The offset into the interleaved array where the first attribute
         * value is located. */
        size_t fOffset;

        /** The attribute index as mapped into the shader. This is updated
         * when the mesh is added to a material and is outside of the scope
         * of the struct to initialise properly. */
        int32_t fAttribIndex;

        Mapping(const std::string & name, size_t offset) : fName(name),
          fOffset(offset), fAttribIndex(-1) {}
      };

      /** The memory alignment required for GPU data. */
      static const size_t kGPUDataAlignment = 4;

      /* The number of attributes stored in this array. */
      size_t fAttribCount;

      /* The length of memory (in bytes) used to interleave the attributes. */
      size_t fMemLength;

      /* The distance between consecutive attributes in the array. */
      size_t fStride;

      /* The list of attribute mappings. */
      std::vector<Mapping> fMappings;

      /* The memory to store the interlaced data. */
      std::unique_ptr<void,
        decltype(&Anubis::Common::Memory::alignedFree)> fMemory;

    public:

      MixedAttributeArray(const std::vector<AttributeArray> & attribArrays);

      /*********************************************************************//**
       *
       * @param attribArrays  The attribute arrays to interleave.
       ************************************************************************/
      //MixedAttributeArray(std::initializer_list<AttributeArray> attribArrays);

      /*********************************************************************//**
       * Perform a deep copy of the mixed attribute array.
       *
       * @param cp  The mixed attribute array to copy.
       ************************************************************************/
      MixedAttributeArray(const MixedAttributeArray & cp);

      /*********************************************************************//**
       * Perform a deep copy of the mixed attribute array on the right hand
       * side of the assignment.
       *
       * @param rhs The mixed attribute array to copy.
       * @return    Return a reference to this object.
       ************************************************************************/
      MixedAttributeArray & operator = (const MixedAttributeArray & rhs);

      const size_t stride()
      {
        return fStride;
      }
    };
  }
}

#endif /* ANUBIS_GRAPHICS_MIXED_ATTRIBUTE_ARRAY_HPP */
