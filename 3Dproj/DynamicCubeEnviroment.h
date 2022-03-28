#pragma once
#include "GameObject.h"
#include "plHelper.h"
#include "D311Helper.h"
#include "DeferredRendering.h"

//basicly a mirrror
class DynamicCube : public GameObject {
public:
	//basicly a mirrror
	DynamicCube(ModelObj* model, Graphics*& gfx, vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	virtual ~DynamicCube();
	void draw(ID3D11DeviceContext*& immediateContext);
	ID3D11UnorderedAccessView** getUAVs();
	ID3D11ComputeShader *getCSShader();
	void setRenderTarget(Graphics*& gfx, int i);
	void update(vec3 camPos, Graphics*& gfx);
	void setViewPort(Graphics *& gfx);
	void firstPass();//deffered
	void secondPass(ID3D11ShaderResourceView*& ShadowMapping,
		ID3D11UnorderedAccessView* UAV,
		int dx, int dy,
		ID3D11ComputeShader* CSShader);
	void ClearRenderTarget(Graphics*& gfx);


private:
	bool initCubeMapping(Graphics*& gfx);

	ID3D11ShaderResourceView* CubeResV;
	ID3D11Texture2D* CubeTex;
	ID3D11ComputeShader* CSShader;
	ID3D11DepthStencilView* dsview;
	ID3D11Texture2D* dsTexture;

	ID3D11UnorderedAccessView** UAVs;
	ID3D11RenderTargetView** RTV;

	D3D11_VIEWPORT DviewPort;
	DeferredRendering dCubeDeff;
};