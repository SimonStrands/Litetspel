#pragma once
#include "UIElements.h"
#include "ResourceManager.h"

class UISprite : public UIElements {
public:
	UISprite(ID3D11ShaderResourceView* _sprite, vec2 pos, vec2 size);
	//SpritePtr from ResoruceManager
	ID3D11ShaderResourceView* sprite;	

	void draw(Graphics*& gfx) override;
private:
};