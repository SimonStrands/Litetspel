#pragma once
#include <Windows.h>
#include "Graphics.h"
#include "deltaTime.h"
#include "Mouse.h"
#include "Camera.h"
#include "imguiManager.h"
#include "ResourceManager.h"

#include "GameObject.h"
#include "BillBoardManager.h"
#include "Light.h"

#include "DeferredRendering.h"
#include "ShadowMap.h"
#include "Graphics.h"
#include "DynamicCubeEnviroment.h"
#include "QuadTree.h"
#include "TrashCollector.h"

#include "DebugCamera.h"

//git
class Game {
public:
	Game(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
	virtual ~Game();
	void run();
	void DrawToBuffer();
	void DrawDynamicCube();
	void ForwardDraw();
	void ForwardDrawCube();
	void DrawAllShadowObject();//without any otherShaders
	void updateShaders(bool vs = true, bool ps = true);
private:
	//width and height of window
	MSG msg = {};
	Graphics *gfx;
	ResourceManager* rm;
	void Update();
	const float PI = 3.14159265359f;
private:
	//logic and others
	DeferredRendering *defRend;
	DeltaTime dt;
	ImguiManager UIManager;
	Mouse* mus;
	Camera* camera;
	ShadowMap* shadowMap;
	QuadTree* Qtree;
	void setUpObject();

	//game objects
	Light **light;
	std::vector<GameObject*> LightVisualizers;
	std::vector<GameObject*> obj;
	std::vector<GameObject*> stataicObj;
	BillBoardManager* billManager;
	DynamicCube* DCube;
	BillBoard *bill;

	//var
	int nrOfLight;//must still exist

	//debug var
	int lightNr;
};