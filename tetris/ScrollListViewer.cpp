#include "ScrollListViewer.h"

#include "../library/collision playground/AABB.h"

using namespace ownfos::graphic;
using namespace ownfos::collision;

ScrollListViewer::ScrollListViewer
(
	ObjectID												id,
	MyApplication* application,
	System* system,
	std::shared_ptr<ownfos::graphic::Window>				window,
	std::shared_ptr<ownfos::graphic::BasicInputListener>	input
) :
	Object(id), MouseWheelListener(window), application(application),
	system(system), window(window), input(input)
{
}

void ScrollListViewer::UpdateData(std::list<std::wstring> data)
{
	std::lock_guard guard(dataMutex);
	this->data = data;
	currentAlpha = 0.0f;
}

void ScrollListViewer::OnCreate()
{
}

void ScrollListViewer::OnUpdate(float deltaTime)
{
	currentAlpha += (targetAlpha - currentAlpha) * 0.1f;
	if (std::fabsf(targetAlpha - currentAlpha) < 0.05f)
		currentAlpha = targetAlpha;

	currentY += (targetY - currentY) * 0.3f;
	if (std::fabsf(targetY - currentY) < 0.05f)
		currentY = targetY;

	if (input->FrameState(KeyCode::MouseLeft) == InputState::PressedNow)
	{
		std::lock_guard guard(dataMutex);
		auto pos = data.begin();
		for (auto i = 0; i < data.size(); i++)
		{
			AABB box
			(
				-200 * window->PixelWidth(),
				(100 + currentY - 40 * i) * window->PixelHeight(),
				100 * window->PixelWidth(),
				blockHeight / 2 * window->PixelHeight()
			);

			if (box.IsPointInside(input->MouseX(), input->MouseY()))
			{
				system->SendMatchRequest(*pos);
				break;
			}
			pos++;
		}
	}
}

void ScrollListViewer::OnRender()
{
	int i = 0;
	std::lock_guard guard(dataMutex);
	for (auto name : data)
	{
		application->RenderBlock
		(
			{ 1.0f, 1.0f, 1.0f, currentAlpha },
			-200,
			100 + currentY - 40 * i,
			0.0f,
			100,
			blockHeight / 2
		);

		application->RenderBlock
		(
			0x31353bff,
			-200,
			100 + currentY - 40 * i,
			0.0f,
			99,
			blockHeight / 2 - 1
		);

		application->RenderText
		(
			name,
			{ 1.0f, 1.0f, 1.0f, currentAlpha },
			-297,
			100 + currentY - 40 * i + blockHeight / 2 - 3,
			0.0f
		);
		++i;
	}
}

void ScrollListViewer::OnDestroy()
{
}

void ScrollListViewer::OnWheelUp()
{
	targetY += blockHeight;
}

void ScrollListViewer::OnWheelDown()
{
	targetY -= blockHeight;
}