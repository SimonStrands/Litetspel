#pragma once
#include "GameObject.h"
#include "plHelper.h"

class SkyBox : public GameObject{
public:
	SkyBox(ModelObj* file, Graphics*& gfx, GameObject* player, std::string textureFile[]);
private:
	bool InitSkyBox(Graphics*& gfx, std::string textureFile[]);
	ID3D11ShaderResourceView* RSV;
	ID3D11Texture2D* cubeTexture;
};

