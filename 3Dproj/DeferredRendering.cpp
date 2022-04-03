#include "DeferredRendering.h"

DeferredRendering::DeferredRendering(Graphics*& gfx, int w, int h)
{
	this->gfx = gfx;
	nrOfRTV = 5;
	if (w == 0 && h == 0) {
		if (!InitDeferred((UINT)gfx->getWH().x, (UINT)gfx->getWH().y)) {
			std::cout << "stop" << std::endl;
		}
	}
	else {
		if (!InitDeferred(w, h)) {
			std::cout << "stop" << std::endl;
		}
	}
	//set compute shader and a extra test for pixel shader
	std::string a;
	
	//load cs
	loadCShader("DeferredLightningCS.cso", gfx->getDevice(), DeferredComputeS);
	loadPShader("TestFragmentshader.cso", gfx->getDevice(), DeferredPixelS);
}

DeferredRendering::~DeferredRendering()
{
	for (int i = 0; i < nrOfRTV; i++) {
		DeferredResV[i]->Release();
		DeferredTex[i]->Release();
		DeferredRTV[i]->Release();
	}
	dsView->Release();
	DeferredPixelS->Release();
	DeferredComputeS->Release();
	UAV->Release();
}

bool DeferredRendering::InitDeferred(int w, int h)
{	
	//change so depth is 1 float???
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = w;
	textureDesc.Height = h;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	for (int i = 0; i < nrOfRTV; i++) {
		if (FAILED(gfx->getDevice()->CreateTexture2D(&textureDesc, NULL, &DeferredTex[i]))) {
			printf("doesn't work tex2d");
			return false;
		}
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	for (int i = 0; i < nrOfRTV; i++) {
		if (FAILED(gfx->getDevice()->CreateRenderTargetView(DeferredTex[i], &renderTargetViewDesc, &DeferredRTV[i]))) {
			printf("doesn't work RTV");
			return false;
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	for (int i = 0; i < nrOfRTV; i++) {
		if (FAILED(gfx->getDevice()->CreateShaderResourceView(DeferredTex[i], &shaderResourceViewDesc, &DeferredResV[i])))
		{
			printf("failed create RSV");
			return false;
		}
	}
	ID3D11Texture2D* depthStencilTex = nullptr;
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc{};
	depthStencilBufferDesc.Width = (UINT)gfx->getWH().x;
	depthStencilBufferDesc.Height = (UINT)gfx->getWH().y;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if (FAILED(gfx->getDevice()->CreateTexture2D(&depthStencilBufferDesc, NULL, &depthStencilTex))) 
	{
		printf("failed create tex2d depth");
		return false;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc{};
	DSVDesc.Format = depthStencilBufferDesc.Format;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	if (FAILED(gfx->getDevice()->CreateDepthStencilView(depthStencilTex, &DSVDesc, &this->dsView))) {
		printf("DEPTH STENCIL VIEW");
		return false;
	}

	depthStencilTex->Release();	
	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(gfx->getSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		printf("failed to swapchain");
		return false;
	}

	HRESULT hr = gfx->getDevice()->CreateUnorderedAccessView(backBuffer, nullptr, &UAV);
	backBuffer->Release();
	return !FAILED(hr);
}

void DeferredRendering::BindFirstPass(ID3D11DepthStencilView* depth)
{
	if (depth == nullptr) {
		gfx->get_IMctx()->OMSetRenderTargets(5, DeferredRTV, gfx->getDepthStencil());
		gfx->get_IMctx()->ClearDepthStencilView(gfx->getDepthStencil(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	else {
		gfx->get_IMctx()->OMSetRenderTargets(5, DeferredRTV, depth);
		gfx->get_IMctx()->ClearDepthStencilView(depth, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	FLOAT color[4] = { 0.1f,0.1f,0.1f,1.f };
	gfx->get_IMctx()->ClearRenderTargetView(DeferredRTV[0], color);
	gfx->get_IMctx()->ClearRenderTargetView(DeferredRTV[1], color);
	gfx->get_IMctx()->ClearRenderTargetView(DeferredRTV[2], color);
	gfx->get_IMctx()->ClearRenderTargetView(DeferredRTV[3], color);
	gfx->get_IMctx()->ClearRenderTargetView(DeferredRTV[4], color);
}

void DeferredRendering::BindSecondPass(ID3D11ShaderResourceView*& ShadowMapping)
{
	FLOAT color[4] = { 0.1f,0.1f,0.1f,1.f };
	ID3D11UnorderedAccessView* nullUAV = nullptr;
	ID3D11RenderTargetView* nullRTV[5] = { nullptr };
	//set till null?
	gfx->get_IMctx()->OMSetRenderTargets(5, nullRTV, nullptr);

	//is this for compute shading
	gfx->get_IMctx()->CSSetShader(DeferredComputeS, nullptr, 0);

	gfx->get_IMctx()->CSSetShaderResources(0, 5, DeferredResV);
	gfx->get_IMctx()->CSSetShaderResources(5, 1, &ShadowMapping);//add ShadowMapping
	
	gfx->get_IMctx()->CSSetUnorderedAccessViews(0, 1, &this->UAV, nullptr);
	//köra computeShader
	//gfx->get_IMctx()->Dispatch(60, 135, 1);//1080p
	gfx->get_IMctx()->Dispatch(40, 45, 1);//720p
	ID3D11ShaderResourceView* nullSRV[6] = { nullptr };
	gfx->get_IMctx()->CSSetShaderResources(0, _countof(nullSRV), nullSRV);
	//nulla unorderedaccesview
	gfx->get_IMctx()->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
}

//using these for dynamic cube mapping
void DeferredRendering::BindSecondPassFunc(
	ID3D11ShaderResourceView*& ShadowMapping,
	ID3D11UnorderedAccessView* UAV,
	int dx, int dy,
	ID3D11ComputeShader* CSShader
)
{
	
	FLOAT color[4] = { 0.1f,0.1f,0.1f,1.f };
	ID3D11UnorderedAccessView* nullUAV = nullptr;
	ID3D11RenderTargetView* nullRTV[5] = { nullptr };
	//set till null?
	gfx->get_IMctx()->OMSetRenderTargets(5, nullRTV, nullptr);

	//is this for compute shading
	if (CSShader == nullptr) {
		gfx->get_IMctx()->CSSetShader(DeferredComputeS, nullptr, 0);
	}
	else {
		gfx->get_IMctx()->CSSetShader(CSShader, nullptr, 0);
	}
	

	gfx->get_IMctx()->CSSetShaderResources(0, 5, DeferredResV);
	gfx->get_IMctx()->CSSetShaderResources(5, 1, &ShadowMapping);//add ShadowMapping

	UINT s = 6;
	gfx->get_IMctx()->CSSetUnorderedAccessViews(0, 1, &UAV, 0);
	//köra computeShader
	gfx->get_IMctx()->Dispatch(dx, dy, 1);
	ID3D11ShaderResourceView* nullSRV[6] = { nullptr };
	gfx->get_IMctx()->CSSetShaderResources(0, _countof(nullSRV), nullSRV);
	//nulla unorderedaccesview
	gfx->get_IMctx()->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
}

ID3D11PixelShader* DeferredRendering::getPS()
{
	return this->DeferredPixelS;
}
