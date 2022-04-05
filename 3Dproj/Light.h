#pragma once
#include "Vec.h"
#include "rotation.h"
#include "Convertions.h"
#include <iostream>
//git

//whatTypeOfLight so I don't ahve to do a dynamic_cast
enum wTofL {
	SPOT, DIR, PLIGHT
};

class Light {
public:
	Light(vec3 pos);
	virtual ~Light();
	vec3& getRotation();
	vec3& getPos();
	vec3& getColor();
protected:
	wTofL flag;
private:
	vec3 color;
	vec3 pos;
};


class SpotLight : public Light{
public:
	SpotLight(vec3 pos, vec3 rot = vec3(0,0,1));
	virtual ~SpotLight();
	vec3& getRotation();
	void changeRot(vec3 rot);
	void addRot(vec3 rot);
	//get view matrix from light
	DirectX::XMMATRIX getLightView();
	//get light and view matrix from light
	DirectX::XMMATRIX getLightViewProj();
	DirectX::XMMATRIX getLightProj();
	//what type of light if its spotlight or directional light
	const wTofL whatOfLight();
	
protected:
	DirectX::XMMATRIX Projection;
private:
	vec3 rot;
};

class DirLight : public SpotLight {
public:
	DirLight(vec3 pos, vec3 rot = vec3(0, 0, 1), float W = 50, float H = 50);
};

class PointLight : public Light {
public:
	PointLight(vec3 pos, float falloff, vec3 Color = vec3(1, 1, 1));
};