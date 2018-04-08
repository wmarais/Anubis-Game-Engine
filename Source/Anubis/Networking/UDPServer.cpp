#include "../../../Include/Anubis/Networking/UDPServer.hpp"

using namespace Anubis::Networking;

/******************************************************************************/
UDPPacket::UDPPacket(size_t maxPktLen) : kMaxPktLen(maxPktLen)
{
  /* Reserver enough space in the vector. */
  fData.reserve(kMaxPktLen);

  /* Set the minimum size of the packet. */
  fData.resize(kMinPktLen);

  /* Clear the memory. */
  memset(fData.data(), 0, kMaxPktLen);
}

/******************************************************************************/
void UDPPacket::clear()
{
  /* Reset the size of the packet back to it's minimum size. */
  fData.resize(kMinPktLen);

  /* Clear the memory. */
  memset(fData.data(), 0, kMaxPktLen);
}

/******************************************************************************/
void UDPPacket::setSeqNumber(uint16_t seqNum)
{
  writeLE<uint16_t>(fData.data() + kSeqNumIndex,
                    fData.size() - kSeqNumIndex, seqNum);
}

/******************************************************************************/
uint16_t UDPPacket::getSeqNumber() const
{
  return readLE<uint16_t>(fData.data() + kSeqNumIndex,
                          fData.size() - kSeqNumIndex);
}

/******************************************************************************/
void UDPPacket::setAckNum(uint16_t ackNum)
{
  writeLE<uint16_t>(fData.data() + kAckNumIndex,
                    fData.size() - kAckNumIndex, ackNum);
}

/******************************************************************************/
uint16_t UDPPacket::getAckNum() const
{
  return readLE<uint16_t>(fData.data() + kAckNumIndex,
                          fData.size() - kAckNumIndex);
}

/******************************************************************************/
void UDPPacket::Chunk::setState(uint8_t state)
{

}

/******************************************************************************/
uint8_t UDPPacket::Chunk::getState() const
{

}

void UDPPacket::Chunk::setPosition(float x, float y)
{

}

void UDPPacket::Chunk::getPosition(float & x, float & y)
{

}

void UDPPacket::Chunk::setTargets(const std::vector<uint16_t> & targets)
{

}

void UDPPacket::Chunk::getTargets(const std::vector<uint16_t> & targets)
{

}























///******************************************************************************/
//UDPServer::UDPServer(IPEndPoint localEP, size_t maxPktLen)
//{

//}

///******************************************************************************/
//UDPServer::~UDPServer()
//{

//}
