#pragma once

#include "..//library/directx11 playground/Window.h"
#include "..//library/directx11 playground/FPSManager.h"

#include <list>
#include <unordered_map>
#include <memory>
#include <optional>
#include <mutex>

#include "Object.h"

using ObjectID = unsigned int;

class Application
{
private:
	ownfos::graphic::FPSManager fpsManager;
	std::unordered_map<ObjectID, std::shared_ptr<Object>> objectMap;
	std::list<ObjectID> deleteList;
	ObjectID nextID = 0;
	std::mutex objectMapMutex;

public:
	Application(int fps);
	virtual ~Application();

	virtual void OnFrameBegin() = 0;
	virtual void OnFrameEnd() = 0;

	int Run();

	template<typename T, typename... Args>
	ObjectID CreateObject(Args... args);

	void DestroyObject(ObjectID id);

	template<typename T>
	std::list<T*> FindObjectWithType();

	Object* FindObjectWithID(ObjectID id);


};

#include "Application.hpp"