#pragma once

#include <memory>

class Application;

using ObjectID = unsigned int;

// interface for game objects
class Object
{
private:
	ObjectID id;

public:
	Object(ObjectID id);
	ObjectID ID();

	virtual void OnCreate() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnRender() = 0;
	virtual void OnDestroy() = 0;
};
