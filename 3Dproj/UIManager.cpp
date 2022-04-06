#include "UIManager.h"

UIManager::UIManager(ResourceManager*& rm, Graphics*& gfx)
{
	this->gfx = gfx;
	this->rm = rm;
	init(gfx);
}

UIManager::~UIManager()
{
	for (int i = 0; i < elements.size(); i++) {
		delete elements[i];
	}
}

void UIManager::createUIString(std::string str, vec2 pos, vec2 size)
{
	strings.push_back(UIString());
}

void UIManager::createUISprite(std::string rmsprite, vec2 pos, vec2 size)
{
	elements.push_back(new UISprite(rm->getSprite(rmsprite, gfx), pos, size));
}

void UIManager::draw()
{
	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//gfx->get_IMctx()->IASetVertexBuffers(0,1,,strid, offset);
	//gfx->get_IMctx()->IASetInputLayout();
	for (int i = 0; i < elements.size(); i++) {
		elements[i]->draw(gfx);
	}
	for (int i = 0; i < strings.size(); i++) {
		strings[i].draw(gfx);
	}
}

void UIManager::init(Graphics*& gfx)
{
	std::string vShaderByteCode[1];
	//load shader
	if (!loadVShader("UIVertexShader.cso", gfx->getDevice(), vShader, vShaderByteCode[0]) &&
		loadPShader("UIPixelShader.cso", gfx->getDevice(), pShader)) 
	{
		std::cout << "error cannot load UI shader" << std::endl;
	}


	//create input layout
	D3D11_INPUT_ELEMENT_DESC inputDesc[2] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	CreateInputLayoutOwn(gfx->getDevice(), inputLayout, vShaderByteCode[0], inputDesc, 2);
}
