#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include <d3d10_1.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wrl.h>
#include <string>
#include <exception>

#include "Color.h"

namespace ownfos::graphic
{
	// -Collection of factories, devices, and default pipeline states.
	// -Think of it as a wrapper class that makes using DirectX more simple and neat.
	//
	// -----Example usage-----
	// auto graphic = std::make_shared<Graphic>(D3D11_CREATE_DEVICE_DEBUG, D3D11_CREATE_DEVICE_DEBUG);
	class Graphic
	{
	private:
		Microsoft::WRL::ComPtr<IDXGIFactory1> factory;
		Microsoft::WRL::ComPtr<ID3D11Device> d3d11Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3d11DeviceContext;
		Microsoft::WRL::ComPtr<ID3D10Device1> d3d10Device;
		Microsoft::WRL::ComPtr<ID2D1Factory> d2dFactory;
		Microsoft::WRL::ComPtr<IDWriteFactory> dwriteFactory;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

		struct RectVertex
		{
			float x, y, z;
			float u, v;
		};

	public:
		// -Pass D3D11_CREATE_DEVICE_DEBUG, D3D10_CREATE_DEVICE_DEBUG to get debug messages
		Graphic(UINT d3d11DeviceFlags = NULL, UINT d3d10DeviceFlags = NULL);

		Microsoft::WRL::ComPtr<ID3D11Buffer> CreateBuffer(UINT bindFlags, int bufferSize, void* initialData);
		Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateTexture2D(std::wstring fileName);
		Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateTexture2D(UINT bindFlags, DXGI_FORMAT format, int width, int height, int sampleCount = 1, UINT miscFlags = NULL);
		Microsoft::WRL::ComPtr<IDXGISwapChain> CreateSwapChain(HWND outputWindow, int width, int height, int bufferCount);
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> CreateRenderTargetView(IDXGISwapChain* swapChain);
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> CreateRenderTargetView(ID3D11Texture2D* texture);
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateShaderResourceView(IDXGISwapChain* swapChain);
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateShaderResourceView(ID3D11Texture2D* texture);
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> CreateDepthStencilView(ID3D11Texture2D* texture);
		Microsoft::WRL::ComPtr<ID3DBlob> CreateShaderBlob(const wchar_t* fullPath);
		Microsoft::WRL::ComPtr<ID3D11VertexShader> CreateVertexShader(ID3DBlob* shaderBlob);
		Microsoft::WRL::ComPtr<ID3D11PixelShader> CreatePixelShader(ID3DBlob* shaderBlob);
		Microsoft::WRL::ComPtr<ID3D11InputLayout> CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* inputElements, int elementCount, ID3DBlob* vertexShaderBlob);
		Microsoft::WRL::ComPtr<ID3D11SamplerState> CreateSamplerState(D3D11_TEXTURE_ADDRESS_MODE addressMode = D3D11_TEXTURE_ADDRESS_WRAP);
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> CreateRasterizerState(D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode);
		Microsoft::WRL::ComPtr<ID2D1RenderTarget> CreateD2DRenderTarget(IDXGISurface1* d3d10SharedSurface);
		Microsoft::WRL::ComPtr<IDXGISurface1> OpenSharedResource(HANDLE sharedHandle);
		Microsoft::WRL::ComPtr<IDWriteTextFormat> CreateTextFormat(std::wstring fontFamily, float size, DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL);

		void ClearRenderTargetView(ID3D11RenderTargetView* renderTargetView, Color clearColor);
		void ClearDepthStencilView(ID3D11DepthStencilView* depthStencilView, FLOAT depth, UINT8 stencil);
		void SetRenderTarget(ID3D11RenderTargetView** renderTargetView, ID3D11DepthStencilView* depthStencilView);
		void SetViewPort(D3D11_VIEWPORT viewPort);
		void SetInputLayout(ID3D11InputLayout* inputLayout);
		void SetVertexShader(ID3D11VertexShader* vertexShader);
		void SetVertexShaderConstantBuffer(ID3D11Buffer** constantBuffer, int slot);
		void SetPixelShader(ID3D11PixelShader* pixelShader);
		void SetPixelShaderConstantBuffer(ID3D11Buffer** constantBuffer, int slot);
		void SetPixelShaderResource(ID3D11ShaderResourceView** shaderResourceView, int slot);
		void UnsetPixelShaderResource(int slot);
		void SetPixelShaderSampler(ID3D11SamplerState** samplerState, int slot);
		void SetVertexBuffer(ID3D11Buffer** vertexBuffer, UINT stride, UINT offset);
		void SetIndexBuffer(ID3D11Buffer* indexBuffer, DXGI_FORMAT format, UINT offset);
		void SetRasterizerState(ID3D11RasterizerState* rasterizerState);
		void UpdateSubresource(ID3D11Resource* resource, void* newData);
		void DrawIndexed(int count, int offset);
	};

}