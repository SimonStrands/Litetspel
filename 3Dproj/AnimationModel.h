#pragma once
#include "Model.h"
#include "Bone.h"
#include <map>
#include "Convertions.h"
//isn't used in 3D course
/*
struct AnimVCB : CB {
	struct {
		DirectX::XMMATRIX element;
	}transform;
	struct {
		DirectX::XMMATRIX element;
	}view;
	struct {
		DirectX::XMMATRIX element;
	}projection;
	//for shadow
	struct {
		DirectX::XMMATRIX element;
	}lightView;
	struct {
		DirectX::XMMATRIX element[50];
	}jointTransforms;
};

class AnimationModel : public Model {
public:
	AnimationModel(const std::string filePath, Graphics*& gfx, vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	void getPose(float dt, Bone& skeleton, DirectX::XMMATRIX ParentTransform = DirectX::XMMatrixIdentity());
	void update(float dt);
private:
	AnimVCB animationVCB;
	std::vector<BoneVertex> vertexToBones;
	Bone skeleton;//root
	Animation anim;
	int boneCount;
	DirectX::XMMATRIX inverseTransform;
	Graphics *gfx;

	void updateVertexBuffer();
	bool LoadModel(const std::string& modelfile);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	bool CreateSkeleton(Bone& bone, aiNode* rn, std::unordered_map<std::string, std::pair<int, DirectX::XMMATRIX>> boneInfo);
	void loadAnimation(const aiScene* scene);
	std::pair<UINT, float> getTimeFraction(std::vector<float>& keyframesTime, float& elapsedTime);
};
*/