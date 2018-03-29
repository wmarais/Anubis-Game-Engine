#include "../../Include/Anubis/Common.hpp"
#include "../../Include/Anubis/Networking.hpp"

using namespace Anubis;
using namespace Anubis::Networking;


void gListenThreadFunc()
{
  /* Create a listen socket. */
}




int main(int argc, char * argv[])
{
  /* The local IP Address (Interface) and port that the socket will bind too. */
  /* IPEndPoint listenAddress = IPEndPoint::makeIPv6(10000, "::1"); */

  IPEndPoint listenAddress = IPEndPoint::makeIPAddr(10000, "localhost", IPEndPoint::Preference::IPv4);

  std::cout << "IP Address: " << listenAddress << std::endl;




}
