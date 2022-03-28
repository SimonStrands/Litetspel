#include "plHelper.h"
#include <wrl.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <dxgidebug.h>
//git
#define STB_IMAGE_IMPLEMENTATION 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//load shader

bool loadVShader(std::string name, ID3D11Device* device, ID3D11VertexShader*& vShader, std::string &vShaderByteCode) {
	std::string shaderData;
	std::ifstream reader;
	reader.open("../x64/Debug/" + name, std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		printf("cannot open vertex file\n");
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());
	HRESULT hr = device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vShader);
	if (FAILED(hr))
	{
		std::cerr << "cannot create vertexShader" << std::endl;
		return false;
	}

	vShaderByteCode = shaderData;
	shaderData.clear();
	reader.close();
	return true;
}
bool loadGShader(std::string name, ID3D11Device* device, ID3D11GeometryShader*& gShader) {
	std::string shaderData;
	std::ifstream reader;
	reader.open("../x64/Debug/" + name, std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "cannot open vertex file" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	if (FAILED(device->CreateGeometryShader(shaderData.c_str(), shaderData.length(), nullptr, &gShader)))
	{
		std::cerr << "cannot create GShader" << std::endl;
		return false;
	}

	shaderData.clear();
	reader.close();
	return true;
}
bool loadPShader(std::string name, ID3D11Device* device, ID3D11PixelShader*& pShader) {
	std::string shaderData;
	std::ifstream reader;
	reader.open("../x64/Debug/" + name, std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "cannot open pixel file" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	if (FAILED(device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &pShader)))
	{
		std::cerr << "cannot create pixelShader" << std::endl;
		return false;
	}
	return true;
}

bool loadCShader(std::string name, ID3D11Device* device, ID3D11ComputeShader*& cShader)
{
	std::string shaderData;
	std::ifstream reader;
	reader.open("../x64/Debug/" + name, std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "cannot open pixel file" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());
	if (FAILED(device->CreateComputeShader(shaderData.c_str(), shaderData.length(), nullptr, &cShader)))
	{
		std::cerr << "cannot create ComputeShader" << std::endl;
		return false;
	}
	return true;
}

bool loadHShader(std::string name, ID3D11Device* device, ID3D11HullShader*& hShader)
{
	std::string shaderData;
	std::ifstream reader;
	reader.open("../x64/Debug/" + name, std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "cannot open pixel file" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());
	if (FAILED(device->CreateHullShader(shaderData.c_str(), shaderData.length(), nullptr, &hShader)))
	{
		std::cerr << "cannot create ComputeShader" << std::endl;
		return false;
	}
	return true;
}

bool loadDShader(std::string name, ID3D11Device* device, ID3D11DomainShader*& dShader)
{
	std::string shaderData;
	std::ifstream reader;
	reader.open("../x64/Debug/" + name, std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "cannot open pixel file" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());
	if (FAILED(device->CreateDomainShader(shaderData.c_str(), shaderData.length(), nullptr, &dShader)))
	{
		std::cerr << "cannot create ComputeShader" << std::endl;
		return false;
	}
	return true;
}

bool CreateInputLayout(ID3D11Device* device, ID3D11InputLayout*& inputLayout, std::string& VbyteCode) 
{
	const int nrOfEl = 5;
	D3D11_INPUT_ELEMENT_DESC inputDesc[nrOfEl] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		
	};

	HRESULT hr = device->CreateInputLayout(inputDesc, nrOfEl, VbyteCode.c_str(), VbyteCode.length(), &inputLayout);
	if (FAILED(hr) || hr != S_OK) {
		printf("input layout 1");
		return false;
	}


	return !FAILED(hr);
} 

bool CreateInputLayoutOwn(ID3D11Device* device, ID3D11InputLayout*& inputLayout, std::string& VbyteCode, D3D11_INPUT_ELEMENT_DESC inputDesc[], int nrOfElements)
{
	HRESULT hr = device->CreateInputLayout(inputDesc, nrOfElements, VbyteCode.c_str(), VbyteCode.length(), &inputLayout);
	if (FAILED(hr) || hr != S_OK) {
		printf("input layout 2");
		return false;
	}

	return !FAILED(hr);
}

bool CreateInputLayoutBill(ID3D11Device* device, ID3D11InputLayout*& inputLayout, std::string& VbyteCode) {
	const int nrOfEl = 1;
	D3D11_INPUT_ELEMENT_DESC inputDesc[nrOfEl] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = device->CreateInputLayout(inputDesc, nrOfEl, VbyteCode.c_str(), VbyteCode.length(), &inputLayout);
	if (FAILED(hr) || hr != S_OK) {
		printf("input layout 2");
		return false;
	}


	return !FAILED(hr);
}

bool CreateTexture(std::string file, ID3D11Device* device, ID3D11Texture2D*& tex, ID3D11ShaderResourceView*& texSRV) 
{
	if (file == "") {
		return false;
	}
	int textureWidth;
	int textureHeight;
	int channels;

	unsigned char * textureData = stbi_load(file.c_str(), &textureWidth, &textureHeight, &channels, 4);

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = textureWidth;
	desc.Height = textureHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = (void*)textureData;
	data.SysMemPitch = textureWidth * 4;
	data.SysMemSlicePitch = textureWidth * textureHeight * 4;

	if (FAILED(device->CreateTexture2D(&desc, &data, &tex))) {
		printf("cannot create texture");
		return false;
	}
	HRESULT hr = device->CreateShaderResourceView(tex, nullptr, &texSRV);
	delete[] textureData;
	tex->Release();

	return !FAILED(hr);
}

bool CreateSamplerState(ID3D11Device* device, ID3D11SamplerState*& sampler) 
{
	D3D11_SAMPLER_DESC desc;
	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = 0;
	desc.MaxAnisotropy = 16;
	desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 0;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = device->CreateSamplerState(&desc, &sampler);
	return !FAILED(hr);
}

bool SetupPipeline(ID3D11Device* device, ID3D11VertexShader**& vShader,
	ID3D11PixelShader**& pShader, ID3D11GeometryShader**& gShader,
	ID3D11HullShader**& hShader, ID3D11DomainShader**& dShader,
	ID3D11InputLayout**& inputLayout,
	ID3D11Texture2D*& tex, ID3D11SamplerState*& sampler)
{
	std::string vShaderByteCode[4];
#pragma region shaderloading
	if (loadVShader("VertexShader.cso", device, vShader[0], vShaderByteCode[0]) &&
		loadVShader("VertexBillBoard.cso", device, vShader[1], vShaderByteCode[1]) &&
		loadVShader("VertexDisplaysment.cso",device, vShader[2], vShaderByteCode[2])&&
		loadVShader("VertexShadow.cso",device, vShader[3], vShaderByteCode[2])&&
		loadGShader("GeometryShader.cso", device, gShader[0]) &&
		loadGShader("Debugging_test.cso", device, gShader[1]) &&
		loadPShader("PSSHNormal.cso", device, pShader[0]) && 
		loadPShader("PixelBillShader.cso", device, pShader[1])&&
		loadPShader("PSSH.cso", device, pShader[2])&&
		loadPShader("DynamicCubicPS.cso", device, pShader[3])&&
		loadHShader("HullDisplaysment.cso", device, hShader[0])&&
		loadHShader("PhongTessHull.cso", device, hShader[1])&&
		loadDShader("DomainDisplaysment.cso", device, dShader[0])&&
		loadDShader("PhongTessDomain.cso", device, dShader[1]))
	{
		//continoue
		
	}
	else {
		std::cerr << "cant load shaders" << std::endl;
		return false;
	}
#pragma endregion

	if (!CreateInputLayout(device, inputLayout[0], vShaderByteCode[0]))
	{
		std::cerr << "cant load inputlayout" << std::endl;
		return false;
	}
	if (!CreateInputLayoutBill(device, inputLayout[1], vShaderByteCode[1]))
	{
		std::cerr << "cant load inputlayout2" << std::endl;
		return false;
	}
	if (!CreateSamplerState(device, sampler))
	{
		std::cerr << "cant load sampler" << std::endl;
		return false;
	}

	return true;
}



