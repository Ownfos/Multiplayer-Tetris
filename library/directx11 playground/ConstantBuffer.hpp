#include "ConstantBuffer.h"

namespace ownfos::graphic
{
	template<typename T>
	template<typename ...Args>
	ConstantBuffer<T>::ConstantBuffer(std::shared_ptr<Graphic> graphic, Args ...args) : graphic(graphic)
	{
		// -Note that std::make_shared<> doesn't work on Data type with 16 byte alignment(e.g. XMMATRIX).
		data = std::shared_ptr<T>(new T(args...));
		constantBuffer = graphic->CreateBuffer(D3D11_BIND_CONSTANT_BUFFER, sizeof(T), data.get());
	}

	template<typename T>
	void ConstantBuffer<T>::UpdateSubresource()
	{
		graphic->UpdateSubresource(constantBuffer.Get(), data.get());
	}

	template<typename T>
	void ConstantBuffer<T>::SetAsVertexShaderBuffer(int slot)
	{
		graphic->SetVertexShaderConstantBuffer(constantBuffer.GetAddressOf(), slot);
	}

	template<typename T>
	void ConstantBuffer<T>::SetAsPixelShaderBuffer(int slot)
	{
		graphic->SetPixelShaderConstantBuffer(constantBuffer.GetAddressOf(), slot);
	}

}