#ifndef ANUBIS_NETWORK_SOCKET_HPP
#define ANUBIS_NETWORK_SOCKET_HPP

#include "../Common.hpp"
#include "IPEndPoint.hpp"

namespace Anubis
{
  namespace Networking
  {
    /***********************************************************************//**
     * A wrapper class around the sockets() layer to abstract away Winsock and
     * Posix socket interfaces. This class is designed to only work with IP
     * sockets.
     **************************************************************************/
    class Socket
    {
      /** A class to store the platform specific information of the socket. */
      struct Data;

      /** The instance of the platform specific data. */
      std::unique_ptr<Data> fData;


      Socket(std::unique_ptr<Data> & data);

    public:

      /** The list of supported Socket Types. */
      enum class Types
      {
        /** Standard UDP. */
        UDP,

        /** Standard TCP. */
        TCP
      };

      enum class Versions : int
      {
        IPv4  = 4,
        IPv6  = 6
      };

      /*********************************************************************//**
       * Create a socket using the specified socket type and IP version. If
       * useIPv4 == true, then an IPv4 socket is created, if useIPv4 == false,
       * then an IPv6 socket is created.
       *
       * @param type    The type of packets / flow control to use.
       * @param useIPv4 Specify if IPv4 must be used (true), else specify that
       *                IPv6 must be used (false).
       ************************************************************************/
      Socket(Types type, Versions version = Versions::IPv4);

      /*********************************************************************//**
       * Destroy the allocated data and close the socket.
       ************************************************************************/
      virtual ~Socket();

      /*********************************************************************//**
       * Bind the socket to the specific local port and interface. If the
       * address is empty, then any interface will be used.
       *
       * @param port  The local port to bind too.
       * @param iface The local interface to bind too.
       ************************************************************************/
      void bind(const IPEndPoint & ep);

      IPEndPoint getEP() const;

      /*********************************************************************//**
       * Listen for a client trying to connect. This is used by TCP servers to
       * listen for connection requests.
       *
       * @param backlog The number of concurrent connection requests.
       * @return        The accepted client socket.
       ************************************************************************/
      std::unique_ptr<Socket> listen(size_t backlog);

      /*********************************************************************//**
       * @brief connect
       * @param addr
       ************************************************************************/
      bool connect(const IPEndPoint & ep);

      void shutdown();

      bool send(const std::vector<uint8_t> & data);

      bool recv(std::vector<uint8_t> & data, size_t len);

      void sendTo(const IPEndPoint & ep, const std::vector<uint8_t> & data);

      void recvFrom(IPEndPoint & ep, std::vector<uint8_t> & data,
                    size_t len);






    };
  }
}

#endif /* ANUBIS_NETWORK_SOCKET_HPP */
