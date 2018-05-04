#include "../../../Include/Anubis/Common.hpp"

using namespace Anubis::Common;

const std::string DataPack::kBasePath = u8"./Data/";
const std::string DataPack::kExtension = u8".anpkg";
const std::string DataPack::kMetaExtension = u8".anmeta";

/******************************************************************************/
DataPack::DataPack(const std::string & path)
{
  /* A flag to indicate whether both paths were invalid. */
  bool bothInvalid = true;

  /* Build the full path the directory. */
  std::string dirPath = kBasePath;
  dirPath.append(path);

  /* Build the full path the file. */
  std::string filePath = dirPath;
  filePath.append(kExtension);

  /* Check if there is a directory at the path. */
  if(boost::filesystem::is_directory(dirPath) && !ANUBIS_LOAD_DATA_PACK_ONLY)
  {
    /* Load the contents of the dir. */
    loadDir(dirPath);

    /* Mark atleast one path as valid. */
    bothInvalid = false;
  }

  /* Check if a datapack file by that name exist. */
  if(boost::filesystem::is_regular_file(filePath))
  {
    /* Load the contents of the data pack. */
    loadPack(filePath);

    /* Mark atleast one path as valid. */
    bothInvalid = false;
  }

  /* Check if both paths were invalid. */
  if(bothInvalid)
  {
    /* There is no resources at the path, this is an error. */
    ANUBIS_THROW_RUNTIME_EXCEPTION("Failed to load data pack: " << path
                                   << ".");
  }
}

/******************************************************************************/
DataPack::~DataPack()
{

}

/******************************************************************************/
void DataPack::loadDir(const std::string & path)
{
  /* Get all the meta-data files. */

}

/******************************************************************************/
void DataPack::loadPack(const std::string & path)
{

}
