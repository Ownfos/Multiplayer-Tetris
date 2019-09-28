#pragma once

#include <memory>

#include "Graphic.h"

namespace ownfos::graphic
{
	// -Texture sampler for pixel shader.
	class Sampler
	{
	private:
		std::shared_ptr<Graphic> graphic;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

	public:
		Sampler(std::shared_ptr<Graphic> graphic, D3D11_TEXTURE_ADDRESS_MODE addressMode = D3D11_TEXTURE_ADDRESS_WRAP);
		void SetAsPixelShaderSampler(int slot);
	};
}

