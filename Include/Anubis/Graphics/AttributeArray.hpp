#ifndef ANUBIS_GRAPHICS_ATTRIBUTE_ARRAY_HPP
#define ANUBIS_GRAPHICS_ATTRIBUTE_ARRAY_HPP

#include "../Common/Memory.hpp"
#include "Colour.hpp"
#include "../Math/Vector2f.hpp"
#include "../Math/Vector4f.hpp"

namespace Anubis
{
  namespace Graphics
  {
    /***********************************************************************//**
     * Contains a single array of attribute data.
     **************************************************************************/
    class AttributeArray
    {
      /*********************************************************************//**
       * The name of the attribute array as used in the respective shader.
       ************************************************************************/
      std::string fName;

      size_t fCount;

      size_t fMemLength;

      size_t fDataSize;

      /* The memory to store the attribute data. */
      std::unique_ptr<void,
        decltype(&Anubis::Common::Memory::alignedFree)> fMemory;

    public:
      /*********************************************************************//**
       * Constructor used to create either a vertex, normal or texture
       * coordinate array.
       *
       * @param name    The name of the attribute array matching the name used
       *                in the respective shader.
       * @param values  The values of the attribute array.
       ************************************************************************/
      AttributeArray(const std::string & name,
                     const std::vector<Anubis::Math::Vector4f> & values);

      /*********************************************************************//**
       * Constructor used to create a vertex or texture coordinate array.
       *
       * @param name    The name of the attribute array.
       * @param values  The values of the attribute array.
       ************************************************************************/
      AttributeArray(const std::string & name,
                     const std::vector<Math::Vector2f> & values);

      /*********************************************************************//**
       * Constructor used to create a vertex or texture coordinate array.
       *
       * @param name    The name of the attribute array.
       * @param values  The values of the attribute array.
       ************************************************************************/
      AttributeArray(const std::string & name,
                     const std::vector<Colour> & values);

      AttributeArray(const AttributeArray & cp);

      AttributeArray & operator = (const AttributeArray & rhs);

      /*********************************************************************//**
       * Copy the element at the specified index into the dst buffer. This
       * function is used by the MixedAtributeArray to interleave the buffers.
       *
       * @param index The index of the element to copy.
       * @param dst   The destination buffer to copy the element to.
       ************************************************************************/
      size_t copyAttribute(size_t index, void * dst) const;

      /*********************************************************************//**
       * Return the number of attributes stored in the array.
       ************************************************************************/
      size_t count() const
      {
        return fCount;
      }

      size_t memLength() const
      {
        return fMemLength;
      }

      size_t dataSize() const
      {
        return fDataSize;
      }

      std::string name() const
      {
        return fName;
      }

    };
  }
}

#endif /* ANUBIS_GRAPHICS_ATTRIBUTE_ARRAY_HPP */
