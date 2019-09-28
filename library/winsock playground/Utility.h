#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <string>

namespace ownfos::network
{
	void Initialize();

	void Uninitialize();

	// -Converts error code from GetLastError() or WSAGetLastError() to error message string.
	std::string GetErrorMessage(DWORD errorCode);
}