#ifndef ANUBIS_NETWORKING_TCP_SERVER_HPP
#define ANUBIS_NETWORKING_TCP_SERVER_HPP

#include "../Common.hpp"
#include "Socket.hpp"

namespace Anubis
{
  namespace Networking
  {
    class TCPServer
    {
      struct Client
      {
        /** Define the maximum queue length for packet reception. */
        static const size_t kMaxRxQueueLen = 4096;

        /** Define the maximum queue length for packet transmission. */
        static const size_t kMaxTxQueueLen = 4096;

        /** The unique ID of the connected client. This can be used to uniquely
         * identify a user. */
        Common::UUID fID;

        /** The name of the connected client. */
        std::string fName;

        /** The socket of the connected client. */
        std::unique_ptr<Socket> fSocket;

        /** The queue where all the recieved packets are stored. */
        Common::CircularQueue<std::vector<uint8_t>, kMaxRxQueueLen> fRxQueue;

        /** The circular queue where all the tx packets are stored. */
        Common::CircularQueue<std::vector<uint8_t>, kMaxTxQueueLen> fTxQueue;

        /** The mutex used to control access to the Rx queue. */
        std::mutex fRxMutex;

        /** The mutex used to control access to the client queue. */

        /** The thread used to read data from the client. */
        std::thread fRxThread;

        /** The thread used to write data to the client. */
        std::thread fTxThread;

      public:

        Client(std::unique_ptr<Socket> & socket);
        ~Client();
      };

      /** The socket to listen on for new connections. */
      std::unique_ptr<Socket> fListenSocket;

      /** The list of connected clients. */
      std::vector<std::unique_ptr<Client>> fClients;

      std::thread fThread;

      std::atomic_bool fIsExecuting;

      /** The monitoring thread. */
      void listenThread();

    public:

      TCPServer(IPEndPoint localEP);
      ~TCPServer();

    };
  }
}

#endif /* ANUBIS_NETWORKING_TCP_SERVER_HPP */
