#pragma once
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <vector>
#include "object.h"
#include "Light.h"
#include <string>

class ImguiManager {
public:
	ImguiManager();
	~ImguiManager();
	void takeObject(object* obj);
	void takeLight(Light* light);
	void updateRender(int lightNr);
private:
	std::vector<object*> obj;
	std::vector<Light*> light;
};