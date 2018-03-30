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

      /** The Address of this Socket. */
      IPEndPoint fLocalEP;
    public:

      /** The list of supported Socket Types. */
      enum class Types
      {
        /** Standard UDP. */
        UDP,

        /** Standard TCP. */
        TCP
      };

      Socket(std::unique_ptr<Data> & data);

      /*********************************************************************//**
       * Create a socket using the specified socket type and IP version.
       *
       * @param type    The type of packets / flow control to use.
       * @param version The IP protocol version to use. The defual is IPv4.
       ************************************************************************/
      Socket(Types type, const IPEndPoint & localEP = IPEndPoint(0, ""));

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
      void bind(const IPEndPoint & localEP);

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
      bool connect(const IPEndPoint & addr);

      void send(const std::vector<uint8_t> & data);

      void recv(std::vector<uint8_t> & data, size_t len);

      void sendTo(const IPEndPoint & addr, const std::vector<uint8_t> & data);

      void recvFrom(IPEndPoint &addr, std::vector<uint8_t> & data,
                    size_t len);






    };
  }
}

#endif /* ANUBIS_NETWORK_SOCKET_HPP */
