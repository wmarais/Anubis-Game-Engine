#include "../../../Include/Anubis/Graphics/MixedAttributeArray.hpp"

using namespace Anubis::Common;
using namespace Anubis::Graphics;

/******************************************************************************/
MixedAttributeArray::MixedAttributeArray(
    const std::vector<AttributeArray> & attribArrays) : fAttribCount(0),
      fMemLength(0), fStride(0), fMemory(nullptr, nullptr)
{
  /* Check atleast one attribute array has been passed in. */
  assert(attribArrays.size() > 0);

  /* Get the length of the attribute of the first array. */
  fAttribCount = attribArrays.begin()->count();

  /* Iterate through all the attribute arrays. */
  for(auto array : attribArrays)
  {
    /* Check that the arrays is the same size. */
    if(array.count() != fAttribCount)
    {
      ANUBIS_THROW_RUNTIME_EXCEPTION("Attribute array sizes do not match, " <<
        "expecting: " << fAttribCount << ", got: " << array.count() << ".");
    }

    /* Build the mapping of the current attribute. */
    fMappings.push_back(Mapping(array.name(), fStride));

    /* Increment the attribute stride. */
    fStride += array.dataSize();
  }

  /* Check if the data fits properly to the required data alignment. */
  if(fStride % kGPUDataAlignment)
  {
    /* Warn the user that the data will be padded. */
    ANUBIS_LOG_WARN("The attribute data will be padded to align to " <<
      kGPUDataAlignment << " byte boundaries. Current attribute size: " <<
      fStride << " bytes, padding: " << fStride/kGPUDataAlignment <<
      " bytes, new size: " << fStride + fStride/kGPUDataAlignment << " bytes.");

    /* Add the padding bytes. */
    fStride += fStride / kGPUDataAlignment;
  }

  /* Calculate the required memory space. */
  fMemLength = fStride * fAttribCount;

  /* Allocate the required memory. */
  fMemory = std::unique_ptr<void, decltype(&Memory::alignedFree)>
    (Memory::alignedAlloc(fMemLength, kGPUDataAlignment), Memory::alignedFree);

  /* Iterate through all the attributes. */
  for(size_t i = 0; i < fAttribCount; i++)
  {
    /* The offet of the current attirbute. */
    size_t offset = i * fStride;

    /* Iterate through all the attribute arrays. */
    for(auto array : attribArrays)
    {
      /* Copy the attribute into the interlaced memory. */
      offset += array.copyAttribute(i, fMemory.get());
    }
  }
}

///******************************************************************************/
//MixedAttributeArray::MixedAttributeArray(std::initializer_list<AttributeArray>
//  attribArrays) : fAttribCount(0), fMemLength(0), fStride(0),
//  fMemory(nullptr, nullptr)
//{
//  /* Check atleast one attribute array has been passed in. */
//  assert(attribArrays.size() > 0);

//  /* Get the length of the attribute of the first array. */
//  fAttribCount = attribArrays.begin()->count();

//  /* Iterate through all the attribute arrays. */
//  for(auto array : attribArrays)
//  {
//    /* Check that the arrays is the same size. */
//    if(array.count() != fAttribCount)
//    {
//      ANUBIS_THROW_RUNTIME_EXCEPTION("Attribute array sizes do not match, " <<
//        "expecting: " << fAttribCount << ", got: " << array.count() << ".");
//    }

//    /* Build the mapping of the current attribute. */
//    fMappings.push_back(Mapping(array.name(), fStride));

//    /* Increment the attribute stride. */
//    fStride += array.dataSize();
//  }

//  /* Check if the data fits properly to the required data alignment. */
//  if(fStride % kGPUDataAlignment)
//  {
//    /* Warn the user that the data will be padded. */
//    ANUBIS_LOG_WARN("The attribute data will be padded to align to " <<
//      kGPUDataAlignment << " byte boundaries. Current attribute size: " <<
//      fStride << " bytes, padding: " << fStride/kGPUDataAlignment <<
//      " bytes, new size: " << fStride + fStride/kGPUDataAlignment << " bytes.");

//    /* Add the padding bytes. */
//    fStride += fStride / kGPUDataAlignment;
//  }

//  /* Calculate the required memory space. */
//  fMemLength = fStride * fAttribCount;

//  /* Allocate the required memory. */
//  fMemory = std::unique_ptr<void, decltype(&Memory::alignedFree)>
//    (Memory::alignedAlloc(fMemLength, kGPUDataAlignment), Memory::alignedFree);

//  /* Iterate through all the attributes. */
//  for(size_t i = 0; i < fAttribCount; i++)
//  {
//    /* The offet of the current attirbute. */
//    size_t offset = i * fStride;

//    /* Iterate through all the attribute arrays. */
//    for(auto array : attribArrays)
//    {
//      /* Copy the attribute into the interlaced memory. */
//      offset += array.copyAttribute(i, fMemory.get());
//    }
//  }
//}

/******************************************************************************/
MixedAttributeArray::MixedAttributeArray(const MixedAttributeArray & cp) :
  fAttribCount(0), fMemLength(0), fStride(0), fMemory(nullptr, nullptr)
{
  /* Copy the simple properties. */
  fAttribCount = cp.fAttribCount;
  fMemLength = cp.fMemLength;
  fStride = cp.fStride;
  fMappings = cp.fMappings;

  /* Allocate memory to store the interleaved data. */
  fMemory = std::unique_ptr<void, decltype(&Memory::alignedFree)>
    (Memory::alignedAlloc(fMemLength, kGPUDataAlignment), Memory::alignedFree);

  /* Copy the memory. */
  memcpy(fMemory.get(), cp.fMemory.get(), fMemLength);
}

/******************************************************************************/
MixedAttributeArray & MixedAttributeArray::operator = (
    const MixedAttributeArray & rhs)
{
  /* Copy the simple properties. */
  fAttribCount = rhs.fAttribCount;
  fMemLength = rhs.fMemLength;
  fStride = rhs.fStride;
  fMappings = rhs.fMappings;

  /* Allocate memory to store the interleaved data. */
  fMemory = std::unique_ptr<void, decltype(&Memory::alignedFree)>
    (Memory::alignedAlloc(fMemLength, kGPUDataAlignment), Memory::alignedFree);

  /* Copy the memory. */
  memcpy(fMemory.get(), rhs.fMemory.get(), fMemLength);

  /* Return a reference to this object. */
  return *this;
}

