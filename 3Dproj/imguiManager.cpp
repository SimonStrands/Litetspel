#include "imguiManager.h"

ImguiManager::ImguiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

ImguiManager::~ImguiManager()
{
	ImGui::DestroyContext();
}

void ImguiManager::takeObject(object* obj)
{
	this->obj.push_back(obj);
}

void ImguiManager::takeLight(Light* light)
{
	this->light.push_back(light);
}

void ImguiManager::updateRender(int lightNr)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	for (int i = 0; i < obj.size(); i++) {
		std::string name = "obj " + std::to_string(i);
		if (ImGui::Begin(name.c_str())) {
			ImGui::SliderFloat("Xpos", &obj[i]->getxPos(), 40.0f, -40.0f);
			ImGui::SliderFloat("Ypos", &obj[i]->getyPos(), 40.0f, -40.0f);
			ImGui::SliderFloat("Zpos", &obj[i]->getzPos(), 40.0f, -40.0f);
			ImGui::SliderFloat("XRot", &obj[i]->getxRot(), 20.0f, -20.0f);
			ImGui::SliderFloat("YRot", &obj[i]->getyRot(), 20.0f, -20.0f);
			ImGui::SliderFloat("ZRot", &obj[i]->getzRot(), 20.0f, -20.0f);
		}
		ImGui::End();
	}
	std::string name = "light" + std::to_string(lightNr);
	if (ImGui::Begin(name.c_str())) {
		ImGui::SliderFloat("Xpos", &light[lightNr]->getPos().x, 60.0f, -60.0f);
		ImGui::SliderFloat("Ypos", &light[lightNr]->getPos().y, 60.0f, -60.0f);
		ImGui::SliderFloat("Zpos", &light[lightNr]->getPos().z, 60.0f, -60.0f);
		ImGui::SliderFloat("XRot", &light[lightNr]->getRotation().x, 6.3f, -6.3f);
		ImGui::SliderFloat("YRot", &light[lightNr]->getRotation().y, 6.3f, -6.3f);
		ImGui::SliderFloat("ZRot", &light[lightNr]->getRotation().z, 6.3f, -6.3f);
	}
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

