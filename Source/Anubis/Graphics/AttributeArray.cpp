#include "../../../Include/Anubis/Graphics/AttributeArray.hpp"

using namespace Anubis::Common;
using namespace Anubis::Math;
using namespace Anubis::Graphics;

/******************************************************************************/
AttributeArray::AttributeArray(const std::string & name,
  const std::vector<Anubis::Math::Vector4f> & values) : fName(name),
  fMemory(nullptr, nullptr)
{
  /* Record the number of attributes in the array. */
  fCount = values.size();

  /* Set the size of the data type. */
  fDataSize = sizeof(Vector4f);

  /* Calculate the amount of memory to store the data. */
  fMemLength = fCount * fDataSize;

  /* Create the memory to store the values information. */
  fMemory = std::unique_ptr<void, decltype(&Memory::alignedFree)>
    (Memory::alignedAlloc(Memory::kGPUAlignment, fMemLength),
     Memory::alignedFree);

  /* Copy the data. */
  memcpy(fMemory.get(), values.data(), fMemLength);
}

/******************************************************************************/
AttributeArray::AttributeArray(const std::string & name,
  const std::vector<Vector2f> & values) : fName(name), fMemory(nullptr, nullptr)
{
  /* Record the number of attributes in the array. */
  fCount = values.size();

  /* Set the size of the data type. */
  fDataSize = sizeof(Vector2f);

  /* Calculate the amount of memory to store the data. */
  fMemLength = fCount * fDataSize;

  /* Create the memory to store the values information. */
  fMemory = std::unique_ptr<void, decltype(&Memory::alignedFree)>
    (Memory::alignedAlloc(Memory::kGPUAlignment, fMemLength),
     Memory::alignedFree);

  /* Copy the data. */
  memcpy(fMemory.get(), values.data(), fMemLength);
}

/******************************************************************************/
AttributeArray::AttributeArray(const std::string & name,
  const std::vector<Colour> & values) : fName(name), fMemory(nullptr, nullptr)
{
  /* Record the number of attributes in the array. */
  fCount = values.size();

  /* Set the size of the data type. */
  fDataSize = sizeof(Vector2f);

  /* Calculate the amount of memory to store the data. */
  fMemLength = fCount * fDataSize;

  /* Create the memory to store the values information. */
  fMemory = std::unique_ptr<void, decltype(&Memory::alignedFree)>
    (Memory::alignedAlloc(Memory::kGPUAlignment, fMemLength),
     Memory::alignedFree);

  /* Copy the data. */
  memcpy(fMemory.get(), values.data(), fMemLength);
}

/******************************************************************************/
AttributeArray::AttributeArray(const AttributeArray & cp) :
  fMemory(nullptr, nullptr)
{
  /* Copy the properties. */
  fName = cp.fName;
  fCount = cp.fCount;
  fMemLength = cp.fMemLength;
  fDataSize = cp.fDataSize;

  /* Create the memory to store the values information. */
  fMemory = std::unique_ptr<void, decltype(&Memory::alignedFree)>
    (Memory::alignedAlloc(Memory::kGPUAlignment, fMemLength),
     Memory::alignedFree);

  /* Copy the data. */
  memcpy(fMemory.get(), cp.fMemory.get(), fMemLength);
}

/******************************************************************************/
AttributeArray & AttributeArray::operator = (const AttributeArray & rhs)
{
  /* Copy the properties. */
  fName = rhs.fName;
  fCount = rhs.fCount;
  fMemLength = rhs.fMemLength;
  fDataSize = rhs.fDataSize;

  /* Create the memory to store the values information. */
  fMemory = std::unique_ptr<void, decltype(&Memory::alignedFree)>
    (Memory::alignedAlloc(Memory::kGPUAlignment, fMemLength),
     Memory::alignedFree);

  /* Copy the data. */
  memcpy(fMemory.get(), rhs.fMemory.get(), fMemLength);

  /* Return a reference to this object. */
  return *this;
}

/******************************************************************************/
size_t AttributeArray::copyAttribute(size_t index, void * dst) const
{

  /* Return the number of bytes copied. */
  return 0;
}
