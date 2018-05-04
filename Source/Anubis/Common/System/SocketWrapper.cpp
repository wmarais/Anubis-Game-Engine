#include "../../../../Include/Anubis/Common/System/SocketWrapper.hpp"

/******************************************************************************/
std::string Anubis::System::getSockErrorMsg(int errCode)
{
  #if ANUBIS_OS == ANUBIS_OS_WINDOWS
    switch(errCode)
    {
      case WSAENETDOWN:
        return "Error Code: 10050 (WSAENETDOWN), Meaning: The network subsystem "
               "has failed.";

      case WSASYSNOTREADY:
        return "Error Code: 10091 (WSASYSNOTREADY), Meaning: The underlying "
               "network subsystem is not ready for network communication.";
      case WSAVERNOTSUPPORTED:
        return "Error Code: 10092 (WSAVERNOTSUPPORTED), Meaning: The version of "
               "Windows Sockets support requested is not provided by this "
               "particular Windows Sockets implementation.";

      case WSANOTINITIALISED:
        return "Error Code: 10093 (WSANOTINITIALISED), Meaning: A successful "
               "WSAStartup call must occur before using this function.";

      case WSAEINPROGRESS:
        return "Error Code: 10036 (WSAEINPROGRESS), Meaning: A blocking Windows "
               "Sockets 1.1 operation is in progress.";

      case WSAEPROCLIM:
        return "Error Code: 10067 (WSAEPROCLIM), Meaning: A limit on the number "
               "of tasks supported by the Windows Sockets implementation has "
               "been reached.";

      case WSAEFAULT:
        return "Error Code: 10014 (WSAEFAULT), Meaning: The lpWSAData parameter "
               "is not a valid pointer.";

      default:
        return "An unknown error occured.";
    }
  #endif /* ANUBIS_OS == ANUBIS_OS_WINDOWS */
}

/******************************************************************************/
void Anubis::System::startSocketLib()
{
  #if ANUBIS_OS == ANUBIS_OS_WINDOWS
    /* Lock the mutex. */
    std::lock_guard<std::mutex> lock(gStartStopSocketLibMutex);

    /* Return the result of the initialisation. */
    WSADATA wsa;

    /* Start Winsock. */
    int result = WSAStartup(MAKEWORD(2,2), &wsa);

    /* Check if an error occured. */
    if(result != 0)
    {
      ANUBIS_THROW_RUNTIME_EXCEPTION(getSockErrorMsg(result));
    }
  #endif /* ANUBIS_OS == ANUBIS_OS_WINDOWS */
}

/******************************************************************************/
void Anubis::System::stopSocketLib()
{
  #if ANUBIS_OS == ANUBIS_OS_WINDOWS
    /* Shut down the sockets lib. */
    if(WSACleanup() != 0)
    {
      /* Write an error out to the log file. */
      ANUBIS_THROW_RUNTIME_EXCEPTION(
            Anubis::System::getSockErrorMsg(WSAGetLastError()));
    }
  #endif /* ANUBIS_OS == ANUBIS_OS_WINDOWS */
}
