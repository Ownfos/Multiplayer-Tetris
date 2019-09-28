#include "TriangleMesh.h"

namespace ownfos::graphic
{
	template<typename T>
	TriangleMesh<T>::TriangleMesh(std::shared_ptr<Graphic> graphic, std::vector<T>&& vertexData, std::vector<unsigned int>&& indexData) : graphic(graphic), vertexData(vertexData), indexData(indexData)
	{
		vertexBuffer = graphic->CreateBuffer(D3D11_BIND_VERTEX_BUFFER, this->vertexData.size() * sizeof(T), &this->vertexData[0]);
		indexBuffer = graphic->CreateBuffer(D3D11_BIND_INDEX_BUFFER, this->indexData.size() * sizeof(T), &this->indexData[0]);
	}

	template<typename T>
	void TriangleMesh<T>::SetAsVertexBuffer()
	{
		graphic->SetVertexBuffer(vertexBuffer.GetAddressOf(), sizeof(T), 0);
		graphic->SetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	template<typename T>
	void TriangleMesh<T>::DrawAll()
	{
		graphic->DrawIndexed(indexData.size(), 0);
	}

	template<typename T>
	void TriangleMesh<T>::DrawPart(int indexCount, int indexOffset)
	{
		graphic->DrawIndexed(indexCount, indexOffset);
	}

	template<typename T>
	size_t TriangleMesh<T>::Size()
	{
		return indexData.size() / 3;
	}

	template<typename T>
	std::tuple<T, T, T> TriangleMesh<T>::operator[](int index)
	{
		return {vertexData[indexData[index * 3]], vertexData[indexData[index * 3 + 1]], vertexData[indexData[index * 3 + 2]]};
	}
}