#include "../../../Include/Anubis/Networking/TCPServer.hpp"

using namespace Anubis::Networking;

/******************************************************************************/
TCPServer::TCPServer(IPEndPoint localEP) :
  fIsExecuting(true)
{
  try
  {
    /* Create the listen socket. */
    fListenSocket = std::make_unique<Socket>(Socket::Types::TCP, localEP);

    /* Create the thread to listen for new connections. */
    fThread = std::thread(&TCPServer::listenThread, this);
  }
  catch(...)
  {
    /* Clear the execution flag. */
    fIsExecuting = false;
  }
}

/******************************************************************************/
TCPServer::~TCPServer()
{

}

/******************************************************************************/
void TCPServer::listenThread()
{

}
