#include "CreateBuffer.h"
#include "GameObject.h"
#include "Graphics.h"

bool CreateVertexConstBuffer(Graphics*& gfx, ID3D11Buffer*& buff)
{
	D3D11_BUFFER_DESC CbDesc;
	CbDesc.ByteWidth = sizeof(Vcb);
	CbDesc.Usage = D3D11_USAGE_DYNAMIC;
	CbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	CbDesc.MiscFlags = 0;
	CbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	InitData.pSysMem = gfx->getVertexconstbuffer();

	HRESULT hr = gfx->getDevice()->CreateBuffer(&CbDesc, &InitData, &buff);
	if (FAILED(hr)) {
		printf("failed");
		return false;
	}

	return !FAILED(hr);
}

bool CreateConstBuffer(Graphics*& gfx, ID3D11Buffer*& buff, UINT size, CB *initdata) {
	D3D11_BUFFER_DESC CbDesc;
	CbDesc.ByteWidth = size;
	CbDesc.Usage = D3D11_USAGE_DYNAMIC;
	CbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	CbDesc.MiscFlags = 0;
	CbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;

	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	InitData.pSysMem = &initdata;

	HRESULT hr = gfx->getDevice()->CreateBuffer(&CbDesc, &InitData, &buff);
	if (FAILED(hr)) {
		printf("failed");
		return false;
	}

	return !FAILED(hr);
}

bool CreatePixelConstBuffer(Graphics*& gfx, ID3D11Buffer*& buff)
{

	D3D11_BUFFER_DESC CbDesc;
	CbDesc.ByteWidth = sizeof(Pcb);
	CbDesc.Usage = D3D11_USAGE_DYNAMIC;
	CbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	CbDesc.MiscFlags = 0;
	CbDesc.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = gfx->getPixelconstbuffer();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	HRESULT hr = gfx->getDevice()->CreateBuffer(&CbDesc, &InitData, &buff);

	return FAILED(hr);
}

bool CreateGeometryConstBuffer(Graphics*& gfx, ID3D11Buffer*& buff)
{
	D3D11_BUFFER_DESC CbDesc;
	CbDesc.ByteWidth = sizeof(Gcb);
	CbDesc.Usage = D3D11_USAGE_DYNAMIC;
	CbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	CbDesc.MiscFlags = 0;
	CbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = gfx->getPixelconstbuffer();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	HRESULT hr = gfx->getDevice()->CreateBuffer(&CbDesc, &InitData, &buff);

	return FAILED(hr);
}

