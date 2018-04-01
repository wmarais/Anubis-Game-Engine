#include "../../../Include/Anubis/Common/System/SocketWrapper.hpp"
#include "../../../Include/Anubis/Networking/Socket.hpp"

using namespace Anubis::Networking;

/******************************************************************************/
struct Socket::Data final
{
  /** Indicate if this object initialised the socket library. */
  bool fSockLibStarted;

  /** The handle to the socket object. */
  SOCKET fHandle;

  /** Default constructor to intialise the important fields. */
  Data(SOCKET handle);

  Data(uint16_t family, int type, int protocol);
  ~Data();

  Data(const Data & cp) = delete;
  Data & operator = (const Data & rhs) = delete;
};

/******************************************************************************/
Socket::Data::Data(SOCKET handle) : fHandle(handle)
{
  /* Start the socket library. */
  Anubis::System::startSocketLib();

  /* Mark the socket library as started. */
  fSockLibStarted = true;
}

/******************************************************************************/
Socket::Data::Data(uint16_t family, int type, int protocol) :
  fHandle(INVALID_SOCKET)
{
  /* Start the socket library. */
  Anubis::System::startSocketLib();

  /* Mark the socket library as started. */
  fSockLibStarted = true;

  /* Create the socket. */
  fHandle = socket(family, type, protocol);

  /* Check if the socket was created or not. */
  if(fHandle == INVALID_SOCKET)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("socket() failed with error code: " <<
                                   WSAGetLastError());
  }

  /* Allow address reuse. */
  BOOL optVal = TRUE;
  if(setsockopt(fHandle, SOL_SOCKET, SO_REUSEADDR,
                reinterpret_cast<char*>(&optVal), sizeof(BOOL)) != 0)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("setsockopt() failed with error code: " <<
                                   WSAGetLastError());
  }
}

/******************************************************************************/
Socket::Data::~Data()
{
  /* Check if the socket is open. */
  if(fHandle != INVALID_SOCKET)
  {
    /* Close the socket and log any errors. */
    if(closesocket(fHandle) != 0)
    {
      ANUBIS_LOG_ERROR("closesocket() failed with error code: " <<
                       WSAGetLastError());
    }
  }

  try
  {
    /* If the library was initialised, the terminate it. */
    if(fSockLibStarted)
    {
      /* Stop the network library. */
      Anubis::System::stopSocketLib();

      /* Mark the library as stopped. */
      fSockLibStarted = false;
    }
  }
  catch(std::exception & ex)
  {
    ANUBIS_LOG_ERROR(ex.what());
  }
}

/******************************************************************************/
Socket::Socket(std::unique_ptr<Data> & data)
{
  /* Set the socket memory. */
  fData = std::move(data);
}

/******************************************************************************/
Socket::Socket(Types type, Versions version)
{
  /* Create the socket object. */
  fData = std::make_unique<Data>(
        version == Versions::IPv4 ? AF_INET : AF_INET6,
        type == Types::TCP ? SOCK_STREAM : SOCK_DGRAM,
        type == Types::TCP ? IPPROTO_TCP : IPPROTO_UDP);
}

/******************************************************************************/
Socket::~Socket() = default;

/******************************************************************************/
void Socket::bind(const IPEndPoint & ep)
{
  ANUBIS_LOG_DEBUG("Binding to: " << ep);

  /* Bind the socket. */
  int result = ::bind(fData->fHandle, reinterpret_cast<const struct sockaddr*>
                      (ep.addrData()), ep.addrDataLen());

  /* Check the bind result. */
  if(result != 0)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("bind() failed with error code: " <<
                                   result);
  }
}

/******************************************************************************/
IPEndPoint Socket::getEP() const
{
  /* A location to store the socket address. */
  struct sockaddr_storage addrData;

  /* The length of the address. */
  socklen_t addrLen = 0;

  /* Read the socket's address information. */
  if(getsockname(fData->fHandle,
                 reinterpret_cast<struct sockaddr*>(&addrData),
                 &addrLen) != 0)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("getsockname() failed with error code: " <<
                                   WSAGetLastError());
  }

  /* Return the IP endpoint information of the socket. */
  return IPEndPoint(&addrData, addrLen);
}

/******************************************************************************/
std::unique_ptr<Socket> Socket::listen(size_t backlog)
{
  /* Listen for a connection. */
  if(::listen(fData->fHandle, backlog) != 0)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("listen() failed with error code: " <<
                                   WSAGetLastError());
  }

  /* Accept the connection and read the address of the client. */
  SOCKET clientHandle = accept(fData->fHandle, nullptr, nullptr);

  /* Create the data object for the socket. */
  std::unique_ptr<Data> data = std::make_unique<Data>(clientHandle);

  /* Return the connected socket. */
  return std::unique_ptr<Socket>(new Socket(data));
}

/******************************************************************************/
bool Socket::connect(const IPEndPoint & ep)
{
  ANUBIS_LOG_DEBUG("Attempting to connect to server.");
  if(::connect(fData->fHandle,
               reinterpret_cast<const struct sockaddr*>(ep.addrData()),
               ep.addrDataLen()) == 0)
  {
    ANUBIS_LOG_DEBUG("Connection successful!");
    return true;
  }

  ANUBIS_LOG_DEBUG("Connection failed with error code: " << WSAGetLastError());
  return false;
}

/******************************************************************************/
void Socket::shutdown()
{
  /* Shutdown the socket. */
  if(::shutdown(fData->fHandle, SD_SEND) != 0)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("shutdown() failed with error code: " <<
                                   WSAGetLastError());
  }
}

/******************************************************************************/
bool Socket::send(const std::vector<uint8_t> & data)
{
  /* The number of bytes that were sent. */
  size_t bytesSent = 0;

  /* Keep looping until all bytes are sent. */
  while(bytesSent != data.size())
  {
    /* Send as many bytes as possible. */
    int result = ::send(fData->fHandle,
                        reinterpret_cast<const char*>(data.data() + bytesSent),
                        data.size() - bytesSent, 0);

    /* Check if the bytes were sent. */
    if(result > 0)
    {
      /* Increment the number of bytes sent. */
      bytesSent += result;
    }
    /* Check if the connection was closed. */
    else if(result == 0)
    {
      /* Return false to indicate that the connection was closed. */
      return false;
    }
    /* Otherwise an error occured. */
    else
    {
      ANUBIS_THROW_RUNTIME_EXCEPTION("send() failed with error code: " <<
                                     WSAGetLastError());
    }
  }

  /* Return the number of bytes sent. */
  return bytesSent;
}

/******************************************************************************/
bool Socket::recv(std::vector<uint8_t> & data, size_t len)
{
  /* Resize the buffer based on what was expected to be read. */
  data.resize(len);

  /* Read the bytes. */
  size_t bytesRead = 0;

  /* Keep reading until all the requested data is read. */
  while(bytesRead != len)
  {
    /* Perform a read. */
    int result = ::recv(fData->fHandle,
                        reinterpret_cast<char*>(data.data() + bytesRead),
                        len - bytesRead, 0);

    /* Check if an error occured. */
    /* Check if data was read. */
    if(result > 0)
    {
      /* Increment the bytes read counter. */
      bytesRead += result;
    }
    /* Check if the socket was closed. */
    else if(result == 0)
    {
      /* Retur false to indicate that the socket was closed. */
      return false;
    }
    /* Else and error occured. */
    else
    {
      ANUBIS_THROW_RUNTIME_EXCEPTION("recv() failed with error code: " <<
                                     WSAGetLastError());
    }
  }

  /* Resize the buffer based on what was read. This should only every be
   * called when the socket has been shutdown during a read, at which point
   * the bytesRead is expected to be 0. */
  data.resize(bytesRead);

  /* Return true to indicate that the data was read. */
  return true;
}

/******************************************************************************/
bool Socket::sendTo(const IPEndPoint & ep, const std::vector<uint8_t> & data)
{
  /* The number of bytes that were sent. */
  size_t bytesSent = 0;

  /* Keep looping until all bytes are sent. */
  while(bytesSent != data.size())
  {
    /* Send as many bytes as possible. */
    int result = sendto(fData->fHandle,
                        reinterpret_cast<const char*>(data.data() + bytesSent),
                        data.size() - bytesSent, 0,
                        reinterpret_cast<const struct sockaddr*>(ep.addrData()),
                        ep.addrDataLen());

    /* Check if some bytes were sent. */
    if(result > 0)
    {
      /* Increment the bytes sent. */
      bytesSent += result;
    }
    /* Else check if the socket was shutdown. */
    else if(result == 0)
    {
      /* Return false to indicate that the socket was shut down. */
      return false;
    }
    /* Else an error occured. */
    else
    {
      ANUBIS_THROW_RUNTIME_EXCEPTION("sendto() failed with error code: " <<
                                     WSAGetLastError());
    }
  }

  /* Return true to indicate that the data was sent. */
  return true;
}

/******************************************************************************/
bool Socket::recvFrom(IPEndPoint & ep, std::vector<uint8_t> & data,
                      size_t maxLen)
{
  /* A location to store the socket address. */
  struct sockaddr_storage addrData;

  /* The length of the address. */
  int addrDataLen = sizeof(struct sockaddr_storage);

  /* Resize the buffer based on what was expected to be read. */
  data.resize(maxLen);

  /* Read the bytes. */
  int result = recvfrom(fData->fHandle,
                        reinterpret_cast<char*>(data.data()), data.size(), 0,
                        reinterpret_cast<struct sockaddr*>(&addrData),
                        &addrDataLen);

  /* Check if the packet was read. */
  if(result > 0)
  {
    /* Resize the vector. */
    data.resize(result);

    /* Set the EP where the data came from. */
    ep = IPEndPoint(&addrData, addrDataLen);

    /* Return true to indicate that the datagram was read. */
    return true;
  }
  /* Else check if it was gracefully shutdown. */
  else if(result == 0)
  {
    /* Return false to indicate the shutdown. */
    return false;
  }

  /* Else an error occured. */
  ANUBIS_THROW_RUNTIME_EXCEPTION("recvfrom() failed with result: " << result
    << ", error code: " << WSAGetLastError() << ".");
}
