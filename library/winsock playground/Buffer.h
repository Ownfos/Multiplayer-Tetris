#pragma once

#include <vector>
#include <string>

namespace ownfos::network
{
	// -A FIFO data structure(like a queue) that Helps building and reading data in socket-sendable form, char array.
	// -All data is managed in char array, so type safty is not guaranteed.
	// Especially be careful when reading data from buffer!
	//
	// -----Example usage-----
	// Buffer buffer;
	// buffer.Write<int>(12345);
	// buffer.WriteWideString(L"Hello, World!");
	// int i = buffer.Read<int>();
	// std::wstring s = buffer.ReadWideString();
	class Buffer
	{
		friend class Socket;

	private:
		// -First 4 byte(int) is reserved for number of bytes in buffer.
		// -this variable will automatically change to 4(actual beginning of data) on Socket::RecieveData().
		int readPosition = 0;
		std::vector<char> buffer;

		void Resize(int size);
		int Size();
		char* Pointer();

		// -Records total number of bytes in this buffer at the beginning.
		void EndWrite();

	public:
		Buffer(int size = 0);

		// -Writes primitive data to buffer.
		// -Note that there's no limit on writing itself, but Socket::RecieveBuffer() does limit it.
		template<typename T>
		void Write(T data);

		// -Writes std::string to buffer.
		// -This will also write the length of the string to buffer,
		//  so that ReadString() can automatically know how many bytes to read.
		void WriteString(std::string string);

		// -Writes std::wstring to buffer.
		// -This will also write the length of the string to buffer,
		//  so that ReadWideString() can automatically know how many bytes to read.
		void WriteWideString(std::wstring string);

		// -Reads data from buffer.
		// -Since all data is stored in char array, no type safety is guaranteed.
		template<typename T>
		T Read();

		// -Reads std::string from buffer.
		std::string ReadString();

		// -Reads std::wstring from buffer.
		std::wstring ReadWideString();
	};
}

#include "Buffer.hpp"