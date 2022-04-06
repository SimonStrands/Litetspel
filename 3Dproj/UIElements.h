#pragma once
#include "Vec.h"
#include "Graphics.h"

class UIElements {
public:
	UIElements(vec2 _pos, vec2 _size);
	virtual ~UIElements();
	virtual void draw(Graphics*& gfx) = 0;
private:
	vec2 position;
	vec2 size;
};
