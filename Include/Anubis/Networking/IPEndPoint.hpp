#ifndef ANUBIS_NETWORK_IP_END_POINT_HPP
#define ANUBIS_NETWORK_IP_END_POINT_HPP

#include "../Common.hpp"

namespace Anubis
{
  namespace Networking
  {
    /** Forward declare the the socket class so it can be made a friend of the
     * IPEndPoint class. */
    class Socket;

    /***********************************************************************//**
     * A class for containing the address information of an IP End Point. The
     * class can accept IPv4, IPv6 and DNS addresses directly.
     **************************************************************************/
    class IPEndPoint final
    {
      /** Make the socket class a friend of the IPEndPoint. This is mostly
       * reduce the public interface that is available to developers (otherwise
       * dangerouse functions and memory has to be exposed). */
      friend class Socket;

      /** The number of octets (uint8_t / bytes) in an IPv4 Address. */
      static const size_t kIPv4OctetCount = 4;

      /** The number of octets (uint8_t / bytes) in an IPv6 address. */
      static const size_t kIPv6OctetCount = 16;

      /** The number of shorts (uint16_t) in an IPv6 address. */
      static const size_t kIPv6ShortsCount = 8;

      /** A class to store the platform specific information of the socket. */
      struct Data;

      /** The instance of the platform specific data. */
      std::unique_ptr<Data> fData;

      /*********************************************************************//**
       * Return the pointer to the OS specific address data. This is used
       * directly by the Socket class.
       *
       * @return  The pointer to the address data.
       ************************************************************************/
      const uint8_t * addrData() const;

      /*********************************************************************//**
       * Return the length of the OS specific address data. This is used
       * directly by the Socket class.
       *
       * @return  The length of the address data.
       ************************************************************************/
      const size_t addrDataLen() const;

    public:
      enum class Preferences : int
      {
        /** Only use IPv4 addresses. */
        IPv4Only  = 0,

        /** Only use IPv6 addresses. */
        IPv6Only  = 1,

        /** Prefer IPv4, but use IPv6 if necisary. */
        IPv4      = 2,

        /** Prefer IPv6, but use IPv4 if necisary. */
        IPv6      = 3,

        /** Use IPv4 if possible, else IPv6. */
        Any       = 4
      };

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
      IPEndPoint(uint16_t port = 0,
                 const std::string & nodeName = "",
                 Preferences pref = Preferences::Any);

      /*********************************************************************//**
       * Perform a deep copy of the cp endpoint object.
       *
       * @param cp  The object to copy.
       ************************************************************************/
      IPEndPoint(const IPEndPoint & cp);

      /*********************************************************************//**
       * The destructor is not used for anything, but is necisary for
       * compilation.
       ************************************************************************/
      ~IPEndPoint();

      /*********************************************************************//**
       * Perform a deep copy of the rhs endpoint object.
       *
       * @param cp  The object to copy.
       ************************************************************************/
      IPEndPoint & operator = (const IPEndPoint & rhs);

      /*********************************************************************//**
       * Check if the IPEndPoint is configured an IPv4 address.
       *
       * @return  True if an IPv4 address is used, else False.
       ************************************************************************/
      bool isIPv4() const;

      /*********************************************************************//**
       * Check if the IPEndPoint is configured with an IPv6 address.
       *
       * @return  True if an IPv6 address is used, else false.
       ************************************************************************/
      bool isIPv6() const;

      /*********************************************************************//**
       * Return the node name that was supplied to the constructor. This is what
       * should be saved to a settings file (not much value in saving the ip
       * string returned by ip()).
       *
       * @return  The node name of the end point.
       ************************************************************************/
      std::string nodeName() const;

      /*********************************************************************//**
       * Return the port number that was supplied to the constructor. This
       * should be saved to the settings file for a Server Socket, but optional
       * for client sockets.
       *
       * @return  The port number of the end point.
       ************************************************************************/
      uint16_t port() const;

      /*********************************************************************//**
       * Return the IP address that is used. This can be diffirent to the node
       * name specified.
       *
       * @return  The IP address as a string.
       ************************************************************************/
      std::string ip() const;
    };
  }
}

/***************************************************************************//**
 * Provide a global overload for the
 * @param os
 * @param ipAddr
 * @return
 ******************************************************************************/
std::ostream & operator << (std::ostream & os,
                            const Anubis::Networking::IPEndPoint & ep);

#endif /* ANUBIS_NETWORK_IP_END_POINT_HPP */
