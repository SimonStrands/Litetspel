#include "rotation.h"
//git
void XRotation(DirectX::XMMATRIX &Matrix, const float rot)
{
	//X rotation
	DirectX::XMMATRIX Mrot(
		 std::cos(rot), 0.0f, std::sin(rot), 0.0f ,
		 0.0f,			 1.0f,	0.0f,			 0.0f ,
		 -std::sin(rot),0.0f, std::cos(rot), 0.0f ,
		 0.0f,			 0.0f,			 0.0f,  1.0f 
);
	Matrix = Matrix * Mrot;
}

void YRotation(DirectX::XMMATRIX &Matrix, const float rot)
{
	DirectX::XMMATRIX Mrot(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, std::cos(rot), std::sin(rot), 0.0f,
		0.0f, -std::sin(rot), std::cos(rot), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	Matrix = Matrix * Mrot;
}

void ZRotation(DirectX::XMMATRIX &Matrix, const float rot)
{
	DirectX::XMMATRIX Mrot(
		std::cos(rot), -std::sin(rot), 0.0f, 0.0f,
		std::sin(rot), std::cos(rot), 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	Matrix = Matrix * Mrot;
}

void XRotation(DirectX::XMVECTOR& Matrix, const float rot)
{
	//X rotation
	DirectX::XMMATRIX Mrot(
		std::cos(rot), 0.0f, std::sin(rot), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-std::sin(rot), 0.0f, std::cos(rot), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	Matrix = XMVector3Transform(Matrix, Mrot);
}

void YRotation(DirectX::XMVECTOR& Matrix, const float rot)
{
	//Y rotation
	DirectX::XMMATRIX Mrot(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, std::cos(rot), std::sin(rot), 0.0f,
		0.0f, -std::sin(rot), std::cos(rot), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	Matrix = XMVector3Transform(Matrix, Mrot);
}
void ZRotation(DirectX::XMVECTOR& Matrix, const float rot)
{
	//Z rotation
	DirectX::XMMATRIX Mrot(
		std::cos(rot), -std::sin(rot), 0.0f, 0.0f,
		std::sin(rot), std::cos(rot), 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	Matrix = XMVector3Transform(Matrix, Mrot);
}