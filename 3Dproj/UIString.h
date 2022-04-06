#pragma once
#include <vector>
#include "UISymbols.h"
#include "vec.h"
class UIString {
public:
	std::vector<UISymbols> symbols;
	vec2 position;

	void draw(Graphics*& gfx);
private:
};