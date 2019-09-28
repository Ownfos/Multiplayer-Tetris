#pragma once

#include "..//library/collision playground/AABB.h"

#include "..//library/directx11 playground/Window.h"
#include "..//library/directx11 playground/BasicInputListener.h"
#include "..//library/directx11 playground/TextListener.h"

#include "Object.h"
#include "MyApplication.h"

class System;

// recieves text input.
// activates when clicked.
class InputBox : public Object
{
private:
	MyApplication*											application;
	System*													sys;
	std::shared_ptr<ownfos::graphic::Window>				window;
	std::shared_ptr<ownfos::graphic::BasicInputListener>	input;
	std::shared_ptr<ownfos::graphic::TextListener>			textListener;
	std::shared_ptr<ownfos::collision::AABB>				collisionBox;
	int														x;
	int														y;
	bool													activated = true;
	bool													caretBlink = true;
	int														blinkCounter = 0;
	int														phase = 0;

public:
	InputBox
	(
		ObjectID												id,
		MyApplication*											application,
		System*													sys,
		std::shared_ptr<ownfos::graphic::Window>				window,
		std::shared_ptr<ownfos::graphic::BasicInputListener>	input,
		int														x,
		int														y,
		std::wstring											initialText
	);

	void OnCreate() override;

	void OnUpdate(float deltaTime) override;

	void OnRender() override;

	void OnDestroy() override;

	std::wstring GetBuffer();
};