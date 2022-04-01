#pragma once
#include "Graphics.h"
#include "deltaTime.h"
#include "rotation.h"
#include "Keyboard.h"
#include "Vec.h"
#include "Mouse.h"

class Camera {
public:
	Camera(Graphics *&gfx, Mouse *mouse = nullptr, vec3 pos = vec3(0,0,10), vec3 rot = vec3(0,0,1));
	virtual ~Camera();
	void updateCamera();
	void updateCamera(float dt);
	vec3 getPos();
	vec3 getRot();
	/*FUL = Forward, UP, Right, this operation must be done before get-forward/up/right Vector function is called each frame*/
	void calcFURVectors();
	vec3 getForwardVec();
	vec3 getUpVector();
	vec3 getRightVector();
	/*
	calcs the directions of the frustom , 0 = left, 1 = right, 2 = up, 3 = down
	when using: cross product the direction with FURVectors or in another way to use them
	*/
	void getViewFrustoms(vec3 frustoms[]);
	void setRotation(vec3 newRot);
	void setPosition(vec3 newpos);
	void addRotation(vec3 addRot);
	void rotateCameraMouse(vec3 Rot, float dt);
	void movePos(vec3 move);
	//void lookDir(vec3 lookdir); //don't know how to make these yet
	//void lookAt(vec3 lookat);	  //don't know how to make these yet
	void setData(float FOVRadians = 90, float viewRatio = 16/9, float nearDist = 0.001, float farDist = 2000);
private:
	Mouse *mouse;
	Vcb *Vcbd;
	LCBGS* Lcbd;
	void rotaiton(DirectX::XMMATRIX &matrix);
	void rotaiton(DirectX::XMMATRIX& matrix, float xRot, float yRot);
	void movement();
	void handleEvent(float dt);

	vec3 FUL[3];//Forward, Up, Left
	DirectX::XMFLOAT3 translation;
	float movementspeed = 20;
	void Translate(float dt);

	//data
	float speed;
	float mouseSensitivity;
	float xCamPos;
	float yCamPos;
	float zCamPos;
	float xCamRot;
	float yCamRot;
	float zCamRot;

	//other data
	float ratio;
	float fov;
	float nearDist;
	float farDist;
};