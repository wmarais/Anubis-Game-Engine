#ifndef ANUBIS_COMMON_SYSTEM_HPP
#define ANUBIS_COMMON_SYSTEM_HPP

#include "Common/Config.hpp"

#if ANUBIS_OS == ANUBIS_OS_WINDOWS
//#define WIN32_LEAN_AND_MEAN
#include<winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
#endif /* ANUBIS_OS == ANUBIS_OS_WINDOWS */

#include "../../../Libraries/Glad/Include/glad/glad.h"

#endif /* ANUBIS_COMMON_SYSTEM_HPP */
