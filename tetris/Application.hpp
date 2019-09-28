#include "Application.h"

template<typename T, typename... Args>
ObjectID Application::CreateObject(Args... args)
{
	auto object = std::make_shared<T>(nextID++, args...);
	objectMap[object->ID()] = object;
	object->OnCreate();

	return object->ID();
}

template<typename T>
std::list<T*> Application::FindObjectWithType()
{
	std::list<T*> rtn;

	for (auto [id, object] : objectMap)
		if (auto ptr = dynamic_cast<T>(object.get()); ptr != nullptr)
			rtn.push_back(ptr);

	return rtn;
}
