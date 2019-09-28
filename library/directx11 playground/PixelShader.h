#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <string>
#include <memory>

#include "Graphic.h"

namespace ownfos::graphic
{
	// -Handles creation and usage of vertex shader.
	//
	// -----Example usage-----
	// PixelShader vs(graphic, L"PixelShader.cso");
	class PixelShader
	{
	private:
		std::shared_ptr<Graphic> graphic;

		Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;

	public:
		// -fileName : name(path) of the compiled shader object relative to program's exe file location.
		PixelShader(std::shared_ptr<Graphic> graphic, std::wstring fileName);
		void SetAsPixelShader();
	};
}

