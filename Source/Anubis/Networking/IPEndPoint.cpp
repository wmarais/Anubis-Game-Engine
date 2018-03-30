#include "../../../Include/Anubis/Common/System.hpp"
#include "../../../Include/Anubis/Networking/IPEndPoint.hpp"

using namespace Anubis::Common;
using namespace Anubis::Networking;

/******************************************************************************/
struct Anubis::Networking::IPEndPoint::Data
{
  /* Lock to control access to the WSAStartup function. I'm not sure if this is
   * required but there seems to be no explicit statement about thread safety,
   * so not taking any chances. */
  static std::mutex fWSAMutex;

  /** The result of the winsock startup. For every succesful startup there
   * must be a cleanup. */
  int fWSAStartupResult;

  /** The node name that was supplied. This can be diffirent to the IP address
   * that is used. I.e. if a dns name was specified. */
  std::string fNodeName;

  /** The byte data of the address structure. */
  std::unique_ptr<uint8_t[]> fAddrData;

  /** The length of the data. */
  size_t fAddrDataLen;

  Data();
  ~Data();

  void buildV4Addr(const sockaddr_in * src, uint16_t port);
  void buildV6Addr(const sockaddr_in6 * src, uint16_t port);
};

std::mutex Anubis::Networking::IPEndPoint::Data::fWSAMutex;

/******************************************************************************/
IPEndPoint::Data::Data() : fWSAStartupResult(-1), fAddrDataLen(0)
{
  /* Lock the WSA access mutex. */
  std::lock_guard<std::mutex> lock(fWSAMutex);

  /* Startup winsock and save the result. */
  WSADATA wsa;
  fWSAStartupResult = WSAStartup(MAKEWORD(2,2), &wsa);

  /* Check if the sockets library was started properly. */
  if(fWSAStartupResult != 0)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("WSAStartup failed with error code: " <<
                                   fWSAStartupResult);
  }
}

/******************************************************************************/
IPEndPoint::Data::~Data()
{
  /* Lock the WSA access mutex. */
  std::lock_guard<std::mutex> lock(fWSAMutex);

  /* Check if the data object was created and that the sockets library was
   * successfully initialised. */
  if(fWSAStartupResult == 0)
  {
    /* Clean up the sockets library. */
    WSACleanup();
  }
}

/******************************************************************************/
void IPEndPoint::Data::buildV4Addr(const sockaddr_in * src, uint16_t port)
{
  /* Calculate the size of the addr object / memory to allocate. */
  fAddrDataLen = sizeof(struct sockaddr_in);

  /* Alocate the memory to store the addr object. */
  fAddrData = std::unique_ptr<uint8_t[]>(new uint8_t[fAddrDataLen]);

  /* Get a type safe pointer to set the parameters. */
  struct sockaddr_in * dst = reinterpret_cast<struct sockaddr_in*>
      (fAddrData.get());

  /* Zero the memory. */
  memset(dst, 0, sizeof(struct sockaddr_in));

  /* Copy the IP address only. */
  memcpy(&(dst->sin_addr), &(src->sin_addr), sizeof(struct in_addr));

  /* Set the port number. */
  dst->sin_port = htons(port);

  /* Set the address family. */
  dst->sin_family = htons(AF_INET);
}

/******************************************************************************/
void IPEndPoint::Data::buildV6Addr(const sockaddr_in6 * src, uint16_t port)
{
  /* Calculate the size of the addr object / memory to allocate. */
  fAddrDataLen = sizeof(struct sockaddr_in6);

  /* Alocate the memory to store the addr object. */
  fAddrData = std::unique_ptr<uint8_t[]>(new uint8_t[fAddrDataLen]);

  /* Get a type safe pointer to set the parameters. */
  struct sockaddr_in6 * dst = reinterpret_cast<struct sockaddr_in6*>
      (fAddrData.get());

  /* Zero the memory. */
  memset(dst, 0, sizeof(struct sockaddr_in6));

  /* Copy the IP address only. */
  memcpy(&(dst->sin6_addr), &(src->sin6_addr), sizeof(struct in6_addr));

  /* Set the port number. */
  dst->sin6_port = htons(port);

  /* Set the address family. */
  dst->sin6_family = htons(AF_INET6);
}

/******************************************************************************/
IPEndPoint::IPEndPoint(uint16_t port, const std::string & nodeName,
  Preferences pref) : fData(nullptr)
{
  /* Create the data object. */
  fData = std::make_unique<Data>();

  /* Save the node name. */
  fData->fNodeName = nodeName;

  /* The hints to pass for address information lookup. */
  struct addrinfo hints;

  /* The result from the address info lookup. */
  struct addrinfo *result = nullptr;

  /* Clear the hints object. */
  memset(&hints, 0, sizeof(struct addrinfo));

  /* Set the appropriate hints. */
  hints.ai_family = AF_UNSPEC;

  /* Retrieve the address list. */
  int ret = getaddrinfo(nodeName.c_str(), nullptr, &hints, &result);

  /* Check if the operation failed. */
  if(ret != 0)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("getaddrinfo() failed with error: " <<
                                   ret);
  }

  /* The first IPv4 address that was found. */
  struct addrinfo * addrInfoIPv4 = nullptr;

  /* The first IPv6 address that was found. */
  struct addrinfo * addrInfoIPv6 = nullptr;

  /* Iterate through the linked list of results until both an IPv4 and IPV6
   * dress has been found or we run out of results. */
  for(struct addrinfo * curResult = result;
      curResult && (!addrInfoIPv4 || !addrInfoIPv6);
      curResult = curResult->ai_next)
  {
    /* Check if any IPv4 address has been found. */
    if(curResult->ai_family == AF_INET)
    {
      /* Set the first IPv4 address. */
      if(!addrInfoIPv4)
      {
        addrInfoIPv4 = curResult;
      }
    }
    else if(curResult->ai_family == AF_INET6)
    {
      /* Set the first IPv6 address. */
      if(!addrInfoIPv6)
      {
        addrInfoIPv6 = curResult;
      }
    }
  }

  /* The best address to use. */
  struct addrinfo * bestAdrInfo = nullptr;

  /* The address family to use. */
  uint16_t family;

  /* Check if there is IPv4 data. */
  if(addrInfoIPv4)
  {
    /* Check if an IPv4 address is adequate. */
    if(pref == Preferences::IPv4Only || pref == Preferences::IPv4 ||
       pref == Preferences::Any)
    {
      family = AF_INET;
      bestAdrInfo = addrInfoIPv4;
    }
  }

  /* Check if there is IPv6 data. */
  if(addrInfoIPv6)
  {
    /* Check if an IPv6 address is adequated. */
    if(!bestAdrInfo &&
       (pref == Preferences::IPv6Only || pref == Preferences::IPv6 ||
        pref == Preferences::IPv4 || pref == Preferences::Any))
    {
      family = AF_INET6;
      bestAdrInfo = addrInfoIPv6;
    }
  }
  /* Check if an IPv4 address can be used instead of an IPv6 address. */
  else if(!bestAdrInfo && pref != Preferences::IPv6Only)
  {
    family = AF_INET;
    bestAdrInfo = addrInfoIPv4;
  }

  /* Check if an address was found. */
  if(bestAdrInfo != nullptr)
  {
    if(bestAdrInfo->ai_family == AF_INET)
    {
      /* Check the assumption about address length and socket structure length
       * matches. */
      assert(bestAdrInfo->ai_addrlen == sizeof(struct sockaddr_in) &&
             "bestAdrInfo->ai_addrlen != sizeof(struct sockaddr_in)");

      /* Build and IPv4 address. */
      fData->buildV4Addr(reinterpret_cast<struct sockaddr_in*>
                         (bestAdrInfo->ai_addr), port);
    }
    else
    {
      /* Check the assumption about address length and socket structure length
       * matches. */
      assert(bestAdrInfo->ai_addrlen == sizeof(struct sockaddr_in6) &&
             "bestAdrInfo->ai_addrlen != sizeof(struct sockaddr_in6)");

      /* Build and IPv6 address. */
      fData->buildV6Addr(reinterpret_cast<struct sockaddr_in6*>
                         (bestAdrInfo->ai_addr), port);
    }
  }

  /* Free the queue. */
  freeaddrinfo(result);

  /* Check if any results were found. */
  if(bestAdrInfo == nullptr)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("Unable to find a suitable IP Address "
      "for node name: " << nodeName);
  }
}

/******************************************************************************/
IPEndPoint::IPEndPoint(const IPEndPoint & cp)
{
  /* Create the data object. */
  fData = std::make_unique<Data>();

  /* Copy the node name. */
  fData->fNodeName = cp.fData->fNodeName;

  /* Set the data length. */
  fData->fAddrDataLen = cp.fData->fAddrDataLen;

  /* Create the memory for the address object. */
  fData->fAddrData = std::unique_ptr<uint8_t[]>(
        new uint8_t[fData->fAddrDataLen]);

  /* Copy the address structure. */
  memcpy(fData->fAddrData.get(), cp.fData->fAddrData.get(),
         fData->fAddrDataLen);
}

/******************************************************************************/
IPEndPoint::~IPEndPoint() = default;

/******************************************************************************/
IPEndPoint & IPEndPoint::operator = (const IPEndPoint & rhs)
{
  /* Copy the node name. */
  fData->fNodeName = rhs.fData->fNodeName;

  /* Set the data length. */
  fData->fAddrDataLen = rhs.fData->fAddrDataLen;

  /* Create the memory for the address object. */
  fData->fAddrData = std::unique_ptr<uint8_t[]>(
        new uint8_t[fData->fAddrDataLen]);

  /* Copy the address structure. */
  memcpy(fData->fAddrData.get(), rhs.fData->fAddrData.get(),
         fData->fAddrDataLen);

  /* Return the reference to this object. */
  return *this;
}

/******************************************************************************/
const uint8_t * IPEndPoint::addrData() const
{
  return fData->fAddrData.get();
}

/******************************************************************************/
const size_t IPEndPoint::addrDataLen() const
{
  return fData->fAddrDataLen;
}

/******************************************************************************/
bool IPEndPoint::isIPv4() const
{
  /* Check to see if the family type is IPv4. */
  return Memory::fromBigEndian<uint16_t>(fData->fAddrData.get(),
                                         fData->fAddrDataLen) == AF_INET;
}

/******************************************************************************/
bool IPEndPoint::isIPv6() const
{
  /* Check to see if the family type is IPv6. */
  return Memory::fromBigEndian<uint16_t>(fData->fAddrData.get(),
                                         fData->fAddrDataLen) == AF_INET6;
}

/******************************************************************************/
std::string IPEndPoint::nodeName() const
{
  return fData->fNodeName;
}

/******************************************************************************/
uint16_t IPEndPoint::port() const
{
  return Memory::fromBigEndian<uint16_t>(
        fData->fAddrData.get() + sizeof(uint16_t),
        fData->fAddrDataLen);
}

/******************************************************************************/
std::string IPEndPoint::ip() const
{
  /* Create a string stream.*/
  std::stringstream ss;

  /* Check if it's IPv4. */
  if(isIPv4())
  {
    /* Convert to the right socket type to get the right address index. */
    sockaddr_in * sockAddr = reinterpret_cast<sockaddr_in*>
        (fData->fAddrData.get());

    /* Print the address to a string. */
    ss << (unsigned int)(sockAddr->sin_addr.S_un.S_un_b.s_b1) << "." <<
          (unsigned int)(sockAddr->sin_addr.S_un.S_un_b.s_b2) << "." <<
          (unsigned int)(sockAddr->sin_addr.S_un.S_un_b.s_b3) << "." <<
          (unsigned int)(sockAddr->sin_addr.S_un.S_un_b.s_b4);
  }
  else
  {
    /* Convert to the right socket type to get the right address index. */
    sockaddr_in6 * sockAddr = reinterpret_cast<sockaddr_in6*>
        (fData->fAddrData.get());

    /* Create a vector of the shorts. */
    std::vector<uint16_t> shorts;
    for(size_t i = 0; i < kIPv6ShortsCount; i++)
    {
      shorts.push_back(Memory::fromBigEndian<uint16_t>(
        sockAddr->sin6_addr.u.Byte, kIPv6OctetCount, i * sizeof(uint16_t)));
    }

    /* Find the longest range of 0's. */
    size_t zerosStart = kIPv6ShortsCount, zerosEnd = kIPv6ShortsCount;

    /* Iterate through all the shorts in the address. */
    for(size_t i = 0; i < shorts.size(); i++)
    {
      /* Get a temporary start and end position from the current index. */
      size_t tempZeroStart = i, tempZeroEnd = i;

      /* Check if the value is zero. */
      if(shorts[i] == 0)
      {
        /* Keep looping until the last zero in this sequence is found. */
        for(; i < shorts.size() && shorts[i] == 0; i++)
        {
          /* Set the new zero index. */
          tempZeroEnd = i;
        }
      }

      /* Check if the new range is larger than the old range. */
      if(tempZeroEnd - tempZeroStart > zerosEnd - zerosStart)
      {
        /* Set the new range indexes. */
        zerosStart = tempZeroStart;
        zerosEnd = tempZeroEnd;
      }
    }

    /* Write the IP address string. */
    for(size_t i = 0; i < shorts.size(); i++)
    {
      /* Check if index is in the shrink range. */
      if(i >= zerosStart && i <= zerosEnd)
      {
        /* Write the shrink value. */
        ss << "::";

        /* Skip forward. */
        i = zerosEnd;
      }
      else
      {
        /* Make sure it's not the first or last short. */
        if(i > 0 && i < shorts.size() - 1)
        {
          /* Write the seperator. */
          ss << ":";
        }

        /* Write the current value. */
        ss << shorts[i];
      }
    }
  }

  /* Return the constructed string. */
  return ss.str();
}

/******************************************************************************/
std::ostream & operator << (std::ostream & os, const IPEndPoint & ep)
{
  /* Check if it's an IPv4 Address. */
  os << ep.nodeName() << "(" << ep.ip() << ") : " << ep.port();

  /* Return the reference to the output stream. */
  return os;
}
