#pragma once
#include "Vec.h"

class UIElements {
public:
	UIElements();
	virtual ~UIElements();
	virtual void draw() = 0;
private:
	vec2 position;
	float width;
	float height;
};
