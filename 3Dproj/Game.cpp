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

	setUpLights();
	
	//shadow map needs to take more lights
	this->shadowMap = new ShadowMap((SpotLight**)light, nrOfLight, gfx, 1280U, 720U);
	//this->shadowMap = new ShadowMap((SpotLight**)light, nrOfLight, gfx, 640u, 360U);
	
	gfx->takeIM(&this->UIManager);
	mouse = new Mouse();
	camera = new Camera(gfx, mouse, vec3(0,0,0), vec3(1,0,0));
	camera->setData();
	
	setUpObject();
	
	Qtree = new QuadTree(stataicObj, vec2(0, 0), 4, 100);
	//(pi,3.14) = 180 degrees
	Qtree->setUpCamProp(2000);	
	for (int i = 0; i < nrOfLight; i++) {
		LightVisualizers.push_back(new GameObject(rm->get_Models("Camera.obj"), gfx, light[i]->getPos(), light[i]->getRotation(), vec3(1.f, 1.0f, 1.0f)));
	}

	setUpParticles();

	/*IMGUI*/
	for (int i = 0; i < nrOfLight; i++) {
		UIManager.takeLight(light[i]);
	}
	
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
	delete mouse;
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
	for (int i = 0; i < billboardGroups.size(); i++) {
		delete billboardGroups[i];
	}
	
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
		//must look for mouse active
		if (mouse->getMouseActive()) {
			mouse->updateMouse(msg);
		}

		while (!mouse->EventBufferEmpty() && mouse->getMouseActive()) {
			mouseEvent me = mouse->ReadEvent();
			std::cout << "x:" << me.getPosX() << "\ny:" << me.getPosY() << std::endl;
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
		gfx->RsetViewPort();


		Update();
		updateShaders();
		if (def_rend){
			//deferred rendering
			defRend->BindFirstPass();
			this->DrawToBuffer();
			defRend->BindSecondPass(shadowMap->GetshadowResV());
		}
		

		//gfx->setTransparant(true);
		gfx->setRenderTarget();
		gfx->get_IMctx()->ClearDepthStencilView(gfx->getDepthStencil(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		if (!def_rend) {
			//if deferred rendfering 
			gfx->get_IMctx()->PSSetShaderResources(1, 1, &shadowMap->GetshadowResV());//add ShadowMapping
			this->DrawToBuffer();
		}
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
	obj[0]->setPos(camera->getPos());
	obj[0]->setRot(vec3(0, camera->getRot().x, -camera->getRot().y) + vec3(0, 1.57f, 0));

	for (int i = 0; i < billboardGroups.size(); i++) {
		billboardGroups[i]->update(dt.dt(), gfx);
	}
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

void Game::ForwardDraw()
{
	gfx->get_IMctx()->IASetInputLayout(gfx->getInputLayout()[1]);
	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	gfx->get_IMctx()->VSSetShader(gfx->getVS()[1], nullptr, 0);
	gfx->get_IMctx()->GSSetShader(gfx->getGS()[0], nullptr, 0);
	gfx->get_IMctx()->PSSetShader(gfx->getPS()[1], nullptr, 0);
	gfx->get_IMctx()->HSSetShader(nullptr, nullptr, 0);
	gfx->get_IMctx()->DSSetShader(nullptr, nullptr, 0);
	for (int i = 0; i < billboardGroups.size(); i++) {
		billboardGroups[i]->draw(gfx);
	}
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
	for (int i = 0; i < billboardGroups.size(); i++) {
		billboardGroups[0]->updateShader(gfx, camera->getPos());
	}
	
	if (vs)
	{
		for (int i = 0; i < obj.size(); i++) {
			obj[i]->updateVertexShader(gfx);
		}
		for (int i = 0; i < LightVisualizers.size(); i++) {
			LightVisualizers[i]->updateVertexShader(gfx);
		}
	}
	if (ps) {
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
	obj.push_back(new GameObject(rm->get_Models("Camera.obj", gfx), gfx, vec3(0.f, 0.f, 10.f), vec3(0.f, 0.f, 0.f), vec3(2.f, 2.0f, 2.0f)));//main
	obj.push_back(new GameObject(rm->get_Models("Camera.obj", gfx), gfx, vec3(0.f, 100.f, 0.f), vec3(0.f, -1.58f, 0.f), vec3(2.f, 2.0f, 2.0f)));//second
	////
	//////OBJECTS
	//obj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(10.f, 5.f, 10.f), vec3(-1.56f, 1.56f, 3.2f), vec3(1.f, 1.f, 1.f)));
	//obj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(-5.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	//obj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(0.f, 0.f, -50.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	////walls
	obj.push_back(new GameObject(rm->get_Models("quad2.obj", gfx), gfx, vec3(0.f, 5.f, 20.f),  vec3(-PI/2, -PI/2, PI), vec3(20.f, 20.f, 20.f)));
	//obj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj", gfx), gfx, vec3(100.f, 5.f, 100.f),  vec3(0, 0.f, 0),   vec3(1.f, 1.f, 1.f)));
	//obj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj", gfx), gfx, vec3(-100.f, 5.f, -100.f), vec3(0, 0, 0),  vec3(1.f, 1.f, 1.f)));
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
	
	//stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(0,0,0), vec3(0, 0, 0), vec3(1, 1, 1)));
	//stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(100,0,100), vec3(0, 0, 0), vec3(1, 1, 1)));
	//stataicObj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(-112,0,-100), vec3(0, 0, 0), vec3(1, 1, 1)));
	float gw = 10;
	float gn = 8;
	for (int x = 0; x < gn; x++) {
		for (int y = 0; y < gn; y++) {
			stataicObj.push_back(new GameObject(rm->get_Models("quad2.obj", gfx), gfx, vec3(x*(gw*2) - ((gn)*gw), -4, y*(gw * 2) - ((gn)*gw)), vec3(0, 0, 1.57f), vec3(10, 10, 10)));
		}
	}
}

void Game::setUpLights()
{
	//current max number is set in graphics.cpp and transforms.hlsli
	nrOfLight = 2;
	light = new Light * [nrOfLight];

	//create the lights with 
	light[0] = new DirLight(vec3(0, 30, 8), vec3(0.1f, -PI / 2, 1.f));
	light[1] = new SpotLight(vec3(18, 46, 45), vec3(-2.4f, -0.5, 1));
	//light[2] = new SpotLight(vec3(8, 47.f, 0), vec3(0, -1, 1));
	//light[3] = new SpotLight(vec3(30, 50, 0), vec3(-1, -1, 1));

	//set color for lights (deafault white)
	light[0]->getColor() = vec3(1, 0, 1);
	//light[1]->getColor() = vec3(1, 0, 1);

	//say to graphics/shaders how many lights we have
	gfx->getLightconstbufferforCS()->nrOfLights.element = nrOfLight;
}

void Game::setUpParticles()
{
	//add the billboards here
	billboardGroups.push_back(new BillBoardGroup(gfx, rm->getFire(), 10, vec3(0, 0, 0), vec3(5, 5, 5)));

	//if billboard have animation add it here
	billboardGroups[0]->setAnimation(6, 1, 0.16f);
}
