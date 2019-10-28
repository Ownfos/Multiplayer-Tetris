#include "Object.h"

#include "Application.h"

Object::Object(ObjectID id) : id(id)
{
}

ObjectID Object::ID()
{
	return id;
}