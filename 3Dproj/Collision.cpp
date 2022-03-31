#include "Collision.h"
//for quad tree
bool collision2d(DirectX::XMFLOAT4 point[], vec2 pos, float size) {
	//if big point or small point is in the triangle
	float objl = point[0].x;
	float obju = point[1].z;
	float objw = point[1].x - point[0].x;
	float objh = point[1].z - point[0].z;

	float treel = pos.x - size;
	float treeu = pos.y + size;
	float treew = size * 2;
	float treeh = size * 2;
	//check if its to much to the right or left
	if (objl > treel + treew || objl + objw < treel) {
		return false;
	}
	if (obju < treeu - treeh || obju - objh > treeu) {
		return false;
	}
	return true;
}
