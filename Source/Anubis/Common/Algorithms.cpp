#include "../../../Include/Anubis/Common/System.hpp"
#include "../../../Include/Anubis/Common.hpp"

using namespace Anubis;
using namespace Anubis::Common;

/******************************************************************************/
void Anubis::pause()
{
  /* Safely write an IO message to cout. */
  Common::Log::get().write(Common::Log::Levels::IO,
                           "Press enter to continue ...");

  /* Wait for the user to press enter. */
  std::cin.get();
}

/******************************************************************************/
std::vector<std::string> Anubis::split(const std::string & str,
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

/******************************************************************************/
bool dirExist(const std::string & path)
{
#ifdef ANUBIS_OS_WINDOWS
  DWORD ftyp = GetFileAttributesA(path.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with your path!

  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;   // this is a directory!

  return false;    // this is not a directory!
#else
  ANUBIS_THROW_RUNTIME_EXCEPTION("Function not implemented");
#endif
}

/******************************************************************************/
bool fileExist(const std::string & path)
{
#ifdef ANUBIS_OS_WINDOWS
  DWORD ftyp = GetFileAttributesA(path.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with your path!
  return true;
#else
  ANUBIS_THROW_RUNTIME_EXCEPTION("Function not implemented");
#endif
}

/******************************************************************************/
std::vector<std::string> allFiles(const std::string & path)
{
#ifdef ANUBIS_OS_WINDOWS
  /* the list of files in the directory. */
  std::vector<std::string> files;

  HANDLE hFind;
  WIN32_FIND_DATA data;

  /* Find the first file in the path. */
  hFind = FindFirstFile("c:\\*.*", &data);
  if (hFind != INVALID_HANDLE_VALUE)
  {

    do {
      printf("%s\n", data.cFileName);
    } while (FindNextFile(hFind, &data));
    FindClose(hFind);
  }
#else
  ANUBIS_THROW_RUNTIME_EXCEPTION("Function not implemented");
#endif
}
