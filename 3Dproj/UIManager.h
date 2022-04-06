#pragma once
#include "ResourceManager.h"
#include "Graphics.h"
#include <vector>
#include "UIString.h"
#include "UISprite.h"
#include <string>
#include "plHelper.h"

class UIManager {
public:
	UIManager(ResourceManager*& rm, Graphics*& gfx);
	virtual ~UIManager();
	void createUIString(std::string str, vec2 pos, vec2 size);
	void createUISprite(std::string rmsprite, vec2 pos, vec2 size);
	void draw();
private:
	std::vector<UIElements*> elements;
	std::vector<UIString> strings;
	ResourceManager* rm;
	Graphics* gfx;
	void init(Graphics*& gfx);
private://d3dshit
	ID3D11InputLayout* inputLayout;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
};