#ifndef ANUBIS_COMMON_SYSTEM_SOCKET_WRAPPER_HPP
#define ANUBIS_COMMON_SYSTEM_SOCKET_WRAPPER_HPP

#include "../System.hpp"
#include "../Misc.hpp"
#include "../Log.hpp"

namespace Anubis
{
  namespace System
  {
    /* Lock to control access to the WSAStartup function. I'm not sure if this is
     * required but there seems to be no explicit statement about thread safety,
     * so not taking any chances. */
    static std::mutex gStartStopSocketLibMutex;

    std::string getSockErrorMsg(int errCode);

    void startSocketLib();
    void stopSocketLib();
  }
}

#endif /* ANUBIS_COMMON_SYSTEM_SOCKET_WRAPPER_HPP */
