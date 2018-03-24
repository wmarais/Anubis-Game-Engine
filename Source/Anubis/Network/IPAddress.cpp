#include "../../../Include/Anubis/Common/System.hpp"
#include "../../../Include/Anubis/Network/IPAddress.hpp"

using namespace Anubis::Network;

/******************************************************************************/
IPAddress::IPAddress() : fData(nullptr), fDataLen(0)
{
}

/******************************************************************************/
IPAddress::IPAddress(const IPAddress & cp)
{
  /** Indicate if the the IP address is a v4 address or not. */
  fIsV4 = cp.fIsV4;
  fDataLen = cp.fDataLen;

  /* Create the memory for the address object. */
  fData = std::unique_ptr<uint8_t[]>(new uint8_t[fDataLen]);

  /* Copy the address structure. */
  memcpy(fData.get(), cp.fData.get(), fDataLen);
}

/******************************************************************************/
template<typename T> void IPAddress::split(const std::string & str,
                                             char delim, T result)
{
  std::stringstream ss(str);
  std::string item;
  while (std::getline(ss, item, delim))
  {
    *(result++) = item;
  }
}

/******************************************************************************/
std::vector<std::string> IPAddress::split(const std::string & str, char delim)
{
  std::vector<std::string> elems;
  split(str, delim, std::back_inserter(elems));
  return elems;
}

/******************************************************************************/
void IPAddress::inetPToN4(const std::string & src, uint8_t dst[kIPv4OctetCount])
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
void IPAddress::inetPToN6(const std::string & src, uint8_t dst[kIPv6OctetCount])
{
  /* Split the IP address into shorts. */
  std::vector<std::string> shorts = split(src, ':');

  /* The current octet index. */
  size_t curOctet = 0;

  /* Iterate through all the shorts. */
  for(size_t i = 0; i < shorts.size(); i++)
  {
    /* Make sure not to go out of bounds. */
    if(curOctet >= kIPv6OctetCount)
    {
      ANUBIS_THROW_RUNTIME_EXCEPTION("The specified IPv6 address is too long: "
        << src);
    }

    /* Check if the current short has a value. */
    if(shorts[i].length())
    {
      /* Calculate the value of the short. */
      uint16_t value = static_cast<uint16_t>(std::stoi(shorts[i], 0, 16));

      /* Write the high byte. */
      dst[curOctet++] = Common::Memory::getByte(1, value);
      dst[curOctet++] = Common::Memory::getByte(0, value);
    }
    /* This will only ever happen for a "::" token. */
    else
    {
      /* Calculate the number of shorts that must be padded. */
      size_t padding = kIPv6ShortsCount - (shorts.size() - 1);

      /* Set the padding bytes to zero. */
      memset(dst + curOctet, 0, padding * sizeof(uint16_t));

      /* Add the offset to the octet index. */
      curOctet += padding * sizeof(uint16_t);
    }
  }
}

/******************************************************************************/
IPAddress IPAddress::makeIPv4(uint16_t port, const std::string &addr)
{
  /* The IPAddress object that will be returned. */
  IPAddress ipAddr;

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
IPAddress IPAddress::makeIPv6(uint16_t port, const std::string &addr)
{
  /* The IPAddress object that will be returned. */
  IPAddress ipAddr;

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
    /* Check if it's the IPv6 loopback address. */
    if(addr == "::1")
    {
      /* Copy the global loopback address. */
       memcpy(sockAddr->sin6_addr.s6_addr, in6addr_loopback.s6_addr,
              sizeof(in6addr_loopback));
    }
    else
    {
      /* Set the IPv6 address. */
      inetPToN6(addr,
                reinterpret_cast<uint8_t*>(sockAddr->sin6_addr.s6_addr));
    }
  }

  /* Return the created address. */
  return ipAddr;
}

/******************************************************************************/
IPAddress & IPAddress::operator = (const IPAddress & rhs)
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
