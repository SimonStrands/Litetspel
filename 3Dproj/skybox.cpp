#include "skybox.h"


SkyBox::SkyBox(ModelObj* file, Graphics*& gfx, GameObject* player, std::string textureFile[]) :
	GameObject(file, gfx, player->getPos())
{
	if (!InitSkyBox(gfx, textureFile)) {
		std::cout << "didn't work" << std::endl;
	}
}

bool SkyBox::InitSkyBox(Graphics*& gfx, std::string textureFile[])
{
	HRESULT hr;
	const int nrOfRTV = 6;

	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = 1024;
	textureDesc.Height = 1024;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = nrOfRTV;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	//ladda texture
	for (int i = 0; i < 6; i++) {
		CreateTextureNR(textureFile[i], );
	}
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = (void*)textureData;
	data.SysMemPitch = textureWidth * 4;
	data.SysMemSlicePitch = textureWidth * textureHeight * 4;

	if (FAILED(gfx->getDevice()->CreateTexture2D(&textureDesc, NULL, &cubeTexture))) {
		printf("doesn't work tex2d");
		return false;
	}
	
	gfx->getDevice()->CreateShaderResourceView(cubeTexture, nullptr, &RSV);
	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVdesc;
	UAVdesc.Format = textureDesc.Format;
	UAVdesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	UAVdesc.Texture2DArray.MipSlice = 0;
	UAVdesc.Texture2DArray.ArraySize = 1;

	


	D3D11_SHADER_RESOURCE_VIEW_DESC RSVdesc{};
	RSVdesc.Format = textureDesc.Format;
	RSVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	RSVdesc.Texture2DArray.FirstArraySlice = 0;
	RSVdesc.Texture2DArray.MostDetailedMip = 0;
	RSVdesc.Texture2DArray.MipLevels = 1;
	RSVdesc.Texture2DArray.ArraySize = nrOfRTV;

	hr = gfx->getDevice()->CreateShaderResourceView(cubeTexture, &RSVdesc, &RSV);
	if (hr != S_OK)
	{
		printf("failed create RSV");
		return false;
	}
	return false;
}


