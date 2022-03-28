#include "Collision.h"

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

/*
//TODO : check this?
	for (int i = 0; i < 2; i++) {
		if (point[i].x < pos.x + size / 2 &&
			point[i].x > pos.x - size / 2 &&
			point[i].z < pos.y + size / 2 &&
			point[i].z < pos.y - size / 2) {
			return true;
		}
	}
	//check the two other points
	if (point[1].x < pos.x + size / 2 &&
		point[0].y < pos.x + size / 2 &&
		point[1].x > pos.x - size / 2 &&
		point[0].y > pos.x - size / 2) {
		return true;
	}
	if (point[0].x < pos.x + size / 2 &&
		point[1].y < pos.x + size / 2 &&
		point[0].x > pos.x - size / 2 &&
		point[1].y > pos.x - size / 2) {
		return true;
	}

	//check if the tringle covers the current triangle
	if (point[0].x < pos.x - size / 2 &&
		point[0].z < pos.y - size / 2 &&
		point[1].x > pos.x + size / 2 &&
		point[1].z > pos.y + size / 2)
	{
		return true;
	}
	return false;
*/