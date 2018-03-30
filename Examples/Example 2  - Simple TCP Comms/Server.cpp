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

    /* The local IP Address (Interface) and port that the socket will bind
     * too. */
    IPEndPoint localEP(10000, "localhost", gSockPref);

    /* Create the listen socket. */
    Socket listenSock(Socket::Types::TCP, gSockVersion);

    /* Bind the socket to the loopback interface. */
    listenSock.bind(localEP);

    /* Wait for a child to connect. */
    std::unique_ptr<Socket> child = listenSock.listen(1);

    /* The vector to read data too. */
    std::vector<uint8_t> data;

    /* Keep reading from the socket whilte it is connected. */
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

    /* Exit with a failure. */
    return EXIT_FAILURE;
  }

  /* Return with success. */
  return EXIT_SUCCESS;
}
