#pragma once
#include "Vec.h"

struct point {
	float pos[3];
	point(vec3 punkt) {
		this->pos[0] = punkt.x;
		this->pos[1] = punkt.y;
		this->pos[2] = punkt.z;
	}
};