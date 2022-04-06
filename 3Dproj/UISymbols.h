#pragma once
#include "UIElements.h"
#include "vec.h"
#include <d3d11.h>

class UISymbols : public UIElements {
public: 
	UISymbols();
	void symbol_to_uv(char symbol);

	void draw(Graphics*& gfx) override;
private:
	vec2 UV_coords;
	ID3D11ShaderResourceView* spriteSheet;
};