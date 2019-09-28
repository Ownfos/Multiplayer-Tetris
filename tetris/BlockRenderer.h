#pragma once

#include "..//library/directx11 playground/Graphic.h"
#include "..//library/directx11 playground/Window.h"
#include "..//library/directx11 playground/VertexShader.h"
#include "..//library/directx11 playground/PixelShader.h"
#include "..//library/directx11 playground/ConstantBuffer.h"
#include "..//library/directx11 playground/TriangleMesh.h"

#include <DirectXMath.h>

struct BlockVertex
{
	float x, y;
	BlockVertex(float x, float y) : x(x), y(y) {}
};

struct BlockColor
{
	ownfos::graphic::Color color;
	BlockColor() : color(0xffffffff) {}
};

struct BlockPosition
{
	DirectX::XMMATRIX world;
	BlockPosition() : world() {}
};

class BlockRenderer
{
private:
	std::shared_ptr<ownfos::graphic::Graphic>						graphic;
	std::shared_ptr<ownfos::graphic::Window>						window;
	std::shared_ptr<ownfos::graphic::VertexShader>					vertexShader;
	std::shared_ptr<ownfos::graphic::PixelShader>					pixelShader;
	std::shared_ptr<ownfos::graphic::ConstantBuffer<BlockColor>>	colorBuffer;
	std::shared_ptr<ownfos::graphic::ConstantBuffer<BlockPosition>> positionBuffer;
	std::shared_ptr<ownfos::graphic::TriangleMesh<BlockVertex>>		vertexBuffer;

public:
	BlockRenderer
	(
		std::shared_ptr<ownfos::graphic::Graphic>	graphic,
		std::shared_ptr<ownfos::graphic::Window>	window
	);

	void RenderBlock
	(
		ownfos::graphic::Color	color,
		int						centerX,
		int						centerY,
		float					centerZ,
		int						halfWidth,
		int						halfHeight
	);
};
