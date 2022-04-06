#pragma once
#include "ResourceManager.h"
#include "Graphics.h"
#include <vector>
#include "UIString.h"
#include "UISprite.h"
#include <string>

class UIManager {
public:
	UIManager(ResourceManager*& rm, Graphics*& gfx);
	virtual ~UIManager();
	void createUIString(std::string str, vec2 pos, vec2 size);
	void createUISprite(std::string rmsprite, vec2 pos, vec2 size);
private:
	std::vector<UIElements*> elements;
	std::vector<UIString> strings;
	ResourceManager* rm;
	Graphics* gfx;
};