#pragma once
#include "ResourceManager.h"
#include "Graphics.h"
#include <vector>
#include "UIString.h"

class UIManager {
public:
	UIManager(ResourceManager*& rm, Graphics*& gfx);
private:
	//std::vector<UIElements> elements;
	std::vector<UIString> strings;
	ResourceManager* rm;
	Graphics* gfx;
};