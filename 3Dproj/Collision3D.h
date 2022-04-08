#pragma once
#include "Vec.h"
#include "GameObject.h"
#include <DirectXCollision.h>

struct ColCube {
	ColCube(vec3 highPoint, vec3 lowPoint) {
		this->highPoint = highPoint;
		this->lowPoint = lowPoint;
	};
	ColCube(DirectX::XMVECTOR boundingBox[]) {
		lowPoint = vec3(boundingBox[0].m128_f32[0], boundingBox[0].m128_f32[1], boundingBox[0].m128_f32[2]);
		highPoint = vec3(boundingBox[1].m128_f32[0], boundingBox[1].m128_f32[1], boundingBox[1].m128_f32[2]);
	};
	ColCube(vec3 highPoint, float width, float height, float depth) {
		this->highPoint = highPoint;
		this->lowPoint = highPoint - vec3(width, height, depth);
	};
	vec3 highPoint;
	vec3 lowPoint;
};
struct ColSphere {
	ColSphere(vec3 position, float size) {
		this->position = position;
		this->size = size;
	};
	vec3 position;
	float size;
};

void collisionWithBlocking(GameObject*& objectA, GameObject*& objectB);

//two spheres
bool collision3D(ColSphere objectA, ColSphere objectB);
bool collision3D(ColSphere objectA, ColCube objectB);

//sphere and a cube
bool collision3D(ColCube objectA, ColCube objectB);

//collision with two aligned axis cubes
bool collision3D(DirectX::BoundingBox objectA, DirectX::BoundingBox objectB);