#include "BillBoard.h"
#include "Graphics.h"

BillBoard::BillBoard(Graphics*& gfx, vec3 pos, ID3D11ShaderResourceView* texSRV, ID3D11ShaderResourceView* NDef, int nrOfCol, int nrOfRow) :
	object(pos),
	points(vec3(0,0,0)),
	anim(nrOfCol, nrOfRow)
{
	this->texSRV = texSRV;
	this->NDef = NDef;
	CreateVertexConstBuffer(gfx, this->getVertexConstBuffer());
	CreateGeometryConstBuffer(gfx, Gg_pConstantBuffer);
	CreatePixelConstBuffer(gfx, Pg_pConstantBuffer);
	D3D11_BUFFER_DESC bDesc = {};
	bDesc.ByteWidth = sizeof(points);
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;
	bDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &points;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	 HRESULT hr = gfx->getDevice()->CreateBuffer(&bDesc, &data, &pointBuffer);

	if (FAILED(hr)) {
		printf("failed");
	}
}

BillBoard::~BillBoard()
{
	if (pointBuffer != nullptr) {
		pointBuffer->Release();
	}
	if (Gg_pConstantBuffer != nullptr) {
		Gg_pConstantBuffer->Release();
	}
	if (Pg_pConstantBuffer != nullptr) {
		Pg_pConstantBuffer->Release();
	}
}

void BillBoard::update(float dt)
{
	anim.update(dt);
}

void BillBoard::draw(Graphics*& gfx, bool sm)
{
	gfx->get_IMctx()->PSSetShaderResources(0, 1, &texSRV);
	gfx->get_IMctx()->PSSetShaderResources(1, 1, &texSRV);
	gfx->get_IMctx()->PSSetShaderResources(2, 1, &NDef);
	static UINT strid = sizeof(point);
	static UINT offset = 0;
	gfx->get_IMctx()->VSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	gfx->get_IMctx()->GSSetConstantBuffers(0, 1, &Gg_pConstantBuffer);
	gfx->get_IMctx()->PSSetConstantBuffers(0, 1, &Pg_pConstantBuffer);
	gfx->get_IMctx()->IASetVertexBuffers(0, 1, &pointBuffer, &strid, &offset);
	gfx->get_IMctx()->Draw(1, 0);
}

void BillBoard::getKdKa(float(&kd)[4], float(&ka)[4])
{
	for (int i = 0; i < 3; i++) {
		kd[i] = 0.5;
		ka[i] = 0.5;
	}
	kd[3] = 1.f;
	ka[3] = 1.f;
}

ID3D11Buffer* BillBoard::getGCB()
{
	return this->Gg_pConstantBuffer;
}

TileAnimation& BillBoard::getTAnim()
{
	return this->anim;
}

void BillBoard::UpdateShader(Graphics*& gfx, vec3 cameraPos, bool v, bool p, bool g)
{
	this->updateVertexShader(gfx);

	this->getKdKa(gfx->getPixelconstbuffer()->kd.element, gfx->getPixelconstbuffer()->ka.element);


	//changing pixel shader cBuffer
	D3D11_MAPPED_SUBRESOURCE resource;
	gfx->get_IMctx()->Map(Pg_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, gfx->getPixelconstbuffer(), sizeof(Pcb));
	gfx->get_IMctx()->Unmap(Pg_pConstantBuffer, 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//GCB
	gfx->getGeometryconstbuffer()->cameraPos.element[0] = -cameraPos.x;
	gfx->getGeometryconstbuffer()->cameraPos.element[1] = -cameraPos.y;
	gfx->getGeometryconstbuffer()->cameraPos.element[2] = -cameraPos.z;

	//uv
	gfx->getGeometryconstbuffer()->uvCords.element[0] = this->getTAnim().uv().xyz.x;
	gfx->getGeometryconstbuffer()->uvCords.element[1] = this->getTAnim().uv().xyz.y;
	gfx->getGeometryconstbuffer()->uvCords.element[2] = this->getTAnim().uv().xyz.z;
	gfx->getGeometryconstbuffer()->uvCords.element[3] = this->getTAnim().uv().w;

	gfx->get_IMctx()->Map(this->getGCB(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, gfx->getGeometryconstbuffer(), sizeof(Gcb));
	gfx->get_IMctx()->Unmap(this->getGCB(), 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
}
