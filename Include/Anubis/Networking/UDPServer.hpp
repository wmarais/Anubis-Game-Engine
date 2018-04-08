#ifndef ANUBIS_NETWORKING_UDP_SERVER_HPP
#define ANUBIS_NETWORKING_UDP_SERVER_HPP

#include "Socket.hpp"
#include "IPEndPoint.hpp"
#include "../Common/Misc.hpp"

namespace Anubis
{
  namespace Networking
  {
    class UDPPacket
    {
      /** The maximum length of any UDP packet. */
      const size_t kMaxPktLen;

      /** The minimum length of the packet in bytes. The minimum size include
       * enough space to store the Type (uint16_t), SeqNum (uint16_t) and
       * AckNum (uint16_t). */
      static const size_t kMinPktLen = 6;

      /** The type of the packet. */
      static const size_t kTypeIndex = 0;

      /** The index of the sequence number in the packet. */
      static const size_t kSeqNumIndex = 2;

      /** The index of the ack number in the packet. */
      static const size_t kAckNumIndex = 4;

      /* The data of the packet. */
      std::vector<uint8_t> fData;

    public:



      /*********************************************************************//**
       * Create a new packet and initialise the contents to null.
       *
       * @param maxPktLen The maximum length of the packet in bytes.
       ************************************************************************/
      UDPPacket(size_t maxPktLen = 512);

      /*********************************************************************//**
       * Clear the contents of the packet.
       ************************************************************************/
      void clear();

      /*********************************************************************//**
       * Set the sequence number of the packet.
       *
       * @param seqNum  The value to set the sequece number too.
       ************************************************************************/
      void setSeqNumber(uint16_t seqNum);

      /*********************************************************************//**
       * Retrieves the sequence number of the packet.
       *
       * @return  The sequence number of the packet.
       ************************************************************************/
      uint16_t getSeqNumber() const;

      /*********************************************************************//**
       * Set the ack number of the last packet to ack.
       *
       * @param ackNum  The last packet to ack.
       ************************************************************************/
      void setAckNum(uint16_t ackNum);



      uint16_t getAckNum() const;

      class Chunk
      {
        std::vector<uint8_t> fData;

      public:
        void setState(uint8_t state);
        uint8_t getState() const;

        void setPosition(float x, float y);
        void getPosition(float & x, float & y);

        void setTargets(const std::vector<uint16_t> & targets);
        void getTargets(const std::vector<uint16_t> & targets);
      };
    };



//    class UnitState
//    {
//      uint16_t fID;
//      float x, y;





//      uint8_t fData;

//    }





    class UDPServer
    {
      /** The maximum size of the client circular queue. */
      static const size_t kMaxPktQueueLen = 4096;


      /** The maximum length of any datagram that is sent or recieved. */
      const size_t kMaxPktLen;

      struct Client
      {
        /** The UUID of the client. */
        Common::UUID fUUID;

        /** The End Point of the client. */
        std::unique_ptr<IPEndPoint> fEndPoint;

        /** The thread safe RX packet queue. */
        Common::CircularQueue<std::vector<uint8_t>, kMaxPktQueueLen> fRxQueue;

        /** The thread safe TX packet queue. */
        Common::CircularQueue<std::vector<uint8_t>, kMaxPktQueueLen> fTxQueue;
      };

      /** The socket used to send / recieve datagrams. */
      std::unique_ptr<Socket> fSocket;


      UDPServer(const UDPServer &) = delete;
      UDPServer & operator = (const UDPServer &) = delete;

    public:

      /*********************************************************************//**
       * Initialise the new UDPServer by creating the comms socket and binding
       * it to the specified localEP.
       *
       * @param localEP   The local end point to use for communication.
       * @param maxPktLen The maximum length (in bytes) of any packet that is
       *                  send or received. The default value of 512 has been
       *                  specifically tuned to try and avoid fragmentation of
       *                  the packet. In networks where jumbo frames etc are
       *                  used, this can be set to a much higher value.
       ************************************************************************/
      UDPServer(IPEndPoint localEP, size_t maxPktLen = 512);

      ~UDPServer();



    };
  }
}

#endif /* ANUBIS_NETWORKING_UDP_SERVER_HPP */
