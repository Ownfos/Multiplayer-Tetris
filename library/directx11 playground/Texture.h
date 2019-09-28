#pragma once

#include <string>
#include <memory>

#include "Graphic.h"
#include "Color.h"

namespace ownfos::graphic
{
	// -Handles creation of Texture2D for post-processing or loads image files to Texture2D for pixel shader input.
	// -You should call ownfos::graphic::Initialize() on Utility.h if you wish to load files.
	//
	// -----Example usage-----
	// Texture woodTexture(graphic, L"wood.jpg");
	// Texture postProcessingSurface(graphic, 600, 400);
	class Texture
	{
	private:
		std::shared_ptr<Graphic> graphic;

		int width;
		int height;
		DXGI_FORMAT format;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

	public:
		// load texture from file to use as shader resource
		Texture(std::shared_ptr<Graphic> graphic, std::wstring fileName);

		// create new empty buffer to use as render target & shader resource
		Texture(std::shared_ptr<Graphic> graphic, int width, int height, DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT);

		void Resize(int width, int height);
		void Clear(Color clearColor, FLOAT depth = 1.0f, UINT8 stencil = 0);
		void SetAsRenderTarget();
		void SetAsPixelShaderResource(int slot);
	};
}

