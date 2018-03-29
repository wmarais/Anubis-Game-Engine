#ifndef ANUBIS_NETWORK_IP_END_POINT_HPP
#define ANUBIS_NETWORK_IP_END_POINT_HPP

#include "../Common.hpp"

namespace Anubis
{
  namespace Networking
  {
    class IPEndPoint
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
      enum class Preferences : int
      {
        /** Only use IPv4 addresses. */
        IPv4Only,

        /** Only use IPv6 addresses. */
        IPv6Only,

        /** Prefer IPv4, but use IPv6 if necisary. */
        IPv4,

        /** Prefer IPv6, but use IPv4 if necisary. */
        IPv6,

        /** Use the first valid address. */
        Any
      };

      IPEndPoint();

      /*********************************************************************//**
       * Create an IP address object from the specified port and node name.
       *
       * The nodeName parameter can be and IPv4 address, an IPv6 address or an
       * DNS name. If an IPv4 or IPv6 address is specified, make sure to set
       * the pref parameter appropriately, specifically don't set and IPv4
       * nodeName and the preference to IPv6Only since this will cause an
       * exception, and visa-versa.
       *
       * @param port      The port to bind or connect too.
       * @param nodeName  The DNS name, IPv4 or IPv6 address of the node.
       * @param pref      The preffered IP version.
       * @return          The created IP address object.
       ************************************************************************/
      IPEndPoint(uint16_t port,
                 const std::string & nodeName = "",
                 Preferences pref = Preferences::Any);

      IPEndPoint(const IPEndPoint & cp);
      ~IPEndPoint();

      /*********************************************************************//**
       * Create an IPv4 Address from the supplied port and address. If the addr
       * string is empty, then any address will be used.
       *
       * @param port  The port to use.
       * @param addr  The IP address to use.
       * @return      The IPAddress object that was created.
       ************************************************************************/
      static IPEndPoint makeIPv4(uint16_t port, const std::string & addr = "");

      /*********************************************************************//**
       * Create an IPv6 Address from the supplied port and address. If the addr
       * string is empty, then any address will be used.
       *
       * @param port  The port to use.
       * @param addr  The IP address to use.
       * @return      The IPAddress object that was created.
       ************************************************************************/
      static IPEndPoint makeIPv6(uint16_t port, const std::string & addr = "");


      IPEndPoint & operator = (const IPEndPoint & rhs);

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

      bool isIPv4() const;

      bool isIPv6() const;

      std::string ipStr() const;
    };
  }
}

std::ostream & operator << (std::ostream & os,
                            const Anubis::Networking::IPEndPoint & ipAddr);

#endif /* ANUBIS_NETWORK_IP_END_POINT_HPP */
