#include "TextRenderer.h"

using namespace ownfos::graphic;
using namespace Microsoft::WRL;
using namespace DirectX;

TextRenderer::TextRenderer
(
	std::shared_ptr<Graphic>	graphic,
	std::shared_ptr<Window>		window,
	ComPtr<IDWriteTextFormat>	textFormat
) :
	ResizeListener(window),
	graphic(graphic),
	window(window),
	textFormat(textFormat)
{
	textSurface = std::make_shared<TextSurface>
		(
			graphic,
			window->Width(),
			window->Height()
			);
	vertexShader = std::make_shared<VertexShader>
		(
			graphic, L"TextVS.cso",
			InputLayout
			{
				{ "POSITION", DXGI_FORMAT_R32G32B32_FLOAT },
				{ "TEXCOORD", DXGI_FORMAT_R32G32_FLOAT }
			}
	);
	pixelShader = std::make_shared<PixelShader>(graphic, L"TextPS.cso");
	positionBuffer = std::make_shared<ConstantBuffer<TextPosition>>(graphic);
	sampler = std::make_shared<Sampler>(graphic);
	vertexBuffer = std::make_shared<TriangleMesh<TextVertex>>
		(
			graphic,
			std::vector<TextVertex>{
				{  1, 1, 0, 1, 0 },
				{ 1,-1, 0, 1, 1 },
				{ -1,-1, 0, 0, 1 },
				{ -1, 1, 0, 0, 0 }
	},
			std::vector<unsigned int>{
					0, 1, 2,
						2, 3, 0
				}
				);
}

void TextRenderer::OnResize(int width, int height)
{
	textSurface->Resize(width, height);
}

void TextRenderer::RenderText
(
	std::wstring	text,
	Color			color,
	int				x,
	int				y,
	float			z
)
{
	window->SetAsRenderTarget();
	vertexShader->SetAsVertexShader();
	pixelShader->SetAsPixelShader();
	vertexBuffer->SetAsVertexBuffer();
	sampler->SetAsPixelShaderSampler(0);

	graphic->UnsetPixelShaderResource(0);
	textSurface->Clear();
	textSurface->WriteText
	(
		text,
		textFormat,
		color,
		window->Width() / 2.0f + x,
		window->Height() / 2.0f - y,
		10000.0f
	);

	textSurface->SetAsPixelShaderResource(0);

	positionBuffer->data->world = XMMatrixTranspose(XMMatrixTranslation(0.0f, 0.0f, z));
	positionBuffer->UpdateSubresource();
	positionBuffer->SetAsVertexShaderBuffer(0);

	vertexBuffer->DrawAll();
}