#pragma once

#include "..//library/directx11 playground/Graphic.h"
#include "..//library/directx11 playground/Window.h"
#include "..//library/directx11 playground/TextSurface.h"
#include "..//library/directx11 playground/VertexShader.h"
#include "..//library/directx11 playground/PixelShader.h"
#include "..//library/directx11 playground/ConstantBuffer.h"
#include "..//library/directx11 playground/ResizeListener.h"
#include "..//library/directx11 playground/Sampler.h"
#include "..//library/directx11 playground/TriangleMesh.h"

#include <DirectXMath.h>

struct TextVertex
{
	float x, y, z;
	float u, v;
	TextVertex(float x, float y, float z, float u, float v) : x(x), y(y), z(z), u(u), v(v) {}
};

struct TextPosition
{
	DirectX::XMMATRIX world;
	TextPosition() : world() {}
};

class TextRenderer : public ownfos::graphic::ResizeListener
{
private:
	std::shared_ptr<ownfos::graphic::Graphic>						graphic;
	std::shared_ptr<ownfos::graphic::Window>						window;
	Microsoft::WRL::ComPtr<IDWriteTextFormat>						textFormat;
	std::shared_ptr<ownfos::graphic::TextSurface>					textSurface;
	std::shared_ptr<ownfos::graphic::VertexShader>					vertexShader;
	std::shared_ptr<ownfos::graphic::PixelShader>					pixelShader;
	std::shared_ptr<ownfos::graphic::ConstantBuffer<TextPosition>>	positionBuffer;
	std::shared_ptr<ownfos::graphic::Sampler>						sampler;
	std::shared_ptr<ownfos::graphic::TriangleMesh<TextVertex>>		vertexBuffer;

public:
	TextRenderer
	(
		std::shared_ptr<ownfos::graphic::Graphic>	graphic,
		std::shared_ptr<ownfos::graphic::Window>	window,
		Microsoft::WRL::ComPtr<IDWriteTextFormat>	textFormat
	);

	void OnResize(int width, int height);

	void RenderText
	(
		std::wstring			text,
		ownfos::graphic::Color	color,
		int						x,
		int						y,
		float					z
	);
};