#include "UIManager.h"

UIManager::UIManager(ResourceManager*& rm, Graphics*& gfx)
{
	this->gfx = gfx;
	this->rm = rm;
}
