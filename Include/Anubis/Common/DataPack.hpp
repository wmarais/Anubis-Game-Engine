#ifndef ANUBIS_COMMON_DATA_PACK_HPP
#define ANUBIS_COMMON_DATA_PACK_HPP

#include "Misc.hpp"
#include "UUID.hpp"

namespace Anubis
{
  namespace Common
  {
    /***********************************************************************//**
     * A data pack class that can and should be used for loading game data. The
     * constructor for the class expect a path relative to ./Data directory
     * of the game. The path can either be a sub directory in ./Data or it can
     * be a DataPack file:
     *
     *    Sub Dir:    ./Data/Level1/DataPack
     *    Data Pack:  ./Data/Level1/DataPack.anpkg
     *
     * If the specified path contains both a Sub Directory and Data Pack with
     * the same name, then the contents of the Sub Directory is loaded first,
     * i.e.:
     *
     *    ./Data/Level1/DataPack
     *
     * Then any non duplicate contents of the Data Pack is loaded, i.e.:
     *
     *    ./Data/Level0/DataPack.anpkg
     *
     * Thus during development, all contents can simply be placed in the
     * unpackaged directory with the same name of the packaged data pack. Then
     * once the directory contents is packaged, and the directory removed
     * (i.e. when shipping the game), the game can still refer to the same
     * location and resources without needing code modifications.
     *
     * It is recommended to use a data directory structure similar to:
     *
     *    ./Data/Common/Common
     *    ./Data/Maps/Map1
     *    ./Data/Maps/Map2
     *    ./Data/Campaign/Level1
     *    ./Data/Campaign/Level2
     *                            ... etc
     *
     * With this layout, the data stored in ./Data/Common/Common can be used for
     * all common elements through out the game. For example, the shaders for
     * rendering text, menu skins, UI sounds, etc. The Maps folder can then be
     * used for storing all the maps in the game and any specific resources
     * that has to be loaded for each map. (This makes them easily available to
     * both single and multiplayer games.) The Campaign can contain any specific
     * information to game's campaing mode regardless of single or multiplayer
     * campaing modes.
     **************************************************************************/
    class DataPack final
    {
    public:
      struct Item
      {
        virtual ~Item() = default;
      };

    private:

      /** All the items that was loaded from the data pack. */
      std::map<Common::UUID, std::unique_ptr<Item>> fItems;

      /*********************************************************************//**
       * Load the contents of a directory.
       *
       * @param path  The path to the directory to load.
       ************************************************************************/
      void loadDir(const std::string & path);

      /*********************************************************************//**
       * Load the contents of a data pack file.
       *
       * @param path  The path to the data pack to load.
       ************************************************************************/
      void loadPack(const std::string & path);

    public:

      /** The base path to open data packs from. */
      static const std::string kBasePath;

      /** The file extension used for data packs. */
      static const std::string kExtension;

      /** The meta-data file extension for data files. */
      static const std::string kMetaExtension;

      /*********************************************************************//**
       * Create a data pack object and load the contents of the data pack at
       * the path. By default both the contents of the directory and the data
       * pack is loaded, however CMake can be configured to only force data
       * packs to be loaded. (Do this before releasing the game.)
       *
       * @param path  The path to the directory or data pack to load.
       ************************************************************************/
      DataPack(const std::string & path);

      /*********************************************************************//**
       * Destroy the data pack object.
       ************************************************************************/
      ~DataPack();

    };
  }
}

#endif /* ANUBIS_COMMON_DATA_PACK_HPP */
