#ifndef ANUBIS_COMMON_FILE_HPP
#define ANUBIS_COMMON_FILE_HPP

namespace Anubis
{
  namespace Common
  {
    class File
    {
    public:
      enum class Modes : uint8_t
      {
        /** Open file for input operations. The file must exist. */
        Read,

        /** Create an empty file for output operations. If a file with the same
         * name already exists, its contents are discarded and the file is
         * treated as a new empty file. */
        Write,

        /** Open file for output at the end of a file. Output operations always
         * write data at the end of the file, expanding it. Repositioning
         * operations (fseek, fsetpos, rewind) are ignored. The file is created
         * if it does not exist. */
        Append,

        /** Open a file for update (both for input and output). The file must
         * exist. */
        ReadUpdate,

        /** Create an empty file and open it for update (both for input and
         * output). If a file with the same name already exists its contents
         * are discarded and the file is treated as a new empty file. */
        WriteUpdate,

        /** Open a file for update (both for input and output) with all output
         * operations writing data at the end of the file. Repositioning
         * operations (fseek, fsetpos, rewind) affects the next input
         * operations, but output operations move the position back to the end
         * of file. The file is created if it does not exist. */
        AppendUpdate
      };

      enum class Types : uint8_t
      {
        /** The file is an ASCII file. */
        ASCII,
        Binary,
      };



      //File(const std::string & path, Modes mode)
    };
  }
}



#endif /* ANUBIS_COMMON_FILE_HPP */
