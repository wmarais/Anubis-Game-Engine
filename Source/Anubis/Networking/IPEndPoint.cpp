#include "../../../Include/Anubis/Common/System.hpp"
#include "../../../Include/Anubis/Networking/IPEndPoint.hpp"

using namespace Anubis::Common;
using namespace Anubis::Networking;

/******************************************************************************/
IPEndPoint::IPEndPoint() : fData(nullptr), fDataLen(0)
{
  WSADATA wsa;
  WSAStartup(MAKEWORD(2,2), &wsa);
}

/******************************************************************************/
IPEndPoint::IPEndPoint(uint16_t port, const std::string & nodeName,
                       Preferences pref)
{
  WSADATA wsa;
  WSAStartup(MAKEWORD(2,2), &wsa);

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
    /* Create the data for the address. */
    fDataLen = bestAdrInfo->ai_addrlen;
    fData = std::unique_ptr<uint8_t[]>(new uint8_t[fDataLen]);

    /* Copy the address. */
    memcpy(fData.get(), bestAdrInfo->ai_addr, fDataLen);

    /* Set the address family / protocol version. */
    *reinterpret_cast<uint16_t*>(fData.get()) = htons(family);

    /* Set the port number. */
    *reinterpret_cast<uint16_t*>(fData.get() + sizeof(uint16_t))
        = htons(port);
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
  WSADATA wsa;
  WSAStartup(MAKEWORD(2,2), &wsa);

  /** Indicate if the the IP address is a v4 address or not. */
  fIsV4 = cp.fIsV4;
  fDataLen = cp.fDataLen;

  /* Create the memory for the address object. */
  fData = std::unique_ptr<uint8_t[]>(new uint8_t[fDataLen]);

  /* Copy the address structure. */
  memcpy(fData.get(), cp.fData.get(), fDataLen);
}

/******************************************************************************/
IPEndPoint::~IPEndPoint()
{
  WSACleanup();
}

/******************************************************************************/
std::vector<std::string> IPEndPoint::split(const std::string & str,
                                           char delim)
{
  /* The list of tokens to return. */
  std::vector<std::string> tokens;

  /* The start of the current token. */
  auto tokenStart = str.begin();

  /* Keep generating tokens while there are tokens. */
  do
  {
    /* The end of the current token. */
    auto tokenEnd = std::find(tokenStart, str.end(), delim);

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
void IPEndPoint::inetPToN4(const std::string & src, uint8_t dst[kIPv4OctetCount])
{
  /* Split the IP address into octets. */
  std::vector<std::string> octets = split(src, '.');

  /* Check there is enough octets. */
  if(octets.size() != kIPv4OctetCount)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("An unexpected number of octets have been "
      "supplied for the IPv4 Address: " << src << ", Octets: " <<
      octets.size());
  }

  /* Iterate through each octet. */
  for(size_t i = 0; i < octets.size(); i++)
  {
    /* Convert the current octet string to an octet. */
    dst[i] = static_cast<uint8_t>(std::stoi(octets[i]));
  }
}

/******************************************************************************/
void IPEndPoint::inetPToN6(const std::string & src,
                           uint8_t dst[kIPv6OctetCount])
{
  /* Split the IP address into shorts. */
  std::vector<std::string> valueStrs = split(src, ':');

  /* Make sure there is atleast one token in the list so the maths work without
   * crashing. */
  if(valueStrs.begin() == valueStrs.end())
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("Invalid IPv6 Address specified: " << src);
  }

  /* Check for a leading or tailing :: which signifies 0 value shriking. If
   * the :: exist at either the start or the end of the IP string then it will
   * result in two empty tokens being generated. Since I'm using an empty token
   * to indicate the position of the ::, the extra empty token must be dropped.
   * Since a single : can never exist by itself at the start or the end of the
   * IPv6 string, this is safe to do. */
  if((valueStrs.begin())->length() == 0)
  {
    valueStrs.erase(valueStrs.begin());
  }
  else if((valueStrs.end() - 1)->length() == 0)
  {
    valueStrs.erase(valueStrs.end() - 1);
  }

  /* Zero the memory of the address fields. */
  memset(dst, 0, kIPv6OctetCount);

  /* The current address byte to write. */
  size_t curByte = 0;

  /* Populate the memory. */
  for(auto valueStr : valueStrs)
  {
    /* Check if the current string is not empty. */
    if(valueStr.length() > 0)
    {
      /* Calculate the value of the short. */
      uint16_t value = static_cast<uint16_t>(std::stoi(valueStr, 0, 16));

      /* Write the high byte of the short. */
      dst[curByte++] = Common::Memory::getByte(1, value);

      /* Write the low byte of the short. */
      dst[curByte++] = Common::Memory::getByte(0, value);
    }
    else
    {
      /* Calculate the number of shorts that were shrunk. */
      curByte += sizeof(uint16_t) * (kIPv6ShortsCount - (valueStrs.size() - 1));
    }
  }
}

/******************************************************************************/
IPEndPoint IPEndPoint::makeIPv4(uint16_t port, const std::string &addr)
{
  /* The IPAddress object that will be returned. */
  IPEndPoint ipAddr;

  /* Create the data for the address. */
  ipAddr.fDataLen = sizeof(sockaddr_in);
  ipAddr.fData = std::unique_ptr<uint8_t[]>(new uint8_t[ipAddr.fDataLen]);

  /* Get a reference to the memory. */
  sockaddr_in * sockAddr = reinterpret_cast<sockaddr_in*>(ipAddr.fData.get());

  /* Set the fields of the address. */
  sockAddr->sin_family = htons(AF_INET);
  sockAddr->sin_port = htons(port);

  /* Check if an interface address has been specified. */
  if(addr.length())
  {
    /* Use the specified interface. */
    inetPToN4(addr, reinterpret_cast<uint8_t*>(&(sockAddr->sin_addr.s_addr)));
  }
  else
  {
    /* Use what ever interface the OS selects. */
    sockAddr->sin_addr.s_addr = INADDR_ANY;
  }

  /* Return the created address. */
  return ipAddr;
}

/******************************************************************************/
IPEndPoint IPEndPoint::makeIPv6(uint16_t port, const std::string &addr)
{
  /* The IPAddress object that will be returned. */
  IPEndPoint ipAddr;

  /* Create the data for the address. */
  ipAddr.fDataLen = sizeof(sockaddr_in6);
  ipAddr.fData = std::unique_ptr<uint8_t[]>(new uint8_t[sizeof(sockaddr_in6)]);

  /* Get a reference to the memory. */
  sockaddr_in6 * sockAddr = reinterpret_cast<sockaddr_in6*>(ipAddr.fData.get());

  /* Clear all the fields. */
  memset(sockAddr, 0, sizeof(sockaddr_in6));

  /* Set the common fields. */
  sockAddr->sin6_family = htons(AF_INET6);
  sockAddr->sin6_port = htons(port);

  /* Check if an address is specified. */
  if(addr.length())
  {
    /* Set the IPv6 address. */
    inetPToN6(addr, reinterpret_cast<uint8_t*>(sockAddr->sin6_addr.s6_addr));
  }

  /* Return the created address. */
  return ipAddr;
}

/******************************************************************************/
IPEndPoint & IPEndPoint::operator = (const IPEndPoint & rhs)
{
  /** Indicate if the the IP address is a v4 address or not. */
  fIsV4 = rhs.fIsV4;
  fDataLen = rhs.fDataLen;

  /* Create the memory for the address object. */
  fData = std::unique_ptr<uint8_t[]>(new uint8_t[fDataLen]);

  /* Copy the address structure. */
  memcpy(fData.get(), rhs.fData.get(), fDataLen);

  /* Return the reference to this object. */
  return *this;
}

/******************************************************************************/
bool IPEndPoint::isIPv4() const
{
  /* Check to see if the family type is IPv4. */
  return Memory::fromBigEndian<uint16_t>(fData.get(), fDataLen) == AF_INET;
}

/******************************************************************************/
bool IPEndPoint::isIPv6() const
{
  /* Check to see if the family type is IPv6. */
  return Memory::fromBigEndian<uint16_t>(fData.get(), fDataLen) == AF_INET6;
}

/******************************************************************************/
std::string IPEndPoint::ipStr() const
{
  /* Create a string stream.*/
  std::stringstream ss;

  /* Check if it's IPv4. */
  if(isIPv4())
  {
    /* Convert to the right socket type to get the right address index. */
    sockaddr_in * sockAddr = reinterpret_cast<sockaddr_in*>(fData.get());

    /* Print the address to a string. */
    ss << (unsigned int)(sockAddr->sin_addr.S_un.S_un_b.s_b1) << "." <<
          (unsigned int)(sockAddr->sin_addr.S_un.S_un_b.s_b2) << "." <<
          (unsigned int)(sockAddr->sin_addr.S_un.S_un_b.s_b3) << "." <<
          (unsigned int)(sockAddr->sin_addr.S_un.S_un_b.s_b4);
  }
  else
  {
    /* Convert to the right socket type to get the right address index. */
    sockaddr_in6 * sockAddr = reinterpret_cast<sockaddr_in6*>(fData.get());

    /* Create a vector of the shorts. */
    std::vector<uint16_t> shorts;
    for(size_t i = 0; i < kIPv6ShortsCount; i++)
    {
      shorts.push_back(Memory::fromBigEndian<uint16_t>(
        sockAddr->sin6_addr.u.Byte, kIPv6OctetCount, i * sizeof(uint16_t)));
    }

    /* Find the longest range of 0's. */
    int zerosStart = kIPv6ShortsCount, zerosEnd = kIPv6ShortsCount;

    /* Iterate through all the shorts in the address. */
    for(int i = 0; i < shorts.size(); i++)
    {
      /* Get a temporary start and end position from the current index. */
      int tempZeroStart = i, tempZeroEnd = i;

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
    for(int i = 0; i < shorts.size(); i++)
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
std::ostream & operator << (std::ostream & os, const IPEndPoint & ipAddr)
{
  /* Check if it's an IPv4 Address. */
  os << ipAddr.ipStr();
  return os;
}
