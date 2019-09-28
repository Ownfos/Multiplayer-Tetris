#pragma once

#include <memory>

#include "Graphic.h"
#include "Color.h"

namespace ownfos::graphic
{
	// -A texture for rendering texts.
	// -Make sure you don't render on this texture while it's set as pixel shader resource,
	//  because that will lead to unexpected behaviors (such as improper rendering)!
	//
	// -----Example usage-----
	// TextSurface textSurface(graphic, 600, 400);
	// auto format = graphic.CreateTextFormat(L"Arial", 12.0f);
	// textSurface.WriteText(L"Hello, World!", format, 0x000000ff, 0.0f, 0.0f, 300.0f);
	class TextSurface
	{
	private:
		std::shared_ptr<Graphic> graphic;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> d3d11Texture;
		Microsoft::WRL::ComPtr<IDXGIKeyedMutex> d3d11KeyedMutex;
		Microsoft::WRL::ComPtr<IDXGIKeyedMutex> d3d10KeyedMutex;
		Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;

	public:
		TextSurface(std::shared_ptr<Graphic> graphic, int width, int height);
		void Resize(int width, int height);
		// -Renders text on specified position, font, and color.
		// -Do not call this function when this texture is still set as pixel shader resource!
		//  You can unbind shader resource by calling Graphic::UnsetPixelShaderResource().
		// -Range of x : 0(left) ~ width in pixel
		// -Range of y : 0(top) ~ height in pixel
		// -Any text that goes beyond maxWidth will be drawn on next line automatically.
		// -You can create text format by calling Graphic::CreateTextFormat(), if needed.
		void WriteText(std::wstring text, Microsoft::WRL::ComPtr<IDWriteTextFormat> format, Color color, float x, float y, float maxWidth, DWRITE_TEXT_ALIGNMENT textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		void SetAsPixelShaderResource(int slot);
		void Clear();
	};
}