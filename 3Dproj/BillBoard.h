#pragma once
#include "object.h"
#include "CreateBuffer.h"
#include "vertex.h"
#include "point.h"
#include "TileAnimation.h"

class Graphics;

class BillBoard : public object {
public:
	
	BillBoard(Graphics *&gfx, vec3 pos, ID3D11ShaderResourceView* texSRV, ID3D11ShaderResourceView* NDef, int nrOfCol = 1, int nrOfRow = 1);
	virtual ~BillBoard();
	void update(float dt);
	void draw(Graphics*& gfx, bool sm = false);
	void getKdKa(float(&kd)[4], float(&ka)[4]);
	ID3D11Buffer* getGCB();
	point points;
	TileAnimation &getTAnim();
	void UpdateShader(Graphics*& gfx, vec3 cameraPos ,bool v = true, bool p = true, bool g = true);
private:
	TileAnimation anim;
	ID3D11Buffer* pointBuffer;
	ID3D11ShaderResourceView* texSRV;
	ID3D11ShaderResourceView* NDef;
	ID3D11Buffer* Gg_pConstantBuffer;
	ID3D11Buffer* Pg_pConstantBuffer;
};