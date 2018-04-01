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
}


#endif /* ANUBIS_COMMON_ALGORITHMS_HPP */
