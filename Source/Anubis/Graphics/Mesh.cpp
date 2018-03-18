#include "../../../Include/Anubis/Graphics/Mesh.hpp"

using namespace Anubis::Graphics;

/******************************************************************************/
Mesh::Mesh(std::vector<uint16_t> indexes,
  std::initializer_list<MixedAttributeArray> attribArrays) : fIndexes(indexes)
{

}

/******************************************************************************/
const std::vector<std::shared_ptr<MixedAttributeArray>> &
  Mesh::mixedAttribArrays() const
{
  return fMixedAttribArrays;
}

///******************************************************************************/
//bool Mesh::matchesLayout(const Mesh & rhs)
//{
//  /* The status of the function. */
//  bool status = true;

//  /* Check the mixed arrays count matches. */
//  if(fMixedAttribArrays.size() != rhs.fMixedAttribArrays.size())
//  {
//    status = false;
//  }

//  /* Check the attribute sizes (Stride) for each atribute array matches. */
//  for(size_t i = 0; i < fMixedAttribArrays.size() && status == true; i++)
//  {
//    /* Check if the stride matches for the current item. */
//    if(fMixedAttribArrays[i]->stride() != rhs.fMixedAttribArrays[i]->stride())
//    {
//      /* If not, then the layout does not match. */
//      status = false;
//    }
//  }

//  /* Check the name and order of  each attribute matches. */

//  /* Check the data types of each matches.

//  /* Return the status of the function. */
//  return status;
//}
