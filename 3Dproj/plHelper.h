#pragma once
#include <d3d11.h>
#include "vertex.h"
#include <string>

//git
bool SetupPipeline(ID3D11Device* device, ID3D11VertexShader**& vShader,
	ID3D11PixelShader**& pShader, ID3D11GeometryShader**& gShader,
	ID3D11HullShader**& hShader, ID3D11DomainShader**& dShader,
	ID3D11InputLayout**& inputLayout,
	ID3D11Texture2D*& tex, ID3D11SamplerState*& sampler);


bool loadVShader(std::string name, ID3D11Device* device, ID3D11VertexShader*& vShader, std::string& vShaderByteCode);
bool loadGShader(std::string name, ID3D11Device* device, ID3D11GeometryShader*& gShader);
bool loadPShader(std::string name, ID3D11Device* device, ID3D11PixelShader*& pShader);
bool loadCShader(std::string name, ID3D11Device* device, ID3D11ComputeShader*& cShader);
bool loadHShader(std::string name, ID3D11Device* device, ID3D11HullShader*& hShader);
bool loadDShader(std::string name, ID3D11Device* device, ID3D11DomainShader*& dShader);
bool CreateTexture(std::string file, ID3D11Device* device, ID3D11Texture2D*& tex, ID3D11ShaderResourceView*& texSRV);
bool CreateInputLayoutOwn(ID3D11Device* device, ID3D11InputLayout*& inputLayout, std::string& VbyteCode, D3D11_INPUT_ELEMENT_DESC inputDesc[], int nrOfElements);
bool CreateTextureCube(std::string files[], ID3D11Device* device, ID3D11Texture2D*& tex, ID3D11ShaderResourceView*& texSRV, UINT wh);