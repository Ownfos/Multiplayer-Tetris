#pragma once

#include <memory>

#include "Graphic.h"

namespace ownfos::graphic
{
	// -Handles creation and usage of constant buffer for shaders(both vertex or pixel).
	// -Pass the structure (or class) you want to use on buffer as template argument.
	// -Remember that no data changes on CPU (i.e. on your code) will be reflected on GPU unless you call UpdateSubresource().
	//
	// -----Example usage-----
	// ConstantBuffer<YourDataStruct> yourBuffer(graphic, arg1, arg2, ...);
	// yourBuffer.data->memberVariable = 12345;
	// yourBuffer.UpdateSubresource();
	template<typename T>
	class ConstantBuffer
	{
	private:
		std::shared_ptr<Graphic> graphic;
		Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

	public:
		std::shared_ptr<T> data;

		template<typename... Args>
		ConstantBuffer(std::shared_ptr<Graphic> graphic, Args... args);
		void UpdateSubresource();
		void SetAsVertexShaderBuffer(int slot);
		void SetAsPixelShaderBuffer(int slot);
	};
}

#include "ConstantBuffer.hpp"
