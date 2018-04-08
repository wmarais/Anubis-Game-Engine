#ifndef ANUBIS_COMMON_ALGORITHMS_HPP
#define ANUBIS_COMMON_ALGORITHMS_HPP

#include "Misc.hpp"

namespace Anubis
{
  /*************************************************************************//**
   * Pause the execution of a thread until the user presses enter.
   ****************************************************************************/
  void pause();

  /*************************************************************************//**
   * Split a string into an array of tokens. This function retains empty tokens
   * between consecutive tokens.
   *
   * @param str
   * @param delim
   * @param skipEmpty
   * @return
   ****************************************************************************/
  std::vector<std::string> split(const std::string & str,
                                 const std::string & delims);



  /*************************************************************************//**
   * @brief writeLE
   * @param dst
   * @param len
   * @param val
   ****************************************************************************/
  template <typename T>
  ANUBIS_INLINE void writeLE(uint8_t * dst, size_t len, T val)
  {
    #ifdef ANUBIS_HOST_IS_LITTLE_ENDIAN
    /* No need to pack it because the host is the same endianess. */
    *reinterpret_cast<T*>(dst) = val;
    #endif /* ANUBIS_HOST_IS_LITTLE_ENDIAN */
  }

  /*************************************************************************//**
   * @brief readLE
   * @param src
   * @param len
   ****************************************************************************/
  template <typename T>
  ANUBIS_INLINE T readLE(const uint8_t * src, size_t len)
  {
    #ifdef ANUBIS_HOST_IS_LITTLE_ENDIAN
    /* No need to pack it because the host is the same endianess. */
    return *reinterpret_cast<const T*>(src);
    #endif /* ANUBIS_HOST_IS_LITTLE_ENDIAN */
  }


}


#endif /* ANUBIS_COMMON_ALGORITHMS_HPP */
