
#include <giga-engine.h>
#include <WinSock2.h>

Win32UDPSocket::Win32UDPSocket() {
	memset(&m_sockaddr, 0, sizeof(sockaddr_in));
}

Win32UDPSocket::~Win32UDPSocket() {
	if (m_socket) {
		closesocket(m_socket);
		m_socket = 0;
	}
}

bool Win32UDPSocket::Connect(std::string server, int port) {
	WSADATA wsaData;

	// Startup winsock
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	// Create a socket
	m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_socket < 0) {
		ErrorSystem::Process(new Error(ERROR_WARN, (char*)"Unable to create socket for server", server));
		return(false);
	}

	// Get the host IP in a computer usable home
	struct hostent* host;
	host = gethostbyname(server.c_str());
	if (host == 0) {
		ErrorSystem::Process(new Error(ERROR_WARN, (char*)"Unable to get host", server));
		return(false);
	}

	// Create our server structure
	m_sockaddr.sin_family = AF_INET;
	memcpy((void *)&m_sockaddr.sin_addr, host->h_addr_list[0], host->h_length);
	m_sockaddr.sin_port = htons(port);

	// Connect
	if (::connect(m_socket, (struct sockaddr*)&m_sockaddr, sizeof(m_sockaddr)) < 0) {
		ErrorSystem::Process(new Error(ERROR_WARN, (char*)"Unable to connect to host", server));
		return(false);
	}

	unsigned long mode = 1;
	ioctlsocket(m_socket, FIONBIO, &mode);

	return(true);
}

int Win32UDPSocket::Write(unsigned char* buffer, int length) {
	int ret = ::send(m_socket, (char*)buffer, length, 0);
	return(ret);
}

int Win32UDPSocket::Read(void* buffer, int length) {
	int ret = ::recv(m_socket, (char*)buffer, length, 0);
	return(ret);
}

void Win32UDPSocket::Close() {
	if (m_socket) {
		closesocket(m_socket);
		m_socket = 0;
	}
}
