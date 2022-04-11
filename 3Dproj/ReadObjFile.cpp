#include "ReadObjFile.h"
#include <DirectXMath.h>
#include "TrashCollector.h"



std::vector<vec3> calcTangent(vertex* vex1, vertex* vex2, vertex* vex3)
{
	vertex vex[3]{ *vex1, *vex2, *vex3 };
	//get all vars we need
	DirectX::XMFLOAT3 e[2];
	DirectX::XMFLOAT2 deltaUV[2];
	vec3 v[3];
	vec2 uv[3];
#pragma region addVAndUV
	arrayToVec(vex[0].pos, v[0]);
	arrayToVec(vex[1].pos, v[1]);
	arrayToVec(vex[2].pos, v[2]);
	arrayToVec(vex[0].uv, uv[0]);
	arrayToVec(vex[1].uv, uv[1]);
	arrayToVec(vex[2].uv, uv[2]);
#pragma endregion addVAndUV

	vec3 dae = v[1] - v[0];
	e[0] = DirectX::XMFLOAT3(dae.x, dae.y, dae.z);
	dae = (v[2] - v[0]);
	e[1] = DirectX::XMFLOAT3(dae.x, dae.y, dae.z);
	//x = u    y = v
	deltaUV[0] = DirectX::XMFLOAT2(uv[1].x - uv[0].x, uv[1].y - uv[0].y);
	deltaUV[1] = DirectX::XMFLOAT2(uv[2].x - uv[0].x, uv[2].y - uv[0].y);
	DirectX::XMMATRIX a(//the delta
		deltaUV[1].y, -deltaUV[0].y, 0, 0,
		-deltaUV[1].x, deltaUV[0].x, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0);
	DirectX::XMMATRIX b(//the E:s
		e[0].x, e[0].y, e[0].z, 0,
		e[1].x, e[1].y, e[1].z, 0,
		0, 0, 0, 0,
		0, 0, 0, 0);
	float div = (deltaUV[0].x * deltaUV[1].y - deltaUV[0].y * deltaUV[1].x);
	if (div == 0) {
		div = 0.0001f;
	}
	DirectX::XMMATRIX res =
		1 / div
		* a * b;
	DirectX::XMFLOAT4X4 mtheReturn;
	DirectX::XMStoreFloat4x4(&mtheReturn, res);

	std::vector<vec3> theReturn;
	theReturn.push_back(vec3(mtheReturn._11, mtheReturn._12, mtheReturn._13));
	theReturn.push_back(vec3(mtheReturn._21, mtheReturn._22, mtheReturn._23));
	return theReturn;
}

void fixtangent(std::vector<vertex> &vertecies)
{
	for (int i = 0; i < vertecies.size();) {
		std::vector<vec3> tanbi = calcTangent(&vertecies[i], &vertecies[i+1], &vertecies[i+2]);
		tanbi[0].Normalize(); tanbi[1].Normalize();
		for (int p = 0; p < 3; p++) {
			vertecies[i + p].fixtang(tanbi[0], tanbi[1]);
		}
		i += 3;
	}
}

bool getMatrialFromFileObj(std::string fileName, std::vector<Material*> &matrial, Graphics*& gfx, ID3D11ShaderResourceView** def)
{
	std::ifstream infile(fileName);
	std::string readWord;
	std::string trash;
	std::string mtlname;
	std::string mapName;
	int CTR = -1;
	
	if (!infile.is_open()) {
		printf("cannot open textures");
		return false;
	}
	bool done = false;
	while (std::getline(infile, readWord) && !done) {
		if (readWord.substr(0, 6) == "mtllib") {
			std::istringstream a;
			a.str(readWord);
			a >> trash >> mtlname;
			done = true;
		}
	}
	infile.close();
	infile.open("obj/" + mtlname);
	if (done && infile.is_open()) {
 		while (std::getline(infile, readWord)) {
			if (readWord.substr(0, 6) == "newmtl") {
				CTR++;
				matrial.resize(CTR + 1, new Material(def));
				std::istringstream a;
				std::string b;
				a.str(readWord);
				a >> trash >> matrial[CTR]->name;
			}
			else if (readWord.substr(0, 2) == "Ns") {
				//do something
				std::istringstream a;
				std::string b;
				a.str(readWord);
				a >> trash >> matrial[CTR]->Ns;
			}
			else if (readWord.substr(0, 2) == "Ka") {
				//ambient
				std::istringstream a;
				a.str(readWord);
				a >> trash >> matrial[CTR]->Ka[0] >> matrial[CTR]->Ka[1] >> matrial[CTR]->Ka[2];
			}
			else if (readWord.substr(0, 2) == "Kd") {
				//deffuse
				std::istringstream a;
				a.str(readWord);
				a >> trash >> matrial[CTR]->Kd[0] >> matrial[CTR]->Kd[1] >> matrial[CTR]->Kd[2];
			}
			else if (readWord.substr(0, 2) == "Ks") {
				//deffuse
				std::istringstream a;
				a.str(readWord);
				a >> trash >>  matrial[CTR]->Ks[0] >> matrial[CTR]->Ks[1] >> matrial[CTR]->Ks[2];
			}
			else if (readWord.substr(0, 6) == "map_Ka") {
				//map_ambient
				std::istringstream a;
				a.str(readWord);
				a >> trash >> mapName;
				matrial[CTR]->loadTexture(mapName, gfx, 1, def);
			}
			else if (readWord.substr(0, 6) == "map_Kd") {
				//map_diffuse
				std::istringstream a;
				a.str(readWord);
				a >> trash >> mapName;
				matrial[CTR]->loadTexture(mapName, gfx, 0, def);
			}
			else if (readWord.substr(0, 6) == "map_Ks") {
				//map_Specular
				std::istringstream a;
				a.str(readWord);
				a >> trash >> mapName;
				matrial[CTR]->loadTexture(mapName, gfx, 2, def);
			}
			else if (readWord.substr(0, 8) == "map_Bump") {
				//map_normal
				std::istringstream a;
				a.str(readWord);
				std::string b;
				bool done = false;
				while (a >> b && !done) {
					if (!(b.substr(0, 2) == "-bm" || b.substr(0, 8) == "map_Bump" || int(b[0]) < 58)) {
						done = true;
						matrial[CTR]->loadTexture(b, gfx, 3, def);
					}
				}
			}
		}
	}
	else {
		//we didnt get what we wanted
		if (matrial.size() == 0) {
			matrial.push_back(new Material(def));
		}
		return false;
	}
	if (matrial.size() == 0) {
		matrial.push_back(new Material(def));
	}
	return true;
}

bool getMatrialFromFile(std::string fileName, std::vector<Material*>& matrial, Graphics*& gfx, ID3D11ShaderResourceView** def)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName,
		aiProcess_JoinIdenticalVertices
	);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		return false;
	}
	bool oneTex = false;
	matrial.resize(scene->mNumMaterials);
	for (int i = 0; i < scene->mNumMaterials; i++) {
		matrial[i] = new Material(def);
		const aiMaterial* pMaterial = scene->mMaterials[i];
		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;
			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string thePath(Path.data);
				matrial[i]->loadTexture(thePath, gfx, 0, def);
			}
		}
		if (pMaterial->GetTextureCount(aiTextureType_NORMALS) > 0) {
			aiString Path;
			if (pMaterial->GetTexture(aiTextureType_NORMALS, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string thePath(Path.data);
				matrial[i]->loadTexture(thePath, gfx, 3, def);
			}
		}
		//load kd, ka, ks, ns
		aiColor3D kd,ka,ks;
		float ns;
		pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ka);
		pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, ks);
		pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, kd);
		pMaterial->Get(AI_MATKEY_SHININESS, ns);
		
		matrial[i]->Ka[0] = ka.r;
		matrial[i]->Ka[1] = ka.g;
		matrial[i]->Ka[2] = ka.b;
		matrial[i]->Kd[0] = kd.r;
		matrial[i]->Kd[1] = kd.g;
		matrial[i]->Kd[2] = kd.b;
		matrial[i]->Ks[0] = ks.r;
		matrial[i]->Ks[1] = ks.g;
		matrial[i]->Ks[2] = ks.b;
		matrial[i]->Ns = ns;

	}
	return true;
}

void createMesh(Graphics*& gfx, std::vector<MeshObj> &Meshes, std::vector<vertex> &vertecies, std::vector<DWORD> indecies, Material* matrial) {
	//fixtangent(vertecies);// if use assimp don't have this else have this!
	MeshObj a(gfx, vertecies, indecies, matrial);
	Meshes.push_back(a);
	if (matrial->flags.Maps[3]) {
		Meshes[Meshes.size() - 1].SetShaders(gfx->getVS()[0], gfx->getPS()[0]);
	}
	else {
		Meshes[Meshes.size() - 1].SetShaders(gfx->getVS()[0], gfx->getPS()[2]);
	}
	vertecies.clear();
}

void readFace(std::string readWord, std::vector<vertex> &vertecies, std::vector<std::array<float, 3>> vPos, std::vector<std::array<float, 2>> vUv, std::vector<std::array<float, 4>> vNorm) {
	std::string* sTemp;
	std::string sTemp2[4];
	std::string trash;
		std::istringstream a;
		a.str(readWord);
		a >> trash >> sTemp2[0] >> sTemp2[1] >> sTemp2[2] >> sTemp2[3];
		if (sTemp2[3] != "") {
			for (int i = 0; i < 3; i++) {
				//nrOfVertexes++;
				sTemp = getDest(sTemp2[i]);
				//när jag läser in faces så får dem första sex alltid samma p.g.a det är så dem har skrivit det på obj filen
				vertecies.push_back(vertex(vPos[std::stoi(sTemp[0]) - 1], vUv[std::stoi(sTemp[1]) - 1], vNorm[std::stoi(sTemp[2]) - 1]));
				delete[] sTemp;
			}
			sTemp = getDest(sTemp2[0]);
			vertecies.push_back(vertex(vPos[std::stoi(sTemp[0]) - 1], vUv[std::stoi(sTemp[1]) - 1], vNorm[std::stoi(sTemp[2]) - 1]));
			delete[] sTemp;
			sTemp = getDest(sTemp2[2]);
			vertecies.push_back(vertex(vPos[std::stoi(sTemp[0]) - 1], vUv[std::stoi(sTemp[1]) - 1], vNorm[std::stoi(sTemp[2]) - 1]));
			delete[] sTemp;
			sTemp = getDest(sTemp2[3]);
			vertecies.push_back(vertex(vPos[std::stoi(sTemp[0]) - 1], vUv[std::stoi(sTemp[1]) - 1], vNorm[std::stoi(sTemp[2]) - 1]));
			delete[] sTemp;
		}
		else {
			for (int i = 0; i < 3; i++) {
				sTemp = getDest(sTemp2[i]);
				vertecies.push_back(vertex(vPos[std::stoi(sTemp[0]) - 1], vUv[std::stoi(sTemp[1]) - 1], vNorm[std::stoi(sTemp[2]) - 1]));
				delete[] sTemp;
			}
		}
}

void getLowest(vec3 box[2], float vPos[3])
{
	//get the lowest
	if (box[0].x < vPos[0]) {
		box[0].x = vPos[0];
	}
	if (box[0].y < vPos[1]) {
		box[0].y = vPos[1];
	}
	if (box[0].z < vPos[2]) {
		box[0].z = vPos[2];
	}
	//get highest
	if (box[1].x > vPos[0]) {
		box[1].x = vPos[0];
	}
	if (box[1].y > vPos[1]) {
		box[1].y = vPos[1];
	}
	if (box[1].z > vPos[2]) {
		box[1].z = vPos[2];
	}
}

bool readObjFile(std::vector<MeshObj>& Meshes, std::string fileName, std::vector<Material*>& matrial, Graphics*& gfx, vec3 box[2])
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate
		| aiProcess_ConvertToLeftHanded
		| aiProcess_GenNormals
		| aiProcess_CalcTangentSpace
		| aiProcess_JoinIdenticalVertices
	);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		return false;
	}
	/*This is for boneanimaiton (not added yet)
	uint boneCount = 0;
	Animation animation;
	uint vao = 0;
	Bone skeleton;
	uint diffuseTexture;
	*/

	//make a for loop here
	for (UINT i = 0; i < scene->mNumMeshes; i++) {
		std::vector<DWORD> indecies;
		std::vector<vertex> vertecies;
		aiMesh* mesh = scene->mMeshes[i];

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			vertex v;
			v.pos[0] = mesh->mVertices[i].x;
			v.pos[1] = mesh->mVertices[i].y;
			v.pos[2] = mesh->mVertices[i].z;
			getLowest(box, v.pos);

			v.norm[0] = mesh->mNormals[i].x;
			v.norm[1] = mesh->mNormals[i].y;
			v.norm[2] = mesh->mNormals[i].z;

			v.uv[0] = mesh->mTextureCoords[0][i].x;
			v.uv[1] = mesh->mTextureCoords[0][i].y;

			v.bitang[0] = mesh->mBitangents[i].x;
			v.bitang[1] = mesh->mBitangents[i].y;
			v.bitang[2] = mesh->mBitangents[i].z;

			v.tang[0] = mesh->mTangents[i].x;
			v.tang[1] = mesh->mTangents[i].y;
			v.tang[2] = mesh->mTangents[i].z;

			vertecies.push_back(v);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace& face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indecies.push_back(face.mIndices[j]);
		}
		if (mesh->mMaterialIndex == 0) {
			mesh->mMaterialIndex = 1;
		}
		createMesh(gfx, Meshes, vertecies, indecies, matrial[mesh->mMaterialIndex - 1]);
	}

	return true;
}

