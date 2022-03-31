#pragma once
#include "plHelper.h"
#include "Light.h"
#include <d3d11.h>
#include "plHelper.h"
#include <DirectXMath.h>
#include "CreateBuffer.h"
#include "Graphics.h"

class ShadowMap {
public:
	ShadowMap(SpotLight** light, int nrOfLights, Graphics* gfx, UINT width = 0, UINT height = 0);
	virtual ~ShadowMap();
	ID3D11DepthStencilView* Getdepthview(int i);
	ID3D11ShaderResourceView*& GetshadowResV();
	ID3D11ShaderResourceView*& fromDepthToSRV();
	void setUpdateShadow();
	void inUpdateShadow(int whatNrOfLight);
private:

	bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height);
	//object
	int nrOfLights;
	SpotLight** light;
	Graphics* gfx;

	ID3D11Texture2D* dsTexture;
	std::vector<ID3D11DepthStencilView*> dsViews;
	ID3D11VertexShader* vertexShadow;
	ID3D11PixelShader* pixelShadow;
	ID3D11Resource* shadowRes;
	ID3D11ShaderResourceView* shadowResV;
	D3D11_VIEWPORT ShadowViewPort;

};