#include "DynamicCubeEnviroment.h"
static const int whdCube = 320;
DynamicCube::DynamicCube(ModelObj* model, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale) :
	GameObject(model, gfx, pos, rot, scale),
	dCubeDeff(gfx, whdCube, whdCube)
{
	SetViewport(this->DviewPort, whdCube, whdCube);
	//set PS VS GS DS
	for (int i = 0; i < this->model->getMehses().size(); i++) {
		this->model->getMehses()[i].SetShaders(gfx->getVS()[0], gfx->getPS()[3]);
	}
	loadCShader("DeffrendCSwithUAVArray.cso", gfx->getDevice(), CSShader);
	CreateDepthStencil(gfx->getDevice(), whdCube, whdCube, dsTexture, dsview);
	initCubeMapping(gfx);
}

DynamicCube::~DynamicCube()
{
	if (CubeResV != nullptr) {
		CubeResV->Release();
	}
	if (CubeTex != nullptr) {
		CubeTex->Release();
	}
	
	if (CSShader != nullptr) {
		CSShader->Release();
	}
	if (dsview != nullptr) {
		dsview->Release();
	}
	if (dsTexture != nullptr) {
		dsTexture->Release();
	}
	for (int i = 0; i < 6; i++) {
		UAVs[i]->Release();
		RTV[i]->Release();
	}
	delete[] UAVs;
	delete[] RTV;
}

void DynamicCube::draw(ID3D11DeviceContext*& immediateContext)
{	
	immediateContext->VSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT offset = 0;
	static UINT strid = sizeof(vertex);
	immediateContext->PSSetShaderResources(0, 1, &this->CubeResV);

	for (int i = 0; i < this->model->getMehses().size(); i++) {
		this->model->getMehses()[i].draw2(immediateContext);
	}
}

ID3D11UnorderedAccessView** DynamicCube::getUAVs()
{
	return this->UAVs;
}

ID3D11ComputeShader *DynamicCube::getCSShader()
{
	return this->CSShader;
}

void DynamicCube::ClearRenderTarget(Graphics*& gfx) {
	FLOAT color[4] = { 0.1f,0.1f,0.1f,1.f };
	for (int i = 0; i < 6; i++) {
		gfx->get_IMctx()->ClearRenderTargetView(RTV[i], color);
	}
}

void DynamicCube::setRenderTarget(Graphics*& gfx, int i)
{
	dCubeDeff.BindFirstPass(dsview);
	gfx->get_IMctx()->ClearDepthStencilView(dsview, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	gfx->get_IMctx()->OMSetRenderTargets(1, &RTV[i], dsview);
}

void DynamicCube::update(vec3 camPos, Graphics*& gfx)
{

}

void DynamicCube::setViewPort(Graphics*& gfx)
{
	gfx->get_IMctx()->RSSetViewports(1, &this->DviewPort);
}

void DynamicCube::firstPass()
{
	this->dCubeDeff.BindFirstPass(this->dsview);
}

void DynamicCube::secondPass(ID3D11ShaderResourceView*& ShadowMapping,
	ID3D11UnorderedAccessView* UAV,
	int dx, int dy,
	ID3D11ComputeShader* CSShader)
{
	this->dCubeDeff.BindSecondPassFunc(ShadowMapping, UAV, dx, dy, CSShader);
}

bool DynamicCube::initCubeMapping(Graphics*& gfx)
{
	HRESULT hr;
	const int nrOfRTV = 6;
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = whdCube;
	textureDesc.Height = whdCube;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = nrOfRTV;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	if (FAILED(gfx->getDevice()->CreateTexture2D(&textureDesc, NULL, &CubeTex))) {
		printf("doesn't work tex2d");
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC RSVdesc{};
	RSVdesc.Format = textureDesc.Format;
	RSVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	RSVdesc.Texture2DArray.FirstArraySlice = 0;
	RSVdesc.Texture2DArray.MostDetailedMip = 0;
	RSVdesc.Texture2DArray.MipLevels = 1;
	RSVdesc.Texture2DArray.ArraySize = nrOfRTV;

	hr = gfx->getDevice()->CreateShaderResourceView(CubeTex, &RSVdesc, &CubeResV);
	if (hr != S_OK)
	{
		printf("failed create RSV");
		return false;
	}

	UAVs = new ID3D11UnorderedAccessView * [nrOfRTV];
	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVdesc;
	UAVdesc.Format = textureDesc.Format;
	UAVdesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	UAVdesc.Texture2DArray.MipSlice = 0;
	UAVdesc.Texture2DArray.ArraySize = 1;
	
	for (int i = 0; i < nrOfRTV; i++) {
		UAVdesc.Texture2DArray.FirstArraySlice = D3D11CalcSubresource(0, i, 1);
		hr = gfx->getDevice()->CreateUnorderedAccessView(CubeTex, &UAVdesc, &UAVs[i]);
		if (hr != S_OK) {
			printf("doesn't work");
			return false;
		}
	}

	RTV = new ID3D11RenderTargetView * [nrOfRTV];
	D3D11_RENDER_TARGET_VIEW_DESC RTVdesc{};
	RTVdesc.Format = textureDesc.Format;
	RTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	RTVdesc.Texture2DArray.ArraySize = 1;
	RTVdesc.Texture2DArray.MipSlice = 0;

	for (int i = 0; i < nrOfRTV; i++) {
		RTVdesc.Texture2DArray.FirstArraySlice = D3D11CalcSubresource(0, i, 1);
		hr = gfx->getDevice()->CreateRenderTargetView(CubeTex, &RTVdesc, &RTV[i]);
		if (hr != S_OK)
		{
			printf("failed create RTV");
			return false;
		}
	}
	

	return true;
}
