#ifndef ANUBIS_NETWORK_IP_ADDRESS_HPP
#define ANUBIS_NETWORK_IP_ADDRESS_HPP

#include "../Common.hpp"

namespace Anubis
{
  namespace Network
  {
    class IPAddress
    {
      /** The number of octets in an IPv4 Address. */
      static const size_t kIPv4OctetCount = 4;

      /** The number of octets in an IPv6 address. */
      static const size_t kIPv6OctetCount = 16;

      static const size_t kIPv6ShortsCount = 8;

      /** Indicate if the the IP address is a v4 address or not. */
      bool fIsV4;

      /** The data of the socket. */
      std::unique_ptr<uint8_t[]> fData;

      /** The length of the data. */
      size_t fDataLen;

      template<typename T> static void split(const std::string & str,
                                             char delim, T result);

      static std::vector<std::string> split(const std::string & str,
                                            char delim);


      /*********************************************************************//**
       * Implement the inet_pton function for IPv4 addresses.
       *
       * @param src The address string to be converted.
       * @param dst The destination where the converted address will be written
       *            too.
       ************************************************************************/
      static void inetPToN4(const std::string & src,
                            uint8_t dst[kIPv4OctetCount]);

      /*********************************************************************//**
       * Implement the inet_pton function for IPv6 addresses.
       *
       * @param src The address string to be converted.
       * @param dst The destination where the converted address will be written
       *            too.
       ************************************************************************/
      static void inetPToN6(const std::string & src,
                            uint8_t dst[kIPv6OctetCount]);

    public:

      IPAddress();
      IPAddress(const IPAddress & cp);

      /*********************************************************************//**
       * Create an IPv4 Address from the supplied port and address. If the addr
       * string is empty, then any address will be used.
       *
       * @param port  The port to use.
       * @param addr  The IP address to use.
       * @return      The IPAddress object that was created.
       ************************************************************************/
      static IPAddress makeIPv4(uint16_t port, const std::string & addr = "");

      /*********************************************************************//**
       * Create an IPv6 Address from the supplied port and address. If the addr
       * string is empty, then any address will be used.
       *
       * @param port  The port to use.
       * @param addr  The IP address to use.
       * @return      The IPAddress object that was created.
       ************************************************************************/
      static IPAddress makeIPv6(uint16_t port, const std::string & addr = "");

      IPAddress & operator = (const IPAddress & rhs);

      ANUBIS_INLINE const uint8_t * data() const
      {
        return fData.get();
      }

      ANUBIS_INLINE uint8_t * data ()
      {
        return fData.get();
      }

      ANUBIS_INLINE size_t dataLen() const
      {
        return fDataLen;
      }
    };
  }
}

#endif /* ANUBIS_NETWORK_IP_ADDRESS_HPP */
