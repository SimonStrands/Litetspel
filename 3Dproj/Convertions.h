#pragma once
#include <DirectXMath.h>
#include "Vec.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Quaternion.h"

DirectX::XMMATRIX assimpToDX(aiMatrix4x4& mat);
vec3 assimpToVec(aiVector3D vec);
vec4 assimpQuartToVec4(aiQuaternion quart);
Quaternion assimpQuartToQuart(aiQuaternion quart);
DirectX::XMMATRIX FlipDiagonal(DirectX::XMMATRIX oth);