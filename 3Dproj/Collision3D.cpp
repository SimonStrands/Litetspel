#include "Collision3D.h"

void collisionWithBlocking(GameObject*& objectA, GameObject*& objectB)
{
}

bool collision3D(ColCube objectA, ColCube objectB)
{
	if (
		//check x
		(
			objectA.lowPoint.x < objectB.highPoint.x &&
			objectA.highPoint.x > objectB.lowPoint.x 
		) &&
		//check y
		(
			objectA.lowPoint.y < objectB.highPoint.y &&
			objectA.highPoint.y > objectB.lowPoint.y
		) 
		&&
		//check z
		(
			objectA.lowPoint.z < objectB.highPoint.z &&
			objectA.highPoint.z > objectB.lowPoint.z
		)
		)
	{
		std::cout << "hit " << std::endl;
		return true;
	};
	return false;
}

bool collision3D(DirectX::BoundingBox objectA, DirectX::BoundingBox objectB)
{
	return objectA.Intersects(objectB);
}
