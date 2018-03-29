#include "../../../Include/Anubis/Common/System.hpp"
#include "../../../Include/Anubis/Networking/Socket.hpp"

using namespace Anubis::Networking;

/******************************************************************************/
struct Anubis::Networking::Socket::Data
{
  /* Lock to control access to the WSAStartup function. I'm not sure if this is
   * required but there seems to be no explicit statement about thread safety,
   * so not taking any chances. */
  static std::mutex fWSAStartupMutex;

  /** The result of the winsock startup. For every succesful startup there
   * must be a cleanup. */
  int fWSAStartupResult;

  /** The address family to use. */
  int fAddressFamily;

  /** The Winsock socket type that was created. */
  int fType;

  /** The protocol to use with the socket. */
  int fProtocol;

  /** The handle to the socket object. */
  SOCKET fHandle;

  /** Default constructo to intialise the important fields. */
  Data() : fWSAStartupResult(-1), fHandle(INVALID_SOCKET) {}
};

/******************************************************************************/
Socket::Socket(std::unique_ptr<Data> & data)
{
  /* Set the socket memory. */
  fData = std::move(data);
}

/******************************************************************************/
Socket::Socket(Types type, IPVersions version)
{
  /** Create the data object of the socket. */
  fData = std::make_unique<Data>();

  /* Convert the enum socket type (Types) to a winsock socket type and
   * protocol. */
  fData->fType = SOCK_DGRAM;
  fData->fProtocol = IPPROTO_UDP;

  /* Check if TCP is required. */
  if(type == Types::TCP)
  {
    /* Configure for TCP. */
    fData->fType = SOCK_STREAM;
    fData->fProtocol = IPPROTO_TCP;
  }

  /* The default family type is IPv4. */
  fData->fAddressFamily = AF_INET;

  /* Check if IPv6 is instead required. */
  if(version == IPVersions::IPv6)
  {
    /* Set the version to IPv6. */
    fData->fAddressFamily = AF_INET6;
  }

  /* Lock the startup mutex. */
  std::lock_guard<std::mutex> lock(Data::fWSAStartupMutex);

  ANUBIS_LOG_DEBUG("Initialising Winsock 2.0.");
  WSADATA wsa;
  fData->fWSAStartupResult = WSAStartup(MAKEWORD(2,2), &wsa);

  /* Check if the socket library was started up properly. */
  if(fData->fWSAStartupResult != 0)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("WSAStartup failed with error code: " <<
                                  fData->fWSAStartupResult);
  }

  ANUBIS_LOG_DEBUG("Creating socket.");
  fData->fHandle = socket(fData->fAddressFamily, fData->fType,
                          fData->fProtocol);

  /* Check if the socket was created or not. */
  if(fData->fHandle == INVALID_SOCKET)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("socket() failed with error code: " <<
                                   WSAGetLastError());
  }
}

/******************************************************************************/
Socket::~Socket()
{
  /* Check the data is valid. */
  if(!fData)
  {
    /* Can't do anything without valid data. */
    return;
  }

  /* Check the result of the startup, dont call cleanup unless it was
   * sucessful. */
  if(fData->fWSAStartupResult == 0)
  {
    /* Check if the socket is open. */
    if(fData->fHandle != INVALID_SOCKET)
    {
      /* Close the socket and log any errors. */
      if(closesocket(fData->fHandle) != 0)
      {
        ANUBIS_LOG_ERROR("closesocket() failed with error code: " <<
                         WSAGetLastError());
      }
    }

    /* Cleanup and log any errors. */
    if(WSACleanup() != 0)
    {
      ANUBIS_LOG_ERROR("WSACleanup() failed with error code: " <<
                       WSAGetLastError());
    }
  }
}

/******************************************************************************/
void Socket::bind(const IPEndPoint &addr)
{
  /* Check the bind result. */
  if(::bind(fData->fHandle, reinterpret_cast<const sockaddr*>(addr.data()),
            addr.dataLen()) != 0)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("bind() failed with error code: " <<
                                   WSAGetLastError());
  }

  /* Record the address that this socket is bound too. */
  fAddress = addr;
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

  std::unique_ptr<uint8_t[]> addr = nullptr;
  int addrLen = 0;

  /* Check if the current socket is an IPv4 socket. */
  if(fData->fAddressFamily == AF_INET)
  {
    /* Calculate the length of an IPv4 address. */
    addrLen = sizeof(sockaddr_in);
  }
  else
  {
    /* Calculate the length of an IPv6 address. */
    addrLen = sizeof(sockaddr_in6);
  }

  /* Create the memory to store the socket data. */
  addr = std::unique_ptr<uint8_t[]>(new uint8_t[addrLen]);

  /* Accept the connection and read the address of the client. */
  SOCKET clientHandle = accept(fData->fHandle,
                               reinterpret_cast<struct sockaddr*>(addr.get()),
                               &addrLen);

  /* Create the socket data. */
  std::unique_ptr<Data> data = std::make_unique<Data>();

  /* Set the properties of the client socket. */
  data->fAddressFamily = fData->fAddressFamily;
  data->fType = fData->fType;
  data->fProtocol = fData->fProtocol;
  data->fHandle = clientHandle;

  /* Return the connected socket. */
  return std::make_unique<Socket>(data);
}

/******************************************************************************/
bool Socket::connect(const IPEndPoint & addr)
{
  ANUBIS_LOG_DEBUG("Attempting to connect to server.");
  if(::connect(fData->fHandle,
               reinterpret_cast<const struct sockaddr*>(addr.data()),
               addr.dataLen()) == 0)
  {
    ANUBIS_LOG_DEBUG("Connection successful!");
    return true;
  }

  ANUBIS_LOG_DEBUG("Connection failed with error code: " << WSAGetLastError());
  return false;
}

/******************************************************************************/
void Socket::send(const std::vector<uint8_t> & data)
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

    /* Check if and error occured. */
    if(result < 0)
    {
      ANUBIS_THROW_RUNTIME_EXCEPTION("send() failed with error code: " <<
                                     WSAGetLastError());
    }
    else
    {
      /* Increment the number of bytes sent. */
      bytesSent += result;
    }
  }
}

/******************************************************************************/
void Socket::recv(std::vector<uint8_t> & data, size_t len)
{
  /* Resize the buffer based on what was expected to be read. */
  data.resize(len);

  /* Read the bytes. */
  int bytesRead = ::recv(fData->fHandle,
                         reinterpret_cast<char*>(data.data()), len, 0);

  /* Check if an error occured. */
  if(bytesRead < 0)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("recv() failed with error code: " <<
                                   WSAGetLastError());
  }
  else
  {
    /* Resize the buffer based on what was read. This should only every be
     * called when the socket has been shutdown during a read, at which point
     * the bytesRead is expected to be 0. */
    data.resize(bytesRead);
  }
}

/******************************************************************************/
void Socket::sendTo(const IPEndPoint & addr, const std::vector<uint8_t> & data)
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
                        reinterpret_cast<const struct sockaddr*>(addr.data()),
                        addr.dataLen());

    /* Check if and error occured. */
    if(result < 0)
    {
      ANUBIS_THROW_RUNTIME_EXCEPTION("sendto() failed with error code: " <<
                                     WSAGetLastError());
    }
    else
    {
      /* Increment the number of bytes sent. */
      bytesSent += result;
    }
  }
}

/******************************************************************************/
void Socket::recvFrom(IPEndPoint & addr, std::vector<uint8_t> & data,
                      size_t len)
{
  /* Resize the buffer based on what was expected to be read. */
  data.resize(len);

  /* The length of the address structure. */
  int addrLen = addr.dataLen();

  /* Read the bytes. */
  int bytesRead = recvfrom(fData->fHandle,
                          reinterpret_cast<char*>(data.data()), len, 0,
                          reinterpret_cast<struct sockaddr*>(addr.data()),
                          &addrLen);

  /* Check if an error occured. */
  if(bytesRead < 0)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("recv() failed with error code: " <<
                                   WSAGetLastError());
  }
  else
  {
    /* Resize the buffer based on what was read. This should only every be
     * called when the socket has been shutdown during a read, at which point
     * the bytesRead is expected to be 0. */
    data.resize(bytesRead);

    /* Verify that the size of the addresses match. */
    if(static_cast<size_t>(addrLen) != addr.dataLen())
    {
      ANUBIS_THROW_RUNTIME_EXCEPTION("Missmatched sockaddr size!");
    }
  }
}
