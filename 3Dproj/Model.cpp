#include "Model.h"
#include "ReadObjFile.h"

ModelObj::ModelObj(const std::string& ModelFile, Graphics*& gfx, ID3D11ShaderResourceView** def)
{
	getMatrialFromFileObj(ModelFile, matrial, gfx, def);
	if (!readObjFile(mMeshes, ModelFile, matrial, gfx, boxSize)) {
		std::cout << "couldn't load " << ModelFile << std::endl;
		MessageBox(nullptr, L"sorry couldn't load file look in terminal for more info", L"ERROR", MB_ICONWARNING | MB_OK);
	}
}

ModelObj::ModelObj()
{
}

void ModelObj::init(const std::string& ModelFile, Graphics*& gfx, ID3D11ShaderResourceView** def)
{
	std::cout << ModelFile.substr(ModelFile.length() - 3,3) << std::endl;
	if (ModelFile.substr(ModelFile.length() - 3, 3) == "obj") {
		getMatrialFromFileObj(ModelFile, matrial, gfx, def);
	}
	else {
		getMatrialFromFile(ModelFile, matrial, gfx, def);
	}
	if (!readObjFile(mMeshes, ModelFile, matrial, gfx, boxSize)) {
		std::cout << "couldn't load " << ModelFile << std::endl;
		MessageBox(nullptr, L"sorry couldn't load file look in terminal for more info", L"ERROR", MB_ICONWARNING | MB_OK);
	}
}

ModelObj::~ModelObj()
{
	for (int i = 0; i < mMeshes.size(); i++) {
		mMeshes[i].begone();
	}
}

void ModelObj::draw(Graphics*& gfx, bool shadowmap)
{
	for (int i = 0; i < mMeshes.size(); i++) {
		if (!shadowmap) {
			this->mMeshes[i].SetShader(gfx->get_IMctx());
			gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
		else{
			gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			gfx->get_IMctx()->IASetInputLayout(gfx->getInputLayout()[1]);
			gfx->get_IMctx()->VSSetShader(gfx->getVS()[3], nullptr, 0);
		}
		mMeshes[i].draw(gfx->get_IMctx());
	}
}

std::vector<MeshObj> &ModelObj::getMehses()
{
	return this->mMeshes;
}

std::vector<Material*>& ModelObj::getMatrial()
{
	return this->matrial;
}

vec3* ModelObj::getBoundingBox()
{
	return this->boxSize;
}

bool ModelObj::loadModel(const std::string& ModelFile)
{
	return false;
}