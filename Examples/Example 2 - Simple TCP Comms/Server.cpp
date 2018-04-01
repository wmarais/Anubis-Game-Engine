/*******************************************************************************
 * @brief       Simple TCP server application.
 * @file        Server.cpp
 * @author      Wynand Marais
 * @copyright   WM Software Product License
 * @details     A simple TCP server application to demonstrate the use of the
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
    /* Create the listen socket. */
    Socket listenSock(Socket::Types::TCP, kSOCK_IP_VERSION);

    /* Bind the socket to the interface. */
    listenSock.bind(IPEndPoint(kPORT, kNODE_NAME, kIPEP_VERSION_PREF));

    /* Wait for a child to connect. */
    std::unique_ptr<Socket> child = listenSock.listen(1);

    /* The vector to read data too. */
    std::vector<uint8_t> data;

    /* Keep reading from the socket while it is connected. */
    while(child->recv(data, 1))
    {
      /* Echo the data straight back. */
      child->send(data);

      /* Clear the recieved data buffer. */
      data.clear();
    }
  }
  catch(std::exception & ex)
  {
    /* Log the error. */
    ANUBIS_LOG_FATAL_ERROR(ex.what());

    /* Pause the thread. */
    Anubis::pause();

    /* Exit with a failure. */
    return EXIT_FAILURE;
  }

  /* Pause the thread. */
  Anubis::pause();

  /* Return with success. */
  return EXIT_SUCCESS;
}
