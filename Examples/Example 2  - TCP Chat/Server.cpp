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
  IPEndPoint listenEP(10000, "localhost", IPEndPoint::Preferences::IPv6);

  /* Print the end point to see the address details. */
  std::cout << "Listen End-Point: " << listenEP << std::endl;

}
