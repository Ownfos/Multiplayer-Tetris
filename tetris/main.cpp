#pragma comment(lib, "../library/collision playground.lib")
#pragma comment(lib, "../library/winsock playground.lib")
#pragma comment(lib, "../library/directx11 playground.lib")

#include "MyApplication.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		MyApplication application(60);

		return application.Run();
	}
	catch (std::exception & exception)
	{
		MessageBox(NULL, exception.what(), "Error", MB_OK);
		return -1;
	}
}