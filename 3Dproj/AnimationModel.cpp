#include "AnimationModel.h"
/*
AnimationModel::AnimationModel(const std::string filePath, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale) :
	Model(gfx)
{
	this->gfx = gfx;
	this->changeScale(scale);
	this->changeRot(rot);
	this->changePos(pos);
	if (!CreateConstBuffer(gfx, this->getVertexConstBuffer(), sizeof(AnimVCB), this->animationVCB))
	{
		printf("doesn work");
	};
	if (!CreatePixelConstBuffer(gfx, this->getPixelConstBuffer())) {
		printf("what?");
	};
	this->LoadModel(filePath);
	//this->skeleton.printc();
}

vec3 mix(vec3 x, vec3 y, float a) {
	//taken from GLSL
	vec3 b = x.mul((float)(1.0f - a));
	vec3 c = y.mul(a);
	return b + c;
}

void AnimationModel::getPose(float dt, Bone& skeleton, DirectX::XMMATRIX ParentTransform)
{
	AnimationData& btt = this->anim.boneTransformations[skeleton.name];
	if (dt > anim.duration/10) {
		anim.currentTime = 0;
		dt = 0.0001f;
	}
	std::pair<UINT, float> fp;
	fp = getTimeFraction(btt.PosTime, dt);

	vec3 pos1 = btt.Pos[fp.first - 1];
	vec3 pos2 = btt.Pos[fp.first];

	vec3 pos = mix(pos1, pos2, dt);

	Quaternion rot1 = btt.Rot[fp.first - 1];
	Quaternion rot2 = btt.Rot[fp.first];

	Quaternion rot;
	rot.interpolate(rot1, rot2, dt);

	vec3 scale1 = btt.Scales[fp.first - 1];
	vec3 scale2 = btt.Scales[fp.first];

	vec3 scale = mix(scale1, scale2, dt);

	DirectX::XMMATRIX PositionMatrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		pos.x, pos.y, pos.z, 1.0f
	);
	DirectX::XMMATRIX rotMatrix(DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z));
	DirectX::XMMATRIX ScaleMatrix(
		scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMMATRIX localTransform = PositionMatrix * rotMatrix * ScaleMatrix;
	DirectX::XMMATRIX globalTransform = (ParentTransform * localTransform);
	//output
	this->animationVCB.jointTransforms.element[skeleton.id] = this->inverseTransform * globalTransform * skeleton.offset;

	for (int i = 0; i < skeleton.children.size(); i++) {
		getPose(dt, skeleton.children[i], globalTransform);
	}

}

void AnimationModel::update(float dt)
{
	this->anim.currentTime += dt;
	getPose(this->anim.currentTime, this->skeleton);
	updateVertexBuffer();
}

void AnimationModel::updateVertexBuffer()
{
	gfx->setVView(this->animationVCB.view.element);
	gfx->setVProj(this->animationVCB.projection.element);
	DirectX::XMMATRIX rot(DirectX::XMMatrixRotationRollPitchYaw(this->getRot().x, this->getRot().y, this->getRot().z));

	DirectX::XMMATRIX scal(
		this->getScale().x, 0.0f, 0.0f, 0.0f,
		0.0f, this->getScale().y, 0.0f, 0.0f,
		0.0f, 0.0f, this->getScale().z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	//transformPotion
	DirectX::XMMATRIX trans(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		this->getPos().x, this->getPos().y, this->getPos().z, 1.0f
	);
	DirectX::XMMATRIX point(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		this->getPoint().x, this->getPoint().y, this->getPoint().z, 1.0f
	);
	DirectX::XMMATRIX rts = point * (scal * (rot * trans));

	//vcbd.transform.element = rts;
	animationVCB.transform.element = rts;

	//changing vertex Shader cBuffer
	D3D11_MAPPED_SUBRESOURCE resource;

	gfx->get_IC()->Map(this->getVertexConstBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &this->animationVCB, sizeof(AnimVCB));
	gfx->get_IC()->Unmap(this->getVertexConstBuffer(), 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
}

bool AnimationModel::LoadModel(const std::string& modelfile)
{
	Assimp::Importer importer;


	const aiScene* pScene = importer.ReadFile(modelfile,
		aiProcess_Triangulate
		| aiProcess_ConvertToLeftHanded
		| aiProcess_GenNormals
		| aiProcess_JoinIdenticalVertices
	);

	if (pScene == NULL) {
		return false;
	}
	this->inverseTransform = DirectX::XMMatrixInverse(nullptr, assimpToDX(pScene->mRootNode->mTransformation));
	this->ProcessNode(pScene->mRootNode, pScene);
	loadAnimation(pScene);

	return true;
}
void AnimationModel::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++) {

		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->mMeshes.push_back(this->ProcessMesh(mesh, scene));
	}
	for (UINT i = 0; i < node->mNumChildren; i++) {
		this->ProcessNode(node->mChildren[i], scene);
	}
}

Mesh AnimationModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<DWORD> indices;
	//vertecies
	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		BoneVertex vert;
		vert.pos[0] = mesh->mVertices[i].x;
		vert.pos[1] = mesh->mVertices[i].y;
		vert.pos[2] = mesh->mVertices[i].z;
		vert.norm[0] = mesh->mNormals[i].x;
		vert.norm[1] = mesh->mNormals[i].y;
		vert.norm[2] = mesh->mNormals[i].z;
		//calc and fix tangent
		if (mesh->mTextureCoords[0]) {
			vert.uv[0] = mesh->mTextureCoords[0][i].x;
			vert.uv[1] = mesh->mTextureCoords[0][i].y;
		}
		vertexToBones.push_back(vert);
	}
	//indecies
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (UINT o = 0; o < face.mNumIndices; o++)
		{
			indices.push_back(face.mIndices[o]);
		}
	}
	//hereChange
	//Skeleton and bones
	std::unordered_map<std::string, std::pair<int, DirectX::XMMATRIX>> boneInfo = {};
	this->boneCount = mesh->mNumBones;
	for (UINT i = 0; i < mesh->mNumBones; i++) {
		aiBone* bone = mesh->mBones[i];
		DirectX::XMMATRIX m = assimpToDX(bone->mOffsetMatrix);
		boneInfo[bone->mName.C_Str()] = { i, m };//?
		for (UINT o = 0; o < bone->mNumWeights; o++)
		{
			vertexToBones[bone->mWeights[o].mVertexId].SetBoneVertex(bone->mWeights[o].mVertexId, bone->mWeights[o].mWeight);
		}
	}
	//maybe gone
	for (int i = 0; i < vertexToBones.size(); i++) {
		vertexToBones[i].normalizeWeight();
	}

	CreateSkeleton(skeleton, scene->mRootNode, boneInfo);

	return Mesh(gfx, vertexToBones, indices);
}

bool AnimationModel::CreateSkeleton(Bone& bone, aiNode* rn, std::unordered_map<std::string, std::pair<int, DirectX::XMMATRIX>> boneInfo)
{
	if(boneInfo.find(rn->mName.C_Str()) != boneInfo.end()){
		std::string name = rn->mName.C_Str();
		bone.name = name;
		bone.id = boneInfo[name].first;
		bone.offset = boneInfo[name].second;
		for (UINT i = 0; i < rn->mNumChildren; i++) {
			Bone child;
			if (CreateSkeleton(child, rn->mChildren[i], boneInfo)) {
				bone.children.push_back(child);
			}
		}
		return true;
	}
	else {
		for (UINT i = 0; i < rn->mNumChildren; i++) {
			if (CreateSkeleton(bone, rn->mChildren[i], boneInfo)) {
				return true;
			}
		}
	}
	return false;
}

void AnimationModel::loadAnimation(const aiScene* scene)
{
	this->anim.boneTransformations = {};
	aiAnimation* animation = scene->mAnimations[0];
	if (animation->mTicksPerSecond != 0.0f) {
		this->anim.tps = (float)animation->mTicksPerSecond;
	}
	else { this->anim.tps = 1.0f; }
	anim.duration = (float)(animation->mDuration * animation->mTicksPerSecond);
	
	for (UINT i = 0; i < animation->mNumChannels; i++) {
		aiNodeAnim* channel = animation->mChannels[i];
		AnimationData animData;
		for (UINT o = 0; o < channel->mNumPositionKeys; o++) {
			animData.PosTime.push_back((float)channel->mPositionKeys[o].mTime);
			animData.Pos.push_back(assimpToVec(channel->mPositionKeys[o].mValue));
		}
		for (UINT o = 0; o < channel->mNumRotationKeys; o++) {
			animData.RotTime.push_back((float)channel->mRotationKeys[o].mTime);
			animData.Rot.push_back(assimpQuartToQuart(channel->mRotationKeys[o].mValue));
			
		}
		for (UINT o = 0; o < channel->mNumScalingKeys; o++) {
			animData.ScaleTime.push_back((float)channel->mPositionKeys[o].mTime);
			animData.Scales.push_back(assimpToVec(channel->mPositionKeys[o].mValue));
		}
		anim.boneTransformations[channel->mNodeName.C_Str()] = animData;
	}
}

std::pair<UINT, float> AnimationModel::getTimeFraction(std::vector<float>& keyframesTime, float& elapsedTime)
{
	UINT segment = 0;
	while (elapsedTime >= keyframesTime[segment]/10) {
		segment++;
	}
	std::cout << segment << std::endl;
	float start = keyframesTime[segment - 1];
	float end = keyframesTime[segment];
	float frac = (elapsedTime - start) / (end - start);
	return {segment, frac};
}
*/