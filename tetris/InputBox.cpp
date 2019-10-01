#include "InputBox.h"

#include "System.h"

using namespace ownfos::graphic;
using namespace ownfos::collision;

InputBox::InputBox
(
	ObjectID							id,
	MyApplication*						application,
	System*								sys,
	std::shared_ptr<Window>				window,
	std::shared_ptr<BasicInputListener> input,
	int									x,
	int									y,
	std::wstring						initialText
) : 
	Object(id), application(application), sys(sys),
	window(window), input(input), x(x), y(y)
{
	textListener = std::make_shared<TextListener>(window, 16, false, false, true, initialText);
	collisionBox = std::make_shared<AABB>
	(
		x * window->PixelWidth(),
		y * window->PixelHeight(),
		100 * window->PixelWidth(),
		10 * window->PixelHeight()
	);
}

void InputBox::OnCreate()
{

}

void InputBox::OnUpdate(float deltaTime)
{
	if (input->FrameState(KeyCode::MouseLeft) == InputState::PressedNow)
	{
		caretBlink = activated = collisionBox->IsPointInside(input->MouseX(), input->MouseY());
		blinkCounter = 0;
		textListener->EnableListening(activated);
	}
	if (activated && ++blinkCounter >= 30)
	{
		blinkCounter = 0;
		caretBlink = !caretBlink;
	}
}

void InputBox::OnRender()
{
	application->RenderBlock(0xffffffff, x, y, 0.0f, 100, 10);
	application->RenderBlock(0x31353bff, x, y, 0.0f, 99, 9);
	application->RenderText(textListener->GetBuffer(), 0xffffffff, -97 + x, 12 + y, 0.0f);
	if (caretBlink)
	{
		std::wstring caret(textListener->GetBuffer());
		auto begin = caret.begin();
		std::advance(begin, textListener->GetCaretPosition());
		caret.erase(begin, caret.end());
		caret.push_back(L'|');
		application->RenderText(caret, 0xffffffff, -97 + x, 12 + y, 0.0f);
	}
}

void InputBox::OnDestroy()
{

}

std::wstring InputBox::GetBuffer()
{
	return textListener->GetBuffer();
}
