#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <string>
#include <vector>
#include <memory>

#include "Graphic.h"

using InputLayout = std::vector<std::pair<std::string, DXGI_FORMAT>>;

namespace ownfos::graphic
{
	// -Handles creation and usage of vertex shader.
	//
	// -----Example usage-----
	// VertexShader vs(graphic, L"VertexShader.cso", {{"POSITION", DXGI_FORMAT_R32G32B32_FLOAT}, {"TEXCOORD", DXGI_FORMAT_R32G32}});
	// std::shared_ptr<VertexShader> sharedVS = std::make_shared<VertexShader>(graphic, L"VertexShader.cso", InputLayout{{"POSITION", DXGI_FORMAT_R32G32B32_FLOAT}, {"TEXCOORD", DXGI_FORMAT_R32G32}});
	class VertexShader
	{
	private:
		std::shared_ptr<Graphic> graphic;

		Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	public:
		// -fileName : name(path) of the compiled shader object relative to program's exe file location.
		// -inputFormat : list of semantic name and dxgi format for shader's input parameter.
		VertexShader(std::shared_ptr<Graphic> graphic, std::wstring fileName, InputLayout inputFormat);
		void SetAsVertexShader();
	};
}

