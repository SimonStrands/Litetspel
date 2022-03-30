#pragma once
#include "BillBoard.h"
#include <vector>
#include "Vec.h"
#include "point.h"

struct CTCB : public CB {
	struct {
		float element;
	}time;
	struct {
		float element;
	}rand;
	struct {
		float element[2];
	}padding;
};

class BillBoardGroup {
public:
	/*max number of particles must be a multiple of 8 else the code is going to to it for you*/
	BillBoardGroup(Graphics*& gfx, ID3D11ShaderResourceView* SRV, int nrOfParticles = 10, vec3 orgin = vec3(0,0,0), vec3 sizeofArea = vec3(0, 0, 0), int maxNumberOfParticles = 0);
	/*
	noaw = nr of Width, 
	noah = nr of height, 
	tb = timebetween, 
	currently only supports x*1 sheets;
	*/
	void setAnimation(int noaw, int noah, float tb);
	virtual ~BillBoardGroup();

	void update(float dt, Graphics*& gfx);
	void updateShader(Graphics*& gfx, vec3 camPos);
	void changeBehavior(ID3D11ComputeShader* cUpdate);
	void changeNumberOfParticles(int nrOf);
	void draw(Graphics*& gfx);
private:
	int nrOfParticles;
private:
	CTCB CompConstBuff;
	vec3 position;
	TileAnimation anim;
	ID3D11ShaderResourceView* SRV;
	std::vector<point> billboards;
	ID3D11ComputeShader* cUpdate;
	ID3D11UnorderedAccessView* billUAV;
	ID3D11Buffer* buff;
	ID3D11Buffer* Gg_pConstantBuffer;
	ID3D11Buffer* Pg_pConstantBuffer;
	ID3D11Buffer* Vg_pConstantBuffer;
	ID3D11Buffer* Cg_pConstantBuffer;
};