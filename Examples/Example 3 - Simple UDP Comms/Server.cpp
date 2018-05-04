/*******************************************************************************
 * @brief       Simple UDP server application.
 * @file        Server.cpp
 * @author      Wynand Marais
 * @copyright   WM Software Product License
 * @details     A simple UDP server application to demonstrate the use of the
 *              Socket and IPEndPoint classes. The application can be configured
 *              for either IPv4 (by uncommenting #define USE_IP_V4) or IPv6 (by
 *              commenting out #define USE_IP_V4).
 ******************************************************************************/
#include "../../Include/Anubis/Common.hpp"
#include "../../Include/Anubis/Networking.hpp"

using namespace Anubis;
using namespace Anubis::Networking;

/* Uncomment this to use IPv4 or comment it out to use IPv6. */
#define USE_IP_V4

/* The port to listen on for data. */
#define kPORT               10000

/* The interface to listen on for data. */
#define kNODE_NAME          "localhost"

/* The maximum size of a packet. */
#define kMAX_DATAGRAM_SIZE  100

/******************************************************************************/
/* Select the appropriate EP Prefecent and Porotocl Version. */
#ifdef USE_IP_V4
  #define kIPEP_VERSION_PREF  IPEndPoint::Preferences::IPv4Only
  #define kSOCK_IP_VERSION    Socket::Versions::IPv4
#else
  #define kIPEP_VERSION_PREF  IPEndPoint::Preferences::IPv6Only
  #define kSOCK_IP_VERSION    Socket::Versions::IPv6
#endif /* USE_IP_V4 */

/******************************************************************************/
int main(int argc, char * argv[])
{
  ANUBIS_UNUSED_VAR(argc);
  ANUBIS_UNUSED_VAR(argv);

  try
  {
    /* Create the server's socket to send / recieve datagrams to clients. */
    Socket socket(Socket::Types::UDP, kSOCK_IP_VERSION);

    /* Bind the socket to the specified Port and Interface (node name).*/
    socket.bind(IPEndPoint(kPORT, kNODE_NAME, kIPEP_VERSION_PREF));

    /* Create a vector to store the data read from the client. */
    std::vector<uint8_t> data;

    /* Create an end point to record the address of the client that send the
     * datagram / packet. */
    IPEndPoint peerEP;

    /* Read a packet of data. When using UDP, a whole packet or no packet is
     * read and thus loop is not required to read all the data. */
    if(socket.recvFrom(peerEP, data, kMAX_DATAGRAM_SIZE))
    {
      /* Log the client that the datagram was recieved from. */
      ANUBIS_LOG_DEBUG("Datagram recieved from: " << peerEP);

      /* Echo the packet back to the client. */
      socket.sendTo(peerEP, data);
    }

    /* Shutdown the socket. */
    socket.shutdown();
  }
  catch(std::exception & ex)
  {
    /* Log the error. */
    ANUBIS_LOG_FATAL_ERROR(ex.what());

    /* Wait for the user to press enter. */
    Anubis::pause();

    /* Exit with a failure. */
    return EXIT_FAILURE;
  }

  /* Wait for the user to press enter. */
  Anubis::pause();

  /* Return with success. */
  return EXIT_SUCCESS;
}
