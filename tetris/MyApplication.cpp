#include "MyApplication.h"

#include <ctime>

#include "BlockRenderer.h"
#include "TextRenderer.h"
#include "System.h"

using namespace ownfos::graphic;

MyApplication::MyApplication(int fps) : Application(fps), randomEngine(time(NULL))
{
	graphic = std::make_shared<Graphic>
		(
			D3D11_CREATE_DEVICE_DEBUG,
			D3D10_CREATE_DEVICE_DEBUG
			);
	window = std::make_shared<Window>(graphic, 600, 400, "window name", false);
	input = std::make_shared<BasicInputListener>(window);
	blockRenderer = std::make_shared<BlockRenderer>(graphic, window);
	textRenderer = std::make_shared<TextRenderer>
		(
			graphic,
			window,
			graphic->CreateTextFormat(L"Yu gothic", 15.0f)
			);

	CreateObject<System>(this, window, input);
}

void MyApplication::OnFrameBegin()
{
	window->Clear(0x31353bff);
}

void MyApplication::OnFrameEnd()
{
	window->Present();
}

void MyApplication::RenderBlock
(
	ownfos::graphic::Color	color,
	int						centerX,
	int						centerY,
	float					centerZ,
	int						halfWidth,
	int						halfHeight
)
{
	blockRenderer->RenderBlock
	(
		color,
		centerX,
		centerY,
		centerZ,
		halfWidth,
		halfHeight
	);
}

void MyApplication::RenderText
(
	std::wstring			text,
	ownfos::graphic::Color	color,
	int		x,
	int		y,
	float	z
)
{
	textRenderer->RenderText(text, color, x, y, z);
}

int MyApplication::RandomInt(int start, int end)
{
	std::uniform_int_distribution<int> distribution(start, end);
	return distribution(randomEngine);
}