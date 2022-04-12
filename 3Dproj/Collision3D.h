#pragma once
#include "Vec.h"
#include "GameObject.h"
#include <DirectXCollision.h>
#include "otherHelps.h"
#include "rect.h"

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

 

bool RayRect(vec3 &rayOrigin, vec3 rayDir, ColCube &cube, vec3 &cp, vec3 &cn, float &t);
bool RectRect(GameObject*& objectA, ColCube& in, ColCube& target, vec3& cp, vec3& cn, float& contact_t, float elapsed_t);

//only moves object A!!!
void moveObject(GameObject*& objectA, GameObject*& objectB, ColCube colCubeA, ColCube colCubeB, float dt);
//void collisionWithBlocking(GameObject*& objectA, GameObject*& objectB);
void collisionWithBlocking(GameObject*& objectA, GameObject*& objectB, float dt);


//two spheres
bool collision3D(ColSphere objectA, ColSphere objectB);
bool collision3D(ColSphere objectA, ColCube objectB);

//sphere and a cube
bool collision3D(ColCube objectA, ColCube objectB);

//collision with two aligned axis cubes
bool collision3D(DirectX::BoundingBox objectA, DirectX::BoundingBox objectB);