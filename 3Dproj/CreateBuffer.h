#pragma once
#include <string>
#include <d3d11.h>
#include <vector>
#include "Graphics.h"
#include <string>

//tww
//#include "Mesh.h"

class MeshObj;
//class Graphics;
class GameObject;

//bool CreateVertexBuffer(Graphics*& gfx, MeshObj& obj, std::string fileName);
bool CreateVertexConstBuffer(Graphics *&gfx, ID3D11Buffer*& buff);
bool CreatePixelConstBuffer(Graphics *&gfx, ID3D11Buffer*& buff);
bool CreateGeometryConstBuffer(Graphics *&gfx, ID3D11Buffer*& buff);
bool CreateConstBuffer(Graphics *&gfx, ID3D11Buffer*& buff, UINT size, CB *initdata);
template<class T>
bool CreateVertexBuffer(ID3D11Device* device, std::vector<T> vertecies, ID3D11Buffer*& vertexBuffer, bool ind = false) {
	D3D11_BUFFER_DESC bDesc = {};
	bDesc.ByteWidth = sizeof(T) * (UINT)vertecies.size();
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	if (ind) {
		bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	}
	
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;
	bDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertecies.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&bDesc, &data, &vertexBuffer);
	if (FAILED(hr)) {
		printf("failed");
		return false;
	}
	if (hr != S_OK) {
		printf("its not ok");
	}
	return !FAILED(hr);
}