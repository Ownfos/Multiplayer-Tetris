#pragma once

#include <string>

namespace ownfos::network
{
	// -Possible causes :
	//  1. Current socket has lost connection while trying to send or recieve data, which means
	//     (hopefully intended) Socket::Disconnect() is called from either host or peer.
	//     Note that Socket::SendData() might not throw this exception immediately if Socket::Disconnect() was call from peer, not host.
	//     In this case, the second Socket::SendData() call after disconnection will probably throw this exception.
	class DisconnectionDetectedException
	{
	private:
		std::string message;

	public:
		DisconnectionDetectedException(std::string message);
		std::string what() const noexcept;
	};

	// -Possible causes :
	//  1. InetPtonW() failed to parse give IPv4 address.
	class InvalidIPAddressException
	{
	public:
		std::string what() const noexcept;
	};
}