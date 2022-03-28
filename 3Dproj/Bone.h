#pragma once
#include "Vec.h"
#include "vertex.h"
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <unordered_map>
#include "Quaternion.h"
//isn't used in 3D course

struct BoneVertex {
	float pos[3];
	float uv[2];
	float norm[3];
	float tang[3];
	float bitang[3];
	int BoneIDs[4] = { 0,0,0,0 };
	float weights[4] = {0,0,0,0};
	
	BoneVertex() {
		for (int i = 0; i < 4; i++) {
			weights[i] = -1;
			BoneIDs[i] = -1;
		}
	}
	
	void SetBoneVertex(int BoneID, float Weight) {
		if (Weight != 0) {
			bool done = false;
			for (int i = 0; i < 4 && !done; i++) {
				if (weights[i] == -1){ //&& BoneIDs[i] == -1) {
					weights[i] = Weight;
					BoneIDs[i] = BoneID;
					done = true;
				}
			}
		}
	}
	//do I need this???
	void normalizeWeight() {
		float mag = sqrt(weights[0] * weights[0] + weights[1] * weights[1]
					   + weights[2] * weights[2] + weights[3] * weights[3]);
		if (mag > 0) {
			for (int i = 0; i < 4; i++) {
				weights[i] /= mag;
			}
		}

		//float mag = weights[0] + weights[1] + weights[2] + weights[3];
		//if (mag > 0) {
		//	for (int i = 0; i < 4; i++) {
		//		weights[i] /= mag;
		//	}
		//}
	}
};

struct Bone {
	int id;
	std::string name;
	DirectX::XMMATRIX offset = DirectX::XMMATRIX(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	std::vector<Bone> children;
	void printc() {
		std::string out = "name: " + name + " id:" + std::to_string(id) + "\n";
		printf(out.c_str());
		printf("have children");
		for (int i = 0; i < children.size(); i++) {
			children[i].printc();
		}
	}
};

struct AnimationData {
	std::vector<float> PosTime;
	std::vector<float> RotTime;
	std::vector<float> ScaleTime;

	//DirectX::XMMATRIX???
	std::vector<vec3> Pos;
	std::vector<Quaternion> Rot;
	std::vector<vec3> Scales;
};

struct Animation {
	float duration = 0.0f;
	float currentTime = 0.0f;
	float tps = 1.0f;
	std::unordered_map<std::string, AnimationData> boneTransformations;
};