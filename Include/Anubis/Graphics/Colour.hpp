#ifndef ANUBIS_GRAPHICS_COLOUR_HPP
#define ANUBIS_GRAPHICS_COLOUR_HPP

#include "../Common/Misc.hpp"

namespace Anubis
{
  namespace Graphics
  {
    class Colour
    {
      union
      {
        uint8_t fMem[4];
      } __attribute__ ((aligned (ANUBIS_SIMD_MEM_ALIGNMENT)));

      static const size_t kRed = 0;
      static const size_t kGreen = 1;
      static const size_t kBlue = 2;
      static const size_t kAlpha = 3;

    public:

      Colour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
      {
        fMem[kRed] = red;
        fMem[kGreen] = green;
        fMem[kBlue] = blue;
        fMem[kAlpha] = alpha;

      }
    };
  }
}


#endif /* ANUBIS_GRAPHICS_COLOUR_HPP */
