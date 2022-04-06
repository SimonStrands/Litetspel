#include "UIManager.h"

UIManager::UIManager(ResourceManager*& rm, Graphics*& gfx)
{
	this->gfx = gfx;
	this->rm = rm;
}

UIManager::~UIManager()
{
	for (int i = 0; i < elements.size(); i++) {
		delete elements[i];
	}
}

void UIManager::createUIString(std::string str, vec2 pos, vec2 size)
{
	strings.push_back(UIString());
}

void UIManager::createUISprite(std::string rmsprite, vec2 pos, vec2 size)
{
	elements.push_back(new UISprite(rm->getSprite(rmsprite, gfx), pos, size));
}

void UIManager::draw()
{
	for (int i = 0; i < elements.size(); i++) {
		//elements[i].draw();
	}
	for (int i = 0; i < strings.size(); i++) {
		strings[i].draw();
	}
}
