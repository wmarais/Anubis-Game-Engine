#include "../../Include/Anubis/Common.hpp"
#include "../../Include/Anubis/Networking.hpp"

using namespace Anubis;
using namespace Anubis::Networking;

/* Uncomment this to use IPv4 or comment it out to use IPv6. */
#define USE_IP_V4

/******************************************************************************/
/* Select the appropriate EP Prefecent and Porotocl Version. */
#ifdef USE_IP_V4
  IPEndPoint::Preferences gSockPref = IPEndPoint::Preferences::IPv4Only;
  Socket::Versions gSockVersion = Socket::Versions::IPv4;
#else
  IPEndPoint::Preferences gSockPref = IPEndPoint::Preferences::IPv6Only;
  Socket::Versions gSockVersion = Socket::Versions::IPv6;
#endif /* USE_IP_V4 */

/******************************************************************************/
int main(int argc, char * argv[])
{
  try
  {
    /* The remote EP to connect too. */
    IPEndPoint serverEP(10000, "localhost", gSockPref);

    /* Create the socket to talk to the server socket. */
    Socket serverSock(Socket::Types::TCP, gSockVersion);

    /* Connect to the server. */
    serverSock.connect(serverEP);

    /* The vector to read data too. */
    std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    /* Send the data to the server. */
    serverSock.send(data);

    /* Clear the data buffer. */
    data.clear();

    /* Read the data back from the server. */
    serverSock.recv(data, 10);

    /* Write the response as a debug message. */
    std::stringstream ss;
    ss << "Server responded with: ";
    for(auto i : data)
    {
      ss << static_cast<int>(i) << ", ";
    }
    ss << std::endl;
    ANUBIS_LOG_DEBUG(ss.str());


    /*  Shutdown the connection. */
    serverSock.shutdown();
  }
  catch(std::exception & ex)
  {
    /* Log the error. */
    ANUBIS_LOG_FATAL_ERROR(ex.what());

    /* Exit with a failure. */
    return EXIT_FAILURE;
  }

  /* Return with success. */
  return EXIT_SUCCESS;
}
