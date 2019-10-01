#pragma once

#include <winsock2.h>
#include <string>
#include <memory>
#include <optional>
#include <atomic>

#include "Buffer.h"

namespace ownfos::network
{
	// -The size of Buffer used on recieving data.
	// -We need this because we can only know the size of a recieved data
	//  after recieve() function is finished, which means that we should prepare
	//  a big buffer that is hopefully bigger than all data that might be sent.
	// -This value determines that size of a "big buffer".
	constexpr int MaxRecieveBufferSize = 4096;

	// -Manages creation, connection/disconnection, and data transmission of winsock TCP/IP SOCKET.
	// -ownfos::network::Initialize() on Utility.h should be called before use.
	// -Copy constructor and assignment operator are deleted!
	//  So use std::shared_ptr<Socket> instead of raw Socket.
	// -Connected socket's SendData()/RecieveData() throwing ownfos::network::DisconnectionDetectedException means
	//  that a peer disconnection has been detected. Use this excetion as disconnection event signal.
	//  This means that every attempt of data transmission will vaildate your connection.
	//
	// -----Example usage (server)-----
	// auto listener = std::make_shared<Socket>();
	// listener->InitializeServer(12345);
	// auto client = listener->AcceptClient();
	//
	// -----Example usage (client)-----
	// auto client = std::make_shared<Socket>();
	// client->ConnectServer(L"127.0.0.1", 12345);
	class Socket
	{
	private:
		SOCKET sock = INVALID_SOCKET;
		std::atomic_bool connected = false;

		// -Used on making new Socket instance from result of Socket::AcceptClient().
		Socket(SOCKET sock);

		void HandleRecieveDataError(int bytesRecieved);

	public:
		// -Create new SOCKET.
		Socket();
		Socket(const Socket& other) = delete;
		~Socket();

		Socket& operator=(const Socket& other) = delete;

		// -Binds socket to current ip with specified port and place it in listening state.
		void InitializeServer(unsigned short port);

		// -Connects to a server on specified ip address and port.
		// -Throws ownfos::network::InvalidIPAddressException if specified ipAddress is not a proper IPv4 address string.
		void ConnectServer(std::wstring ipAddress, unsigned short port);

		// -Accepts incoming ConnectServer() requests.
		// -You should call InitializeServer() before calling this function.
		std::shared_ptr<Socket> AcceptClient();

		// -Sends data to server/client connected on this socket.
		// -This will block thread until peer socket recieves the data or disconnection is detected.
		// -Throws ownfos::network::DisconnectionDetectedException if socket isn't connected yet or is disconnected while sending packet.
		void SendData(Buffer& buffer);

		// -Recieves data from server/client connected on this socket.
		// -Note that there is a limit on recievable buffer size, ownfos::network::MaxRecieveBufferSize.
		// -This will block thread until peer socket sends data or disconnection is detected.
		// -Throws ownfos::network::DisconnectionDetectedException if socket isn't connected yet or is disconnected while waiting for packet.
		Buffer RecieveData();

		// -Disconnect and close socket if connection has been already made.
		// -This is automatically called whenever desctuctor is called,
		//  or Socket::SendData() and Socket::RecieveData() detects a socket disconnection.
		void Disconnect();

		// -Returns host's ip address.
		std::wstring GetAddress();

		// -Returns peer's ip address.
		std::wstring GetPeerAddress();
	};
}
