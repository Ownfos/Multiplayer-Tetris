#pragma once

#include <dxgi.h>
#include <string>
#include <tuple>

namespace ownfos::graphic
{
	constexpr float PI = 3.141592f;

	void Initialize();

	void Uninitialize();

	std::wstring GetFullPath(std::wstring fileName);

	int BytesPerPixel(DXGI_FORMAT format);

	std::string GetErrorMessage(DWORD errorCode);

	// -Converts degree to radian
	float Radian(float degree);
	
	// -Converts radian to degree
	float Degree(float radian);
}