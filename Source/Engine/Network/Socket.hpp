
#ifndef socket_hpp
#define socket_hpp

#ifdef WIN32
typedef Win32UDPSocket UDPSocket;
#elif __APPLE__
typedef PosixUDPSocket UDPSocket;
#elif __linux__
typedef PosixUDPSocket UDPSocket;
#endif

#endif
