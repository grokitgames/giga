
#ifndef socket_hpp
#define socket_hpp

#ifdef WIN32
#include <Network/Win32UDPSocket.hpp>
typedef Win32UDPSocket UDPSocket;
#elif __APPLE__
#include <Network/PosixUDPSocket.hpp>
typedef PosixUDPSocket UDPSocket;
#elif __linux__
#include <Network/PosixUDPSocket.hpp>
typedef PosixUDPSocket UDPSocket;
#endif

#endif
