#include "../../../Include/Anubis/Common.hpp"

using namespace Anubis;

/******************************************************************************/
std::vector<std::string> split(const std::string & str,
                               const std::string & delims)
{
  /* The list of tokens to return. */
  std::vector<std::string> tokens;

  /* The start of the current token. */
  auto tokenStart = str.begin();

  /* Keep generating tokens while there are tokens. */
  do
  {
    /* The end of the current token. */
    auto tokenEnd = std::find_if(tokenStart, str.end(),
      [&delims](const char & val)
      {
        return std::find(delims.begin(), delims.end(), val) != delims.end();
      });

    /* Create the string token. */
    tokens.push_back(std::string(tokenStart, tokenEnd));

    /* Set the start position. */
    tokenStart = tokenEnd + 1;
  }
  while(tokenStart < str.end());

  /* Return the list of tokens. */
  return tokens;
}
