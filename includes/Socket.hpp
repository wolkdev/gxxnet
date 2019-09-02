#ifndef SOCKET__HPP
#define SOCKET__HPP

#ifdef _WIN32
#include "NativeSocket/WinSocket.hpp"
#elif __unix__
#include "NativeSocket/UnixSocket.hpp"
#endif

#endif // !SOCKET__HPP