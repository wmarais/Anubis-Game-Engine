/*******************************************************************************
 * @brief       Simple TCP client application.
 * @file        Client.cpp
 * @author      Wynand Marais
 * @copyright   WM Software Product License
 * @details     A simple TCP client application to demonstrate the use of the
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
    /* Create the socket to talk to the server. */
    Socket serverSock(Socket::Types::TCP, kSOCK_IP_VERSION);

    /* Connect to the server. */
    serverSock.connect(IPEndPoint(kPORT, kNODE_NAME, kIPEP_VERSION_PREF));

    /* The vector to read data too. */
    std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    /* Send the data to the server. */
    serverSock.send(data);

    /* Clear the data buffer. */
    data.clear();

    /* Read the data back from the server. */
    serverSock.recv(data, 10);

    /* Shutdown the connection. */
    serverSock.shutdown();

    /* Write the response as a debug message. */
    std::stringstream ss;
    ss << "Server responded with: ";
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
