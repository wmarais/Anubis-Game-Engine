/*******************************************************************************
 *
 * The purpose of this test is to determine if the build system  can:
 *
 *  - Build the application with SSE4.1 support.
 *  - Sucessfully run the application.
 ******************************************************************************/

/* Include the required SSE4 headers. */
#include <xmmintrin.h>
#include <smmintrin.h>

int main(int argc, char * argv[])
{
  /* Create a 3 x 4 x 5 right angle. */
  __m128 value = {3.0f, 4.0f, 0.0f, 0.0f};

  /* Calculate the length of the vector. */
  float length = _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(value, value,
                                                     0x71)));

  /* Check that the length was correctly calcualted. */
  if(length == 5.0f)
  {
    /* Indicate the program ran successfully. */
    return EXIT_SUCCESS;
  }

  /* Indicate that the application failed. */
  return EXIT_FAILURE;
}
