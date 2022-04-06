#pragma once
#include "UISymbols.h"
#include <vector>
class UIString {
public:
	std::vector<UISymbols> symbols;
private:
	void draw();
};