#include "Collision3D.h"


bool RayRect(vec3 &rayOrigin, vec3 rayDir, ColCube &cube, vec3 &cp, vec3 &cn, float &t)
{
	vec3 t_near = (cube.lowPoint - rayOrigin) / rayDir;
	vec3 t_far = (cube.highPoint - rayOrigin) / rayDir;

	float txMin = (cube.lowPoint.x - rayOrigin.x) / rayDir.x;
	float txMax = (cube.highPoint.x - rayOrigin.x) / rayDir.x;
	float tyMin = (cube.lowPoint.y - rayOrigin.y) / rayDir.y;
	float tyMax = (cube.highPoint.y - rayOrigin.y) / rayDir.y;
	float tzMin = (cube.lowPoint.z - rayOrigin.z) / rayDir.z;
	float tzMax = (cube.highPoint.z - rayOrigin.z) / rayDir.z;
	if (txMax < txMin) { float tmp = txMax; txMax = txMin; txMin = tmp; }
	if (tyMax < tyMin) { float tmp = tyMax; tyMax = tyMin; tyMin = tmp; }
	if (tzMax < tzMin) { float tmp = tzMax; tzMax = tzMin; tzMin = tmp; }

	float tMin = txMin;
	if (tMin < tyMin) { tMin = tyMin; }
	if (tMin < tzMin) { tMin = tzMin; }
	float tMax = txMax;
	if (tMax > tyMax) { tMax = tyMax; }
	if (tMax > tzMax) { tMax = tzMax; }
	
	if (!(txMin > tyMax || tyMin > txMax)) {
		if (!(tMin > tzMax || tzMin > tMax)) {
			if (tzMin > tMin) {
				tMin = tzMin;
			}
			if (tzMax > tMax) {
				tMax = tzMax;
			}
			float distance = tMin;
			if (distance < 0) {
				distance = tMax;
			}
			if (distance > 0) {
				t = distance;
				cp = rayOrigin + rayDir * t;
				//check normal
				if (t_near.x > t_near.y && t_near.x > t_near.z) {
					if (rayDir.x < 0) {
						cp = vec3(1, 0, 0);
					}
					else {
						cp = vec3(-1, 0, 0);
					}
				}
				else if(t_near.y > t_near.x && t_near.y > t_near.z) {
					if (rayDir.y < 0) {
						cp = vec3(0, 1, 0);
					}
					else {
						cp = vec3(0, -1, 0);
					}
				}
				else if (t_near.z > t_near.y && t_near.z > t_near.x) {
					if (rayDir.z < 0) {
						cp = vec3(1, 0, 1);
					}
					else {
						cp = vec3(1, 0, 0);
					}
				}

			}
			//returnVar.distance = distance;  
		}
	}

	return false;
}

bool RectRect(GameObject*& objectA, ColCube& in, ColCube& target, vec3& cp, vec3& cn, float& contact_t, float elapsed_t)
{
	vec3 rayDir = objectA->getPos() - objectA->getlastPosition();
	if (rayDir.x == 0 && rayDir.y == 0 && rayDir.z == 0) {
		return false;
	}
	ColCube expandedTarget(
		target.highPoint + (in.highPoint - in.lowPoint) * 0.5,
		target.lowPoint - (in.highPoint - in.lowPoint) * 0.5
	);
	vec3 pos = objectA->getPos();
	if (RayRect(pos, rayDir * elapsed_t, expandedTarget, cp, cn, contact_t)) {
		if (contact_t > 0 && contact_t < 1.f) {
			return true;
		}
	}
	
	return false;
}

void moveObject(GameObject*& objectA, GameObject*& objectB, ColCube colCubeA, ColCube colCubeB, float dt)
{
	vec3 lowdir(colCubeA.highPoint - colCubeB.lowPoint);
	vec3 highdir(colCubeB.highPoint - colCubeA.lowPoint);
	float dirs[6]{ lowdir.x,lowdir.y ,lowdir.z ,highdir.x ,highdir.y ,highdir.z };
	float lowest = dirs[0];
	int index = 0;
	for (int i = 1; i < 6; i++) {
		if (lowest > dirs[i]) {
			index = i;
			lowest = dirs[i];
		}
	}
	
	vec3 dir;
	switch (index)
	{
	case 0:
		objectA->movePos(vec3(-lowdir.x,0,0));
		break;
	case 1:
		objectA->movePos(vec3(0, -lowdir.y, 0));
		break;
	case 2:
		objectA->movePos(vec3(0, 0, -lowdir.z));
		break;
	case 3:
		objectA->movePos(vec3(highdir.x, 0, 0));
		break;
	case 4:
		//objectA->movePos(vec3(0, highdir.y, 0));
		//+ (objectA->getPos().y - colCubeA.lowPoint.y)
		//objectA->setPos(vec3(objectA->getPos().x, colCubeB.highPoint.y , objectA->getPos().z));
		objectA->setPos(vec3(objectA->getPos().x, 
			objectB->getPos().y + objectB->getWidthHeightDepth().y/2 + objectA->getWidthHeightDepth().y/2 - 0.6, 
			objectA->getPos().z));
		break;
	case 5:
		objectA->movePos(vec3(0, 0, highdir.z));
		break;
	}
}

static int oua = 0;
void collisionWithBlocking(GameObject*& objectA, GameObject*& objectB, float dt)
{
	DirectX::XMVECTOR a[2];
	DirectX::XMVECTOR b[2];
	objectA->getBoundingBox(a);
	objectB->getBoundingBox(b);
	ColCube colCubeA(a);
	ColCube colCubeB(b);
	if (collision3D(colCubeA, colCubeB))
	{
		//check later if object can move
		moveObject(objectA, objectB, colCubeA, colCubeB, dt);
		//change so the other object move?
		//moveObject(objectA, objectB, colCubeA, colCubeB, dt);
	}
}

bool collision3D(ColSphere objectA, ColSphere objectB)
{
	return (objectA.position - objectB.position).length() < objectA.size + objectB.size;
}

bool collision3D(ColSphere objectA, ColCube objectB)
{
	float sqDist = 0.0f;
	for (int i = 0; i < 3; i++) {
		float v = objectA.position.getWithNumber(i);
		if (v < objectB.lowPoint.getWithNumber(i)) {
			sqDist += (objectB.lowPoint.getWithNumber(i) - v) * (objectB.lowPoint.getWithNumber(i) - v);
		}
		if (v > objectB.highPoint.getWithNumber(i)) {
			sqDist += (v - objectB.highPoint.getWithNumber(i)) * (v - objectB.highPoint.getWithNumber(i));
		}
	}
	return sqDist <= objectA.size * objectA.size;
}

bool collision3D(ColCube objectA, ColCube objectB)
{
	return (
		//check x
		(
			objectA.lowPoint.x < objectB.highPoint.x&&
			objectA.highPoint.x > objectB.lowPoint.x
			) &&
		//check y
		(
			objectA.lowPoint.y < objectB.highPoint.y&&
			objectA.highPoint.y > objectB.lowPoint.y
			)
		&&
		//check z
		(
			objectA.lowPoint.z < objectB.highPoint.z&&
			objectA.highPoint.z > objectB.lowPoint.z
			)
		);
}



bool collision3D(DirectX::BoundingBox objectA, DirectX::BoundingBox objectB)
{
	return objectA.Intersects(objectB);
}
