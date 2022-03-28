#pragma once
#include <d3d11.h>
#include "Vec.h"
#include "Graphics.h"
#include "Model.h"

class object {
public:
	object();
	object(vec3 pos);
	virtual ~object();
	virtual void draw(Graphics*& gfx, bool sm) = 0;

	vec3 getPos();
	const vec3 getRot();
	vec3 getScale();
	const vec3 getPoint();

	//TODO : change names on these
	void setPos(vec3 pos);
	void setRot(vec3 pos);
	void setScale(vec3 pos);
	//change the mid point that the object rotate around 
	void setPoint(vec3 point);
	//change the mid point that the object rotate around 
	void movePoint(vec3 point);
	void movePos(vec3 pos);
	//current rotation + rot
	void addRot(vec3 rot);
	void addScale(vec3 scale);

	ID3D11Buffer*& getVertexConstBuffer();
	void getKdKa(float kd[4], float ka[4]);

	//debug
	float& getxRot();
	float& getyRot();
	float& getzRot();
	void setModel(ModelObj* m);

	float& getxPos();
	float& getyPos();
	float& getzPos();

	void updateVertexShader(Graphics*& gfx);
	void updatePixelShader(Graphics*& gfx);
private:
	vec3 pos;
	vec3 rot;
	vec3 scale;
	vec3 rPoint;
	ModelObj* model;
	ID3D11Buffer* Vg_pConstantBuffer;
private:
	//test to see if we can make it faster
	DirectX::XMMATRIX scaleMat;
	DirectX::XMMATRIX transMat;
	DirectX::XMMATRIX pointMat;
};

