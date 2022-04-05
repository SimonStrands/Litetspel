#include "skybox.h"

SkyBox::SkyBox(ModelObj* file, Graphics*& gfx, vec3 pos, std::string textureFile[]) :
	GameObject(file, gfx, pos, vec3(0,0,0), vec3(10,10,10))
{
	if (!InitSkyBox(gfx, textureFile)) {
		std::cout << "didn't work" << std::endl;
	}
}

SkyBox::~SkyBox()
{
	RSV->Release();
	cubeTexture->Release();
}

void SkyBox::draw(Graphics*& gfx)
{
	//set pixelshader and vertexshader
	gfx->get_IMctx()->VSSetShader(gfx->getVS()[2], nullptr ,0);
	gfx->get_IMctx()->PSSetShader(gfx->getPS()[3], nullptr ,0);

	gfx->get_IMctx()->VSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	//draw
	for (int i = 0; i < model->getMehses().size(); i++) {
		model->getMehses()[i].draw(gfx->get_IMctx());
	}
}

void SkyBox::update(vec3 position)
{
	GameObject::setPos(position);
}

bool SkyBox::InitSkyBox(Graphics*& gfx, std::string textureFile[])
{
	if(!CreateTextureCube(textureFile, gfx->getDevice(), cubeTexture, RSV, 1024)) {
		std::cout << "stop" << std::endl;
	}
	
	for (int i = 0; i < model->getMehses().size(); i++) {
		model->getMehses()[i].getMatrial()->texSRVPS[0] = RSV;
	}
	
	return true;
}


