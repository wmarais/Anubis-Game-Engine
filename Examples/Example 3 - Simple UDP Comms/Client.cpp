/*******************************************************************************
 * @brief       Simple UDP client application.
 * @file        Client.cpp
 * @author      Wynand Marais
 * @copyright   WM Software Product License
 * @details     A simple UDP client application to demonstrate the use of the
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
    /* The remote (server) EP to send data too. */
    IPEndPoint serverEP(kPORT, kNODE_NAME, kIPEP_VERSION_PREF);

    /* Create the socket to talk to the server socket. */
    Socket serverSock(Socket::Types::UDP, kSOCK_IP_VERSION);

    /* The vector to first write and then to read too. */
    std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    /* Send the data to the server. */
    serverSock.sendTo(serverEP, data);

    /* Clear the data buffer. */
    data.clear();

    /* Read the data back from the server. */
    serverSock.recvFrom(serverEP, data, 100);

    /*  Shutdown the socket. */
    serverSock.shutdown();

    /* Write the response as a debug message. */
    std::stringstream ss;
    ss << "Server, " << serverEP << ", responded with: ";
    for(auto i : data)
    {
      ss << static_cast<int>(i) << ", ";
    }
    ss << std::endl;
    ANUBIS_LOG_DEBUG(ss.str());
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
