#pragma once

#include "..//library/directx11 playground/MouseWheelListener.h"

#include "Object.h"
#include "MyApplication.h"
#include "System.h"

namespace
{
	constexpr float blockHeight = 40;
}

class ScrollListViewer : public Object, public ownfos::graphic::MouseWheelListener
{
private:
	MyApplication* application;
	System* system;
	std::shared_ptr<ownfos::graphic::Window>				window;
	std::shared_ptr<ownfos::graphic::BasicInputListener>	input;
	std::mutex												dataMutex;
	std::list<std::wstring>									data;
	float													targetAlpha = 1.0f;
	float													currentAlpha = 0.0f;
	float													targetY = 0;
	float													currentY = 0;

public:
	ScrollListViewer
	(
		ObjectID												id,
		MyApplication* application,
		System* system,
		std::shared_ptr<ownfos::graphic::Window>				window,
		std::shared_ptr<ownfos::graphic::BasicInputListener>	input
	);

	// updates client list to display
	void UpdateData(std::list<std::wstring> data);

	void OnCreate() override;

	void OnUpdate(float deltaTime) override;

	void OnRender() override;

	void OnDestroy() override;

	void OnWheelUp() override;

	void OnWheelDown() override;
};