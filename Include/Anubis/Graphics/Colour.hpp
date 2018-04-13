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
        float fMem[4];
      } __attribute__ ((aligned (ANUBIS_SIMD_MEM_ALIGNMENT)));

      static const size_t kRed = 0;
      static const size_t kGreen = 1;
      static const size_t kBlue = 2;
      static const size_t kAlpha = 3;

    public:

      Colour(float red, float green, float blue, float alpha)
      {
        fMem[kRed] = red;
        fMem[kGreen] = green;
        fMem[kBlue] = blue;
        fMem[kAlpha] = alpha;

      }

      const float * data() const
      {
        return fMem;
      }
    };

    static const Colour kBlack(0.0f, 0.0f, 0.0f, 0.0f);
  }
}


#endif /* ANUBIS_GRAPHICS_COLOUR_HPP */
