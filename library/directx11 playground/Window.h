#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <memory>
#include <unordered_map>
#include <optional>
#include <memory>

#include "Graphic.h"
#include "Color.h"

namespace ownfos::graphic
{
	// -Forward declaration.
	class ResizeListener;
	class InputListener;

	// -Handles creation of window, window procedure, and management of resources required for rendering (e.g. swap chain, RTV, ...).
	// -Mouse/keyboard input is delegated to InputListener instances.
	//
	// -----Example usage-----
	// std::shared_ptr<Window> myWindow = std::make_shared<Window>(graphic, 600, 400, L"Window Title");
	class Window
	{
	private:
		std::shared_ptr<Graphic> graphic;

		HWND window;

		Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

		int width;
		int height;

		DWORD windowStyle;

		std::list<InputListener*> inputListeners;
		std::list<ResizeListener*> resizeListeners;

		static std::unordered_map<HWND, Window*> windowMap;

		void ResizeResources(int width, int height);

		static LRESULT WINAPI WindowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	public:
		Window(std::shared_ptr<Graphic> graphic, int width, int height, const char* windowName, bool resizable = false);
		~Window();

		void Resize(int width, int height);
		void Present();
		void Clear(Color clearColor, FLOAT depth = 1.0f, UINT8 stencil = 0);
		void SetAsRenderTarget();

		// -Returns width of window in pixel.
		int Width();

		// -Returns height of window in pixel.
		int Height();

		// -Returns width of single pixel in normalized coordinate.
		float PixelWidth();

		// -Returns height of single pixel in normalized coordinate.
		float PixelHeight();

		void RegisterInpuListener(InputListener* listener);

		void UnregisterInputListener(InputListener* listener);

		void RegisterResizeListener(ResizeListener* listener);

		void UnregisterResizeListener(ResizeListener* listener);

		// -Returns WPARAM when WM_QUIT message is sent.
		static std::optional<WPARAM> ProcessMessage();
	};
}