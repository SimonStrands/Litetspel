#pragma once
#include <array>
#include "Vec.h"

//git
struct vertex {
	float pos[3];
	float uv[2];
	float norm[3];
	float tang[3];
	float bitang[3];
	vertex() {
		//yes should be like this
	}
	vertex(const std::array<float, 3>& position, const std::array<float, 2>& uvC, const std::array<float, 4>& normal) {
		for (int i = 0; i < 3; i++) {
			this->pos[i] = position[i];
			this->norm[i] = normal[i];
			tang[i] = 0;
			bitang[i] = 0;
		}
		uv[0] = uvC[0];
		uv[1] = uvC[1];
	}
	void fixtang(vec3 tang, vec3 bitang) {
		this->tang[0] = tang.x;
		this->tang[1] = tang.y;
		this->tang[2] = tang.z;
		this->bitang[0] = bitang.x;
		this->bitang[1] = bitang.y;
		this->bitang[2] = bitang.z;
	}
};

