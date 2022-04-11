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
	int getType() const;
	DirectX::XMMATRIX getLightViewProj() const;
	float getFallOff()const;
	//get light and view matrix from light
	DirectX::XMMATRIX getLightProj() const;
protected:
	DirectX::XMMATRIX Projection;
	wTofL flag;
	vec3 color;
	vec3 pos;
	vec3 rot;
	float fallOff;
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
	//what type of light if its spotlight or directional light
	const wTofL whatOfLight();
private:
	
};

class DirLight : public SpotLight {
public:
	DirLight(vec3 pos, vec3 rot = vec3(0, 0, 1), float W = 50, float H = 50);
};

class PointLight : public Light {
public:
	PointLight(vec3 pos, float falloff, vec3 Color = vec3(1, 1, 1));
};