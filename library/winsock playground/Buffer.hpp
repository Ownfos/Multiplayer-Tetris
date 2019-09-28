#pragma once

#include "Buffer.h"

#include <exception>

namespace ownfos::network
{
	template<typename T>
	void Buffer::Write(T data)
	{
		auto pointer = reinterpret_cast<char*>(&data);
		for (int i = 0; i < sizeof(T); i++)
			buffer.push_back(pointer[i]);
	}

	template<typename T>
	T Buffer::Read()
	{
		if (readPosition + sizeof(T) > buffer.size())
			throw std::exception("no more data available");

		T data;
		auto pointer = reinterpret_cast<char*>(&data);
		for (int i = 0; i < sizeof(T); i++)
		{
			pointer[i] = buffer[readPosition + i];
		}
		readPosition += sizeof(T);

		return data;
	}
}