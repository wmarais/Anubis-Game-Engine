#include "../../../Include/Anubis/Common/System.hpp"
#include "../../../Include/Anubis/Networking/IPEndPoint.hpp"
#include "../../../Include/Anubis/Common/System/SocketWrapper.hpp"

using namespace Anubis::Common;
using namespace Anubis::Networking;

/******************************************************************************/
struct Anubis::Networking::IPEndPoint::Data final
{
  /** The result of the winsock startup. For every succesful startup there
   * must be a cleanup. */
  bool fSockLibStarted;

  /** The node name that was supplied. This can be diffirent to the IP address
   * that is used. I.e. if a dns name was specified. */
  std::string fNodeName;

  /** The byte data of the address structure. */
  std::unique_ptr<uint8_t[]> fAddrData;

  /** The length of the data. */
  size_t fAddrDataLen;

  /** Delete the assignment operator to always force the user of the copy
   * constructor. */
  Data & operator = (const Data & rhs) = delete;

  /*************************************************************************//**
   * Initialise the winsock library.
   ****************************************************************************/
  Data();

  Data(const sockaddr * sockAddr, size_t len);

  /*************************************************************************//**
   * Initialise the winsock library and copy the data from the cp object.
   *
   * @param cp  The data object to copy from.
   ****************************************************************************/
  Data(const Data & cp);

  /*************************************************************************//**
   * Shutdown the winsock library.
   ****************************************************************************/
  ~Data();

  /*************************************************************************//**
   * Build the the endpoint data for an IPv4 address.
   *
   * @param src   The address structur from which to copy the IPv4 address.
   * @param port  The port number to use for the end point.
   ****************************************************************************/
  void buildV4Addr(const sockaddr_in * src, uint16_t port);

  /*************************************************************************//**
   * Build the the endpoint data for an IPv6 address.
   *
   * @param src   The address structur from which to copy the IPv6 address.
   * @param port  The port number to use for the end point.
   ****************************************************************************/
  void buildV6Addr(const sockaddr_in6 * src, uint16_t port);

  /*************************************************************************//**
   * This function only exist to make the getaddrinf() function raii safe.
   *
   * @param nodeName  The node name for which the address information must be
   *                  obtained.
   * @return          If an error occured nullptr, else !nullptr.
   ****************************************************************************/
  static struct addrinfo * getAddrInfo(const std::string & nodeName);
};

/******************************************************************************/
IPEndPoint::Data::Data() : fSockLibStarted(false), fAddrDataLen(0)
{
  /* Start the sockets library. */
  Anubis::System::startSocketLib();

  /* Mark the socket library as started. */
  fSockLibStarted = true;
}

/******************************************************************************/
IPEndPoint::Data::Data(const struct sockaddr * sockAddr, size_t len) :
  fSockLibStarted(false), fAddrDataLen(0)
{
  /* Start the sockets library. */
  Anubis::System::startSocketLib();

  /* Mark the socket library as started. */
  fSockLibStarted = true;

  /* Calculate the expected length of the socket data. */
  fAddrDataLen = sockAddr->sa_family == AF_INET ? sizeof(struct sockaddr_in) :
                                                  sizeof(struct sockaddr_in6);

  /* Make sure the expected data length match. */
  if(fAddrDataLen != len)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("Expected address data length ("
    << fAddrDataLen << ") does not match supplied data length " << len << ".");
  }

  /* Create the address data memory. */
  fAddrData = std::unique_ptr<uint8_t[]>(new uint8_t[fAddrDataLen]);

  /* Copy the address data. */
  std::memcpy(fAddrData.get(), sockAddr, fAddrDataLen);
}

/******************************************************************************/
IPEndPoint::Data::Data(const Data & cp) : fSockLibStarted(false),
  fAddrDataLen(0)
{
  /* The Winsock Initialise must run again so we dont up in a situation where
   * the original EndPoint object has been destroyed therby reducing the
   * reference count to 0 and then destroying Winsock. */

  /* Start the sockets library. */
  Anubis::System::startSocketLib();

  /* Mark the socket library as started. */
  fSockLibStarted = true;

  /* Copy the node name. */
  fNodeName = cp.fNodeName;

  /* Copy the data. */
  fAddrDataLen = cp.fAddrDataLen;

  /* Copy the address data only if there is any. */
  if(fAddrDataLen > 0)
  {
    /* Create the address data memory. */
    fAddrData = std::unique_ptr<uint8_t[]>(new uint8_t[fAddrDataLen]);

    /* Copy the address data. */
    std::memcpy(fAddrData.get(), cp.fAddrData.get(), fAddrDataLen);
  }
}

/******************************************************************************/
IPEndPoint::Data::~Data()
{
  try
  {
    /* If the library was initialised, the terminate it. */
    if(fSockLibStarted)
    {
      /* Stop the network library. */
      Anubis::System::stopSocketLib();

      /* Mark the library as stopped. */
      fSockLibStarted = false;
    }
  }
  catch(std::exception & ex)
  {
    ANUBIS_LOG_ERROR(ex.what());
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
  dst->sin_family = AF_INET;
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
  dst->sin6_family = AF_INET6;
}

/******************************************************************************/
struct addrinfo * IPEndPoint::Data::getAddrInfo(const std::string & nodeName)
{
  /* The hints to pass for address information lookup. */
  struct addrinfo hints;

  /* The result from the address info lookup. */
  struct addrinfo * result = nullptr;

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

  /* Return the results pointer. */
  return result;
}

/******************************************************************************/
IPEndPoint::IPEndPoint(void * addrData, size_t dataLen)
{
  /* Create the data object. */
  fData = std::make_unique<Data>(reinterpret_cast<struct sockaddr*>(addrData),
                                 dataLen);
}

/******************************************************************************/
IPEndPoint::IPEndPoint(uint16_t port, const std::string & nodeName,
  Preferences pref) : fData(nullptr)
{
  /* Create the data object. */
  fData = std::make_unique<Data>();

  /* Save the node name. */
  fData->fNodeName = nodeName;

  /* Get a list of suitable addresses for the node name. */
  std::unique_ptr<struct addrinfo, decltype(&freeaddrinfo)> results(
        Data::getAddrInfo(nodeName), freeaddrinfo);

  /* The first IPv4 address that was found. */
  struct addrinfo * addrInfoIPv4 = nullptr;

  /* The first IPv6 address that was found. */
  struct addrinfo * addrInfoIPv6 = nullptr;

  /* Iterate through the linked list of results until both an IPv4 and IPV6
   * dress has been found or we run out of results. */
  for(struct addrinfo * curResult = results.get();
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

  /* Check if there is IPv4 data. */
  if(addrInfoIPv4)
  {
    /* Check if an IPv4 address is adequate. */
    if(pref == Preferences::IPv4Only || pref == Preferences::IPv4 ||
       pref == Preferences::Any)
    {
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
      bestAdrInfo = addrInfoIPv6;
    }
  }
  /* Check if an IPv4 address can be used instead of an IPv6 address. */
  else if(!bestAdrInfo && pref != Preferences::IPv6Only)
  {
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
  /* Create the data object and copy it from cp. */
  fData = std::make_unique<Data>(*(cp.fData.get()));
}

/******************************************************************************/
IPEndPoint::~IPEndPoint() = default;

/******************************************************************************/
IPEndPoint & IPEndPoint::operator = (const IPEndPoint & rhs)
{
  /* Create the data object and copy the data. */
  fData = std::make_unique<Data>(*(rhs.fData.get()));

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
  /* Get a pointer to the address information. */
  struct sockaddr * addr = reinterpret_cast<struct sockaddr*>
      (fData->fAddrData.get());

  /* Check to see if the family type is IPv4. */
  return addr->sa_family == AF_INET;
}

/******************************************************************************/
bool IPEndPoint::isIPv6() const
{
  /* Get a pointer to the address information. */
  struct sockaddr * addr = reinterpret_cast<struct sockaddr*>
      (fData->fAddrData.get());

  /* Check to see if the family type is IPv4. */
  return addr->sa_family == AF_INET;
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

    #if ANUBIS_OS == ANUBIS_OS_WINDOWS
      /* Print the address to a string. */
      ss << (unsigned int)(sockAddr->sin_addr.S_un.S_un_b.s_b1) << "." <<
            (unsigned int)(sockAddr->sin_addr.S_un.S_un_b.s_b2) << "." <<
            (unsigned int)(sockAddr->sin_addr.S_un.S_un_b.s_b3) << "." <<
            (unsigned int)(sockAddr->sin_addr.S_un.S_un_b.s_b4);
    #elif ANUBIS_OS == ANUBIS_OS_UNIX

    #endif

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
      #if ANUBIS_OS == ANUBIS_OS_WINDOWS
        shorts.push_back(Memory::fromBigEndian<uint16_t>(
          sockAddr->sin6_addr.u.Byte, kIPv6OctetCount, i * sizeof(uint16_t)));
      #elif ANUBIS_OS == ANUBIS_OS_UNIX

      #endif
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
