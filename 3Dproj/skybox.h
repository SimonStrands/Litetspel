#pragma once
#include "GameObject.h"
#include "plHelper.h"

class SkyBox : public GameObject{
public:
	SkyBox(ModelObj* file, Graphics*& gfx, vec3 pos, std::string textureFile[]);
	virtual ~SkyBox();
	void draw(Graphics*& gfx);
	void update(vec3 position);
private:
	bool InitSkyBox(Graphics*& gfx, std::string textureFile[]);
	ID3D11ShaderResourceView* RSV;
	ID3D11Texture2D* cubeTexture;
};

