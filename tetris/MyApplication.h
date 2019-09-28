#pragma once

#include "../library/winsock playground/Socket.h"

#include "..//library/directx11 playground/Graphic.h"
#include "..//library/directx11 playground/Window.h"
#include "..//library/directx11 playground/BasicInputListener.h"

#include <random>

#include "Application.h"

class BlockRenderer;
class TextRenderer;

class MyApplication : public Application
{
private:
	std::shared_ptr<ownfos::graphic::Graphic>				graphic;
	std::shared_ptr<ownfos::graphic::Window>				window;
	std::shared_ptr<ownfos::graphic::BasicInputListener>	input;
	std::shared_ptr<BlockRenderer>							blockRenderer;
	std::shared_ptr<TextRenderer>							textRenderer;
	std::default_random_engine								randomEngine;

public:
	MyApplication(int fps);

	void OnFrameBegin() override;

	void OnFrameEnd() override;

	void RenderBlock
	(
		ownfos::graphic::Color	color,
		int						centerX,
		int						centerY,
		float					centerZ,
		int						halfWidth,
		int						halfHeight
	);

	void RenderText
	(
		std::wstring			text,
		ownfos::graphic::Color	color,
		int						x,
		int						y,
		float					z
	);

	int RandomInt(int start, int end);
};

