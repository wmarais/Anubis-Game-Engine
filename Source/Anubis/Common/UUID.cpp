#include "../../../Include/Anubis/Common/System.hpp"
#include "../../../Include/Anubis/Common/UUID.hpp"
#include "../../../Include/Anubis/Common/Memory.hpp"

using namespace Anubis::Common;

/******************************************************************************/
Anubis::Common::UUID::UUID()
{
  /* Generate the UUID using the Windows RPCRT4 library. */
  #ifdef ANUBIS_HAS_RPCRT4
    /* The temporary windows UUID object. */
    ::UUID uuid;

    /* Create the UUID object. */
    UuidCreate(&uuid);

    /* Extract all the bytes from the UUID. */
    fOctets[0] = Memory::getByte(3, uuid.Data1);
    fOctets[1] = Memory::getByte(2, uuid.Data1);
    fOctets[2] = Memory::getByte(1, uuid.Data1);
    fOctets[3] = Memory::getByte(0, uuid.Data1);

    fOctets[4] = Memory::getByte(1, uuid.Data2);
    fOctets[5] = Memory::getByte(0, uuid.Data2);

    fOctets[6] = Memory::getByte(1, uuid.Data3);
    fOctets[7] = Memory::getByte(0, uuid.Data3);

    fOctets[8]  = uuid.Data4[0];
    fOctets[9]  = uuid.Data4[1];
    fOctets[10] = uuid.Data4[2];
    fOctets[11] = uuid.Data4[3];
    fOctets[12] = uuid.Data4[4];
    fOctets[13] = uuid.Data4[5];
    fOctets[14] = uuid.Data4[6];
    fOctets[15] = uuid.Data4[7];
  #endif /* ANUBIS_HAS_RPCRT4 */
}

/******************************************************************************/
void Anubis::Common::UUID::writeString(std::ostream & os) const
{
  /* Write the UUID in ASCII form and return the reference to the ostream. */
  for(size_t i = 0; i < kOctetCount; i++)
  {
    /* Set the format to print out. */
    os << std::hex << std::setw(2) << std::setfill('0') << int(fOctets[i]);

    /* Check if a "-" character must be inserted. */
    if(i == 3 || i == 5 || i == 7 || i == 9)
    {
      os << '-';
    }
  }
}

/******************************************************************************/
void Anubis::Common::UUID::writeBinary(std::ostream & os) const
{
  /* Write the binary octets. */
  for(size_t i = 0; i < kOctetCount; i++)
  {
    os << fOctets[i];
  }
}
