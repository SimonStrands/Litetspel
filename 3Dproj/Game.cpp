#include "Game.h"

//git
Game::Game(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	gfx = new Graphics(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	defRend = new DeferredRendering(gfx);
	//Create a buffer for the light const buffer(hlsli)
	CreateConstBuffer(gfx, gfx->getConstBuffers(0), sizeof(*gfx->getLightconstbufferforCS()), gfx->getLightconstbufferforCS());
	CreateConstBuffer(gfx, gfx->getConstBuffers(1), sizeof(*gfx->getCamPosconstbuffer()), gfx->getCamPosconstbuffer());
	//Resource manager
	rm = new ResourceManager(gfx);

	//create lights
	nrOfLight = 1;
	light = new Light * [nrOfLight];
	light[0] = new DirLight(vec3(0,60,8), vec3(0.1f, -PI/2, 1.f));
	//light[1] = new SpotLight(vec3(18, 46, 45), vec3(-2.4f, -0.5, 1));
	//light[2] = new SpotLight(vec3(8, 47.f, 0), vec3(0, -1, 1));
	//light[3] = new SpotLight(vec3(30, 50, 0), vec3(-1, -1, 1));
	gfx->getLightconstbufferforCS()->nrOfLights.element = nrOfLight;
	
	//shadow map needs to take more lights
	this->shadowMap = new ShadowMap((SpotLight**)light, nrOfLight, gfx);
	
	gfx->takeIM(&this->UIManager);
	mus = new Mouse(gfx->getWH());
	camera = new Camera(gfx, mus, vec3(0,0,0), vec3(1,0,0));
	camera->setData();
	
	setUpObject();
	//Qtree = new QuadTree(stataicObj, vec2(0, 0), 4, 200);
	Qtree = new QuadTree(stataicObj, vec2(0, 0), 4, 100);
	//(pi,3.14) = 180 degrees
	Qtree->setUpCamProp(2000);
	
	
 	bill = new BillBoard(gfx, vec3(0.f, 0.f, 9.f), rm->getFire(), rm->getDef()[1], 6);
	billManager = new BillBoardManager(gfx, rm->getFire(), 10, vec3(0,0,0),vec3(5,5,5));
	billManager->setAnimation(6, 1, 0.16f);
	////DCube cannot use standard obj:s without fucking others shaders
	DCube = new DynamicCube(rm->get_Models("roundsol.obj", gfx), gfx, vec3(5.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(2.f, 2.0f, 2.0f));
	//DCube = new DynamicCube(rm->get_Models("DCube.obj"), gfx, vec3(5.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(2.f, 2.0f, 2.0f));
	/////LIGHT////////
	for (int i = 0; i < nrOfLight; i++) {
		LightVisualizers.push_back(new GameObject(rm->get_Models("Camera.obj"), gfx, light[i]->getPos(), light[i]->getRotation(), vec3(1.f, 1.0f, 1.0f)));
	}
	//UI
	//light
	for (int i = 0; i < nrOfLight; i++) {
		UIManager.takeLight(light[i]);
	}
	//camera
	//UIManager.takeObject(obj[1]);
	
	
	gfx->takeLight((SpotLight**)light, nrOfLight);
	
	lightNr = 0;
}

Game::~Game() 
{
	//part of game
 	TC::GetInst().empty();
	delete gfx;
	delete rm;

	//logic and other
	delete defRend;
	delete mus;
	delete camera;
	if (shadowMap != nullptr) {
		delete shadowMap;
	}

	//objects
	for (int i = 0; i < LightVisualizers.size(); i++) {
		delete light[i];
		delete LightVisualizers[i];
	}
	delete[] light;
	for (int i = 0; i < obj.size(); i++) {
		delete obj[i];
	}
	for (int i = 0; i < stataicObj.size(); i++) {
		delete stataicObj[i];
	}
	delete Qtree;
	delete DCube;
	delete bill; 
	delete billManager;
}


void Game::run()
{
	static bool once = true;
	while (msg.message != WM_QUIT && once)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		gfx->clearScreen();
		gfx->setTransparant(false);
		//for shadow
		//måste uppdatera detta så inte hela object uppdateras när bara skugga ska
		shadowMap->setUpdateShadow();
		vec3 camLP = camera->getPos();
		vec3 camLR = camera->getRot();
		for (int i = 0; i < nrOfLight; i++) {
			//set cam position to lightposition
			camera->setPosition(light[i]->getPos());
			camera->setRotation(light[i]->getRotation());
			shadowMap->inUpdateShadow(i);
			updateShaders(true, false);
			
			DrawAllShadowObject();
		}
		//set cam position so its the real cam
		camera->setPosition(camLP);
		camera->setRotation(camLR);
		gfx->setProjection(0);//last can be dir light


		Update();
		updateShaders();

		bill->UpdateShader(gfx, camera->getPos());
		//DrawDynamicCube();
		//
		defRend->BindFirstPass();

		this->DrawToBuffer();

		defRend->BindSecondPass(shadowMap->GetshadowResV());

		gfx->setTransparant(true);
		gfx->setRenderTarget();
		this->ForwardDraw();
		gfx->present(this->lightNr);

		//once = false;
	}
	printf("quit"); 
}

void Game::Update()
{
	dt.restartClock();
	//keyboard
	
	//obj[3]->addRot(vec3(0, 1.f * dt.dt(), 0));
	camera->updateCamera((float)dt.dt());
	if (getkey('N')) {
		DirectX::XMMATRIX viewMatrix = DirectX::XMMATRIX(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			-obj[1]->getPos().x, -obj[1]->getPos().y, -obj[1]->getPos().z, 1.0f
		);
		XRotation(viewMatrix, obj[1]->getRot().x);
		YRotation(viewMatrix, obj[1]->getRot().y);
		gfx->getVertexconstbuffer()->view.element = viewMatrix;
	}

	bill->update((float)dt.dt());
	billManager->update(dt.dt(), gfx);
	mus->UpdateMouse();
	for (int i = 0; i < LightVisualizers.size(); i++) {
		LightVisualizers[i]->setPos(light[i]->getPos());
		LightVisualizers[i]->setRot(vec3(0 , light[i]->getRotation().x, -light[i]->getRotation().y) + vec3(0,1.57f,0));
	}
	
	gfx->Update((float)dt.dt(), camera->getPos());

#pragma region camera_settings
	if (getkey('C')) {
		camera->setPosition(light[lightNr]->getPos());
		camera->setRotation(light[lightNr]->getRotation());
	}
	if (getkey('1')) {
		lightNr = 0;
	}
	if (getkey('2')) {
		lightNr = 1;
	}
	if (getkey('3')) {
		lightNr = 2;
	}
	if (getkey('4')) {
		lightNr = 3;
	} 
#pragma endregion camera_settings
}

void Game::DrawToBuffer()
{	
	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IMctx()->IASetInputLayout(gfx->getInputLayout()[0]);
	gfx->get_IMctx()->GSSetShader(nullptr, nullptr, 0);
	gfx->get_IMctx()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	gfx->get_IMctx()->HSSetShader(nullptr, nullptr, 0);
	gfx->get_IMctx()->DSSetShader(nullptr, nullptr, 0);
	for (int i = 0; i < obj.size(); i++) {
		obj[i]->draw(gfx);
	}
    camera->calcFURVectors();
	Qtree->draw(gfx, camera);
	Qtree->clearAlrDraw();

	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IMctx()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	gfx->get_IMctx()->HSSetShader(nullptr, nullptr, 0);
	gfx->get_IMctx()->DSSetShader(nullptr, nullptr, 0);
	gfx->get_IMctx()->PSSetShader(gfx->getPS()[0], nullptr, 0);
	for (int i = 0; i < LightVisualizers.size(); i++) {
		LightVisualizers[i]->draw(gfx, false);
	}
}

void Game::DrawDynamicCube()
{
	gfx->setProjection(2);
	vec3 camLP = camera->getPos();
	vec3 camRT = camera->getRot();
	vec3 DCRot = DCube->getRot();
	camera->setPosition(vec3(DCube->getPos()));
	camera->setRotation(vec3(DCube->getRot()));
	DCube->ClearRenderTarget(gfx);

	//draw all textures
	for (int i = 0; i < 6; i++) {
		//change camera angel 2
		switch (i)
		{
		case 0:
			camera->setRotation(vec3(DCRot.x + (PI/2), DCRot.y, 0));//right
			break;
		case 1:
			camera->setRotation(vec3(DCRot.x - (PI / 2), DCRot.y, 0));//left
			break;
		case 2:
			camera->setRotation(vec3(DCRot.x, DCRot.y+ (PI / 2), 0));//up
			break;
		case 3:
			camera->setRotation(vec3(DCRot.x, DCRot.y - (PI / 2), 0));//bottom
			break;
		case 4:
			camera->setRotation(vec3(DCRot.x, DCRot.y, 0));//forward
			break;
		case 5:
			camera->setRotation(vec3(DCRot.x + PI, DCRot.y, 0));//back
			break;
		}
		camera->updateCamera();
		updateShaders(true, false);
		DCube->setViewPort(gfx);
		
		//DEFFERED RENDERING
		DCube->firstPass();//needed
		this->DrawToBuffer();
		DCube->secondPass(shadowMap->GetshadowResV(), DCube->getUAVs()[i], 16, 40, DCube->getCSShader());
		//FORWARD RENDERING
		DCube->setRenderTarget(gfx, i);
		this->ForwardDrawCube();
		
	
	}

	//turn things back to how they where
	gfx->RsetViewPort();
	gfx->setProjection(0);
	camera->setPosition(camLP);
	camera->setRotation(camRT);
	camera->updateCamera();

	//for DEBUG CAMERA
	if (getkey('N')) {
		DirectX::XMMATRIX viewMatrix = DirectX::XMMATRIX(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			-obj[1]->getPos().x, -obj[1]->getPos().y, -obj[1]->getPos().z, 1.0f
		);
		XRotation(viewMatrix, obj[1]->getRot().x);
		YRotation(viewMatrix, obj[1]->getRot().y);
		gfx->getVertexconstbuffer()->view.element = viewMatrix;
	}
	updateShaders(true, false);
}

void Game::ForwardDraw()
{
	gfx->get_IMctx()->IASetInputLayout(gfx->getInputLayout()[0]);
	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IMctx()->GSSetShader(nullptr, nullptr, 0);
	gfx->get_IMctx()->HSSetShader(nullptr, nullptr, 0);
	gfx->get_IMctx()->DSSetShader(nullptr, nullptr, 0);
	//DCube->update(camera->getPos(), gfx);
	//DCube->draw(gfx->get_IMctx());

	gfx->get_IMctx()->IASetInputLayout(gfx->getInputLayout()[1]);
	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	gfx->get_IMctx()->VSSetShader(gfx->getVS()[1], nullptr, 0);
	gfx->get_IMctx()->GSSetShader(gfx->getGS()[0], nullptr, 0);
	gfx->get_IMctx()->PSSetShader(gfx->getPS()[1], nullptr, 0);
	gfx->get_IMctx()->HSSetShader(nullptr, nullptr, 0);
	gfx->get_IMctx()->DSSetShader(nullptr, nullptr, 0);
	bill->draw(gfx);
	billManager->draw(gfx);
}

//the Dynamic cube that draw these infront
void Game::ForwardDrawCube()
{
	gfx->get_IMctx()->IASetInputLayout(gfx->getInputLayout()[1]);
	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	gfx->get_IMctx()->VSSetShader(gfx->getVS()[1], nullptr, 0);
	gfx->get_IMctx()->GSSetShader(gfx->getGS()[0], nullptr, 0);
	gfx->get_IMctx()->PSSetShader(gfx->getPS()[1], nullptr, 0);
	gfx->get_IMctx()->HSSetShader(nullptr, nullptr, 0);
	gfx->get_IMctx()->DSSetShader(nullptr, nullptr, 0);
	bill->draw(gfx);
	billManager->draw(gfx);
}

void Game::DrawAllShadowObject()
{
	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IMctx()->GSSetShader(nullptr, nullptr, 0);
	gfx->get_IMctx()->PSSetShader(nullptr, nullptr, 0);
	for (int i = 0; i < obj.size(); i++) {

		obj[i]->draw(gfx, true);
	}
	camera->calcFURVectors();
	Qtree->draw(gfx, camera, true);
	Qtree->clearAlrDraw();
}

void Game::updateShaders(bool vs, bool ps)
{

	billManager->updateShader(gfx, camera->getPos());
	if (vs)
	{
		DCube->updateVertexShader(gfx);
		for (int i = 0; i < obj.size(); i++) {
			obj[i]->updateVertexShader(gfx);
		}
		for (int i = 0; i < LightVisualizers.size(); i++) {
			LightVisualizers[i]->updateVertexShader(gfx);
		}
	}
	if (ps) {
		DCube->updatePixelShader(gfx);
		for (int i = 0; i < obj.size(); i++) {
			obj[i]->updatePixelShader(gfx);
		}
		for (int i = 0; i < LightVisualizers.size(); i++) {
			LightVisualizers[i]->updatePixelShader(gfx);
		}
		for (int i = 0; i < stataicObj.size(); i++) {
			stataicObj[i]->updatePixelShader(gfx);
		}
	}
}

void Game::setUpObject()
{
	////////OBJECTS///////////
	//cameras
	//obj.push_back(new GameObject(rm->get_Models("Camera.obj", gfx), gfx, vec3(0.f, 0.f, 10.f), vec3(0.f, 0.f, 0.f), vec3(2.f, 2.0f, 2.0f)));//main
	//obj.push_back(new GameObject(rm->get_Models("Camera.obj", gfx), gfx, vec3(0.f, 100.f, 0.f), vec3(0.f, -1.58f, 0.f), vec3(2.f, 2.0f, 2.0f)));//second
	////
	//////OBJECTS
	//obj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(10.f, 5.f, 10.f), vec3(-1.56f, 1.56f, 3.2f), vec3(1.f, 1.f, 1.f)));
	//obj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(-5.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	//obj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(0.f, 0.f, -50.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	////walls
	////obj.push_back(new GameObject(rm->get_Models("quad2.obj"), gfx, vec3(0.f, 5.f, 20.f),  vec3(-PI/2, -PI/2, PI), vec3(20.f, 20.f, 20.f)));
	//obj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(20.f, 5.f, 0.f),  vec3(-PI/2, 0.f, PI),   vec3(20.f, 20.f, 20.f)));
	//obj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(0.f, 5.f, -20.f), vec3(-PI/2, PI/2, PI),  vec3(20.f, 20.f, 20.f)));
	//obj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(-20.f, 5.f, 0.f), vec3(-PI/2, PI, PI),   vec3(20.f, 20.f, 20.f)));
	
	//static
	/*
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx),        gfx, vec3(25.f, 0.f, 0.f), vec3(0.f, 0.f, -1.57f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx),        gfx, vec3(0.f, 0.f, 25.f), vec3(0.f, 0.f, 1.57f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx),        gfx, vec3(-25.f, 0.f, 0.f), vec3(-1.57f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx),        gfx, vec3(0.f, 0.f, -25.f), vec3(1.57f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(25.f, 0.f, -5.f), vec3(0.f, 0.f, 0.f), vec3(0.3f, 0.3f, 0.3f)));
	
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx),     gfx, vec3(25.f, 0.f, 5.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(-37.5f, 0.f, -37.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(37.5f, 0.f, -37.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(-37.5f, 0.f, 37.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(37.5f, 0.f, 37.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(12.5f, 0.f, 37.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(12.5f, 0.f, -37.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(-12.5f, 0.f, 37.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(-12.5f, 0.f, -37.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(37.5f, 0.f, 12.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(-37.5f, 0.f, 12.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(37.5f, 0.f, -12.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(-37.5f, 0.f, -12.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(12.5f, 0.f, 12.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(-12.5f, 0.f, 12.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(12.5f, 0.f, -12.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(12.5f, 0.f, -12.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	*/
	//int nrOfPlantsXY = 16;
	//float sizeOfTree = 100 * 2;
	//for (int x = 0; x < nrOfPlantsXY; x++) {
	//	for (int y = 0; y < nrOfPlantsXY; y++) {
	//		stataicObj.push_back(new GameObject(rm->get_IDK(),
	//			gfx, vec3(((sizeOfTree/ nrOfPlantsXY) * x) - sizeOfTree/2, 0.f, ((sizeOfTree / nrOfPlantsXY) * y) - sizeOfTree / 2),
	//			vec3(0.f, 0.f, 1.58f), vec3(5.f, 5.f, 5.f)));
	//	}
	//}
	
	float gw = 8;
	float gn = 8;
	for (int x = 0; x < gn; x++) {
		for (int y = 0; y < gn; y++) {
			stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(x*(gw*2) - ((gn)*gw), -4, y*(gw * 2) - ((gn)*gw)), vec3(0, 0, 1.57f), vec3(gw, gw, gw)));
			//stataicObj.push_back(new GameObject(rm->get_Models("DoubleMesh.obj", gfx), gfx, vec3(x*(gw*2) - ((gn)*gw), -4, y*(gw * 2) - ((gn)*gw)), vec3(0, 0, 1.57f), vec3(1, 1, 1)));
		}
	}


	//obj[3]->setTesselation(true, gfx);
}
