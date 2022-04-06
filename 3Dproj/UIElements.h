#pragma once
#include "Vec.h"

class UIElements {
public:
	UIElements(vec2 _pos, vec2 _size);
	virtual ~UIElements();
	virtual void draw() = 0;
private:
	vec2 position;
	vec2 size;
};
