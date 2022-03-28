#include "Convertions.h"

DirectX::XMMATRIX assimpToDX(aiMatrix4x4& mat) {
	DirectX::XMMATRIX ret(
		(float)mat.a1, (float)mat.a2, (float)mat.a3, (float)mat.a4,
		(float)mat.b1, (float)mat.b2, (float)mat.b3, (float)mat.b4,
		(float)mat.c1, (float)mat.c2, (float)mat.c3, (float)mat.c4,
		(float)mat.d1, (float)mat.d2, (float)mat.d3, (float)mat.d4
	);
	return ret;
}
vec3 assimpToVec(aiVector3D vec)
{
	vec3 theReturn;
	theReturn.x = vec.x;
	theReturn.y = vec.y;
	theReturn.z = vec.z;
	return theReturn;
}
vec4 assimpQuartToVec4(aiQuaternion quart)
{
	vec4 theReturn;
	theReturn.w = quart.w;
	theReturn.xyz.x = quart.x;
	theReturn.xyz.y = quart.y;
	theReturn.xyz.z = quart.z;
	return theReturn;
};

Quaternion assimpQuartToQuart(aiQuaternion quart)
{
	Quaternion theReturn;
	theReturn.w = quart.w;
	theReturn.x = quart.x;
	theReturn.y = quart.y;
	theReturn.z = quart.z;
	return theReturn;
};

DirectX::XMMATRIX FlipDiagonal(DirectX::XMMATRIX oth) {
	DirectX::XMFLOAT4 v2F[4];
	for (int i = 0; i < 4; i++) {
		DirectX::XMStoreFloat4(&v2F[i], oth.r[i]);
	}
	
	return DirectX::XMMATRIX(	v2F[0].x, v2F[1].x, v2F[2].x, v2F[3].x,
								v2F[0].y, v2F[1].y, v2F[2].y, v2F[3].y, 
								v2F[0].z, v2F[1].z, v2F[2].z, v2F[3].z, 
								v2F[0].w, v2F[1].w, v2F[2].w, v2F[3].w
	);
}
