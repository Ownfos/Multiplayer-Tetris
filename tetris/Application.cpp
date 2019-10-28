#include "Application.h"

#include "..//library/directx11 playground/Utility.h"
#include "..//library/winsock playground/Utility.h"

Application::Application(int fps) : fpsManager(fps)
{
	ownfos::graphic::Initialize();
	ownfos::network::Initialize();
}

Application::~Application()
{
	try
	{
		for (auto [id, object] : objectMap)
			object->OnDestroy();

		objectMap.clear();

		ownfos::graphic::Uninitialize();
		ownfos::network::Uninitialize();
	}
	catch (std::exception & exception)
	{
		MessageBox(NULL, exception.what(), "", MB_OK);
	}
}

int Application::Run()
{
	try
	{
		while (true)
		{
			if (auto exitCode = ownfos::graphic::Window::ProcessMessage(); exitCode.has_value())
				return exitCode.value();

			if (fpsManager.IsNextFrame())
			{
				std::lock_guard<std::mutex> lockGuard(objectMapMutex);

				OnFrameBegin();

				for (auto [id, object] : objectMap)
					object->OnUpdate(fpsManager.GetDeltaTime() / 1000000000.0f);
				for (auto [id, object] : objectMap)
					object->OnRender();

				OnFrameEnd();

				for (auto id : deleteList)
					objectMap.erase(id);
			}
		}
	}
	catch (std::exception & exception)
	{
		MessageBox(NULL, exception.what(), "Error on application's loop", MB_OK);
		return -1;
	}
}

void Application::DestroyObject(ObjectID id)
{
	deleteList.push_back(id);
}

Object* Application::FindObjectWithID(ObjectID id)
{
	if (objectMap.find(id) == objectMap.end())
		return nullptr;
	else
		return objectMap[id].get();
}