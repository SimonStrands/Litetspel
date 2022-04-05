#include "ShadowMap.h"
#include "rotation.h"
#include "D311Helper.h"


ShadowMap::ShadowMap(SpotLight** light, int nrOfLights, Graphics* gfx, UINT width, UINT height)
{
	this->gfx = gfx;
	this->light = light;
	this->nrOfLights = nrOfLights;
	std::string a;
	loadVShader("VertexShadow.cso", gfx->getDevice(), vertexShadow, a);
	loadPShader("PixelShadow.cso", gfx->getDevice(), pixelShadow);
	if (width == 0 && height == 0) {
		width = (UINT)gfx->getWH().x;
		height = (UINT)gfx->getWH().y;
	}
	if (!CreateDepthStencil(gfx->getDevice(), width, height)) {
		printf("something didnt go right");
	}
	SetViewport(ShadowViewPort, width, height);
}

ShadowMap::~ShadowMap()
{
	if (dsTexture != nullptr) {
		dsTexture->Release();
	}
	for (int i = 0; i < dsViews.size(); i++) {
		if (dsViews[i] != nullptr) {
			dsViews[i]->Release();
		}
	}
	if (vertexShadow != nullptr) {
		vertexShadow->Release();
	}
	if (pixelShadow != nullptr) {
		pixelShadow->Release();
	}
	if (shadowRes != nullptr) {
		shadowRes->Release();
	}
	if (shadowResV != nullptr) {
		shadowResV->Release();
	}
}

ID3D11DepthStencilView* ShadowMap::Getdepthview(int i)
{
	return dsViews[i];
}

ID3D11ShaderResourceView*& ShadowMap::GetshadowResV()
{
	return this->shadowResV;
}

ID3D11ShaderResourceView*& ShadowMap::fromDepthToSRV()
{	
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2DArray.ArraySize = nrOfLights;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.MipLevels = 1;
	
	for (int i = 0; i < nrOfLights; i++) {
		dsViews[i]->GetResource(&shadowRes);
		HRESULT hr = gfx->getDevice()->CreateShaderResourceView(
			shadowRes, &srvDesc, &shadowResV
		);
		if (hr != S_OK) {
			printf("can create shadowResourceView");
		}
		
	}
	shadowRes->Release();
	return shadowResV;
}

void ShadowMap::setUpdateShadow()
{

	//set view port here
	gfx->get_IMctx()->RSSetViewports(1, &this->ShadowViewPort);
	for (int i = 0; i < nrOfLights; i++) {
		gfx->get_IMctx()->ClearDepthStencilView(this->dsViews[i], D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	}
	gfx->get_IMctx()->VSSetShader(vertexShadow, nullptr, 0);
	gfx->get_IMctx()->PSSetShader(nullptr, nullptr, 0);
	ID3D11ShaderResourceView* const pSRV[1] = { NULL };
	gfx->get_IMctx()->PSSetShaderResources(4, 1, pSRV);
}

void ShadowMap::inUpdateShadow(int whatNrOfLight)
{
	ID3D11RenderTargetView* pNullRTV = NULL;
	gfx->get_IMctx()->OMSetRenderTargets(1, &pNullRTV, this->Getdepthview(whatNrOfLight));

	gfx->getVertexconstbuffer()->projection.element = this->light[whatNrOfLight]->getLightProj();
	
	gfx->getVertexconstbuffer()->view.element = this->light[whatNrOfLight]->getLightView();
}

bool ShadowMap::CreateDepthStencil(ID3D11Device* device, UINT width, UINT height)
{
	//CREATE TEXTURE 2D ARRAY
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = nrOfLights;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	

	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &dsTexture)))
	{
		printf("failed create 2d texture");
		return false;
	}

	//DEPTH STENCIL ARRAY
	D3D11_DEPTH_STENCIL_VIEW_DESC descView = {};
	descView.Format = DXGI_FORMAT_D32_FLOAT;
	descView.Flags = 0;
	descView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	descView.Texture2DArray.ArraySize = 1;
	descView.Texture2DArray.MipSlice = 0;

	for (int i = 0; i < nrOfLights; i++) {
		descView.Texture2DArray.FirstArraySlice = D3D11CalcSubresource(0, i, 1);
		ID3D11DepthStencilView* dsView;
		if (FAILED(device->CreateDepthStencilView(dsTexture, &descView, &dsView))) {
			printf("failed to create dSV");
			return false;
		}
		dsViews.push_back(dsView);
	}

	//DEPTH STENCIL TO SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.ArraySize = nrOfLights;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.MipLevels = 1;

	HRESULT hr = gfx->getDevice()->CreateShaderResourceView(
		dsTexture, &srvDesc, &shadowResV
	);
	if (hr != S_OK) {
		printf("can create shadowResourceView");
	}

	
	return true;
}

