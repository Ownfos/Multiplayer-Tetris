#pragma once

#include <vector>

#include "Graphic.h"

namespace ownfos::graphic
{
	// -----Example usage-----
	// TriangleMesh<RectVertex> rectangle(graphic,
	// {
	//	{ 0,0,0 },
	//	{ ... }
	// },
	// {
	//  0,1,2,
	//  ...
	// });
	//
	// auto sharedRect = std::make_shared<TriangleMesh<VertexType>>(graphic,
	// std::vector<VertexType>{
	//  {...}
	//  },
	// std::vector<unsigned int>{
	//  ...
	// }};
	template<typename T>
	class TriangleMesh
	{
	private:
		std::shared_ptr<Graphic> graphic;
		std::vector<T> vertexData;
		std::vector<unsigned int> indexData;
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	public:
		TriangleMesh(std::shared_ptr<Graphic> graphic, std::vector<T>&& vertexData, std::vector<unsigned int>&& indexData);
		void SetAsVertexBuffer();
		void DrawAll();
		void DrawPart(int indexCount, int indexOffset);
		// -Returns number of triangles
		size_t Size();
		// -Returns triangle vertices at specified index
		std::tuple<T, T, T> operator[](int index);
	};
}

#include "TriangleMesh.hpp"