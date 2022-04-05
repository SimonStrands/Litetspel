#pragma once
#include "Graphics.h"
#include "deltaTime.h"
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
#include "SoundManager.h"

#include "DebugCamera.h"
#include "flags.h"
#include "skybox.h"

//git
class Game {
public:
	Game(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
	virtual ~Game();
	void run();
	void DrawToBuffer();
	void ForwardDraw();
	void DrawAllShadowObject();//without any otherShaders
	void updateShaders(bool vs = true, bool ps = true);
	bool processMessage();
private:
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
	Mouse* mouse;
	Keyboard* keyboard;
	Camera* camera;
	ShadowMap* shadowMap;
	QuadTree* Qtree;
	SoundManager soundManager;
	SkyBox* Space;
	void setUpObject();
	void setUpLights();
	void setUpParticles();

	//game objects
	Light **light;
	std::vector<GameObject*> LightVisualizers;
	std::vector<GameObject*> obj;
	std::vector<GameObject*> stataicObj;
	std::vector<BillBoardGroup*> billboardGroups;

	//var
	int nrOfLight;//must still exist

	//debug var
	int lightNr;
};