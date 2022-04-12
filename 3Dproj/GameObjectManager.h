#pragma once
#include "GameObject.h"
#include <map>
#include "ResourceManager.h"
#include "Light.h"

class GameObjectManager {
public:
	GameObjectManager(Graphics*& gfx, ResourceManager*& rm);
	virtual ~GameObjectManager();
	void addGameObject(GameObject* obj, std::string name);
	void CreateGameObject(std::string modelFile, std::string name = "", vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	void update();
	void updateVertex();
	void updatePixel();
	void draw();
	void drawShadow(Light* lights, int nrOfLighrs);
	GameObject* getGameObject(std::string key);
private:
	Graphics* gfx;
	ResourceManager* rm;
	int modelID;
	std::map<std::string, GameObject*> gameObjects;

};