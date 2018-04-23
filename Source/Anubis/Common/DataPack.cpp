#include "../../../Include/Anubis/Common.hpp"

using namespace Anubis::Common;

const std::string DataPack::kBasePath = u8"./Data";
const std::string DataPack::kExtension = u8".anpkg";
const std::string DataPack::kMetaExtension = u8".anmeta";

/******************************************************************************/
DataPack::DataPack(const std::string & path)
{
  /* A flag to indicate whether both paths were invalid. */
  bool bothInvalid = true;

  /* Check if there is a directory at the path. */
  if(dirExist(path) && !ANUBIS_LOAD_DATA_PACK_ONLY)
  {
    /* Load the contents of the dir. */
    loadDir(path);

    /* Mark atleast one path as valid. */
    bothInvalid = false;
  }

  /* Check if a datapack file by that name exist. */
  if(fileExist(path))
  {
    /* Load the contents of the data pack. */
    loadPack(path);

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
