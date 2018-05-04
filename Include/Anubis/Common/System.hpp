#ifndef ANUBIS_COMMON_SYSTEM_HPP
#define ANUBIS_COMMON_SYSTEM_HPP

#include "Common/Config.hpp"

//##############################################################################
// WINDOWS HEADERS
//##############################################################################
#if ANUBIS_OS == ANUBIS_OS_WINDOWS
//#define WIN32_LEAN_AND_MEAN
#include<winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
#endif /* ANUBIS_OS == ANUBIS_OS_WINDOWS */

//##############################################################################
// UNIX HEADERS
//##############################################################################
#if ANUBIS_OS == ANUBIS_OS_UNIX
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

typedef int SOCKET;
#define INVALID_SOCKET  -1

#endif /* ANUBIS_OS == ANUBIS_OS_UNIX */


#include "../../../Libraries/Glad/Include/glad/glad.h"

#endif /* ANUBIS_COMMON_SYSTEM_HPP */
