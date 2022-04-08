#include "Collision3D.h"

void collisionWithBlocking(GameObject*& objectA, GameObject*& objectB)
{
	DirectX::XMVECTOR a[2];
	DirectX::XMVECTOR b[2];
	objectA->getBoundingBox(a);
	objectB->getBoundingBox(b);
	if (collision3D(ColCube(a), ColCube(b))) 
	{
		objectA->movePos(objectA->getlastPosition() - objectA->getPos());
		objectB->movePos(objectB->getlastPosition() - objectB->getPos());
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
	//check if one side is inside
	if (
		//check x
		(
			objectA.lowPoint.x < objectB.highPoint.x &&
			objectA.highPoint.x > objectB.lowPoint.x 
			//||
			//objectA.lowPoint.x < objectB.lowPoint.x &&
			//objectA.highPoint.x > objectB.highPoint.x
		) &&
		//check y
		(
			objectA.lowPoint.y < objectB.highPoint.y &&
			objectA.highPoint.y > objectB.lowPoint.y 
			//||
			//objectA.lowPoint.y < objectB.lowPoint.y &&
			//objectA.highPoint.y > objectB.highPoint.y
		) 
		&&
		//check z
		(
			objectA.lowPoint.z < objectB.highPoint.z &&
			objectA.highPoint.z > objectB.lowPoint.z 
			//||
			//objectA.lowPoint.z < objectB.lowPoint.z &&
			//objectA.highPoint.z > objectB.highPoint.z
		)
		)
	{
		std::cout << "hit " << std::endl;
		return true;
	}
	return false;
}


bool collision3D(DirectX::BoundingBox objectA, DirectX::BoundingBox objectB)
{
	return objectA.Intersects(objectB);
}
