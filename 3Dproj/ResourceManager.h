#pragma once
#include "Model.h"
#include "plHelper.h"
#include <map>
#include <thread>
#include "TrashCollector.h"

struct threadInfo {
	ModelObj* model;
	std::string name;
	ID3D11ShaderResourceView** def;
	Graphics*& gfx;
};

class ResourceManager {
public:
	ResourceManager(Graphics *& gfx);
	~ResourceManager();
	
	void loadThings(Graphics*& gfx);
#pragma region Textures
	ID3D11ShaderResourceView** getDef();
	ID3D11ShaderResourceView* getFire();
	ID3D11ShaderResourceView* getSprite(std::string textureFile, Graphics*& gfx);
#pragma endregion
	Material* getdefMatrial;
	ModelObj* get_Models(std::string key);
	/*can use this method even if the object is not already loded*/
	ModelObj* get_Models(std::string key, Graphics*& gfx);
	
private:
#pragma region Textures
	ID3D11ShaderResourceView** def;//0 = difdef 1 = ndef
	ID3D11ShaderResourceView* Fire;
#pragma endregion

	std::map<std::string, ModelObj*> Models;
	std::map<std::string, ID3D11ShaderResourceView*> Sprites;
	Material* defMatrial;

	void addMaterialToTrashCollector(ModelObj* model);
	void cantLoad(LPCWSTR theerror);
};