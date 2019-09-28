#include "BlockRenderer.h"

using namespace ownfos::graphic;
using namespace DirectX;

BlockRenderer::BlockRenderer
(
	std::shared_ptr<Graphic>	graphic,
	std::shared_ptr<Window>		window
) :
	graphic(graphic),
	window(window)
{
	vertexShader = std::make_shared<VertexShader>
	(
		graphic,
		L"BlockVS.cso",
		std::vector<std::pair<std::string, DXGI_FORMAT>>
		{
			{ "POSITION", DXGI_FORMAT_R32G32_FLOAT }
		}
	);
	pixelShader		= std::make_shared<PixelShader>(graphic, L"BlockPS.cso");
	colorBuffer		= std::make_shared<ConstantBuffer<BlockColor>>(graphic);
	positionBuffer	= std::make_shared<ConstantBuffer<BlockPosition>>(graphic);
	vertexBuffer	= std::make_shared<TriangleMesh<BlockVertex>>
	(
		graphic,
		std::vector<BlockVertex>
		{
			{  1, 1 },
			{  1,-1 },
			{ -1,-1 },
			{ -1, 1 }
		},
		std::vector<unsigned int>
		{
			0, 1, 2,
			2, 3, 0
		}
	);
}

void BlockRenderer::RenderBlock
(
	ownfos::graphic::Color	color,
	int						centerX,
	int						centerY,
	float					centerZ,
	int						halfWidth,
	int						halfHeight
)
{
	window->SetAsRenderTarget();
	vertexShader->SetAsVertexShader();
	pixelShader->SetAsPixelShader();
	vertexBuffer->SetAsVertexBuffer();

	colorBuffer->data->color = color;
	colorBuffer->UpdateSubresource();
	colorBuffer->SetAsPixelShaderBuffer(0);

	positionBuffer->data->world = XMMatrixTranspose
	(
		XMMatrixScaling
		(
			window->PixelWidth()	* halfWidth,
			window->PixelHeight()	* halfHeight,
			1.0f
		) *
		XMMatrixTranslation
		(
			window->PixelWidth()	* centerX,
			window->PixelHeight()	* centerY,
			centerZ
		)
	);

	positionBuffer->UpdateSubresource();
	positionBuffer->SetAsVertexShaderBuffer(0);
	vertexBuffer->DrawAll();
}