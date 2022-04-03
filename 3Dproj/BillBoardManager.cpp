#include "BillBoardManager.h"
#include "Random.h"
//set position later

BillBoardGroup::BillBoardGroup(Graphics*& gfx, ID3D11ShaderResourceView* SRV, int nrOfParticles, vec3 orgin, vec3 sizeofArea, int maxNumberOfParticles):
	anim()
{
	this->nrOfParticles = nrOfParticles;
	//if its not a multiple of 8 add so it is
	if (nrOfParticles % 8 != 0) {
		nrOfParticles += (8 - (nrOfParticles % 8));
	}
	if (maxNumberOfParticles == 0) {
		maxNumberOfParticles = nrOfParticles;
	}
	srand(time(NULL));
	//load a Computeshader
	loadCShader("BillBoardUpdate.cso", gfx->getDevice(), cUpdate);
	CompConstBuff.time.element = 0;
	for (int i = 0; i < maxNumberOfParticles; i++) {
		billboards.push_back(point(vec3(
			RandomNumber(orgin.x - sizeofArea.x, orgin.x + sizeofArea.x),
			RandomNumber(orgin.y - sizeofArea.y, orgin.y + sizeofArea.y),
			RandomNumber(orgin.z - sizeofArea.z, orgin.z + sizeofArea.z))));
	}
	this->SRV = SRV;

	CreateVertexConstBuffer(gfx, this->Vg_pConstantBuffer);
	CreateGeometryConstBuffer(gfx, this->Gg_pConstantBuffer);
	CreatePixelConstBuffer(gfx, this->Pg_pConstantBuffer);
	CreateConstBuffer(gfx, this->Cg_pConstantBuffer, sizeof(CTCB), &CompConstBuff);

	//create UAV
	D3D11_BUFFER_DESC buffDesc;
	buffDesc.ByteWidth = sizeof(point) * maxNumberOfParticles;
	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS;//how does this bind with vertex Buffer?
	buffDesc.CPUAccessFlags = 0;
	buffDesc.StructureByteStride = 0;
	buffDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = billboards.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	if (FAILED(gfx->getDevice()->CreateBuffer(&buffDesc, &data, &buff))) {
		printf("doesn't work create Buffer");
		return;
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC UavDesc;
	UavDesc.Format = DXGI_FORMAT_R32_FLOAT;
	UavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UavDesc.Buffer.FirstElement = 0;
	UavDesc.Buffer.NumElements = maxNumberOfParticles * 3;
	UavDesc.Buffer.Flags = 0;
	if (FAILED(gfx->getDevice()->CreateUnorderedAccessView(buff, &UavDesc, &billUAV))) {
		printf("doesn't work create Buffer");
		return;
	}
}

void BillBoardGroup::setAnimation(int noaw, int noah, float tb)
{
	this->anim.setAnimation(noaw, noah, tb);
}

BillBoardGroup::~BillBoardGroup()
{
	if (cUpdate != nullptr) {
		cUpdate->Release();
	}
	if (billUAV != nullptr) {
		billUAV->Release();
	}
	if (buff != nullptr) {
		buff->Release();
	}
	if (Gg_pConstantBuffer != nullptr) {
		Gg_pConstantBuffer->Release();
	}
	if (Pg_pConstantBuffer != nullptr) {
		Pg_pConstantBuffer->Release();
	}
	if (Vg_pConstantBuffer != nullptr) {
		Vg_pConstantBuffer->Release();
	}
	if (Cg_pConstantBuffer != nullptr) {
		Cg_pConstantBuffer->Release();
	}
}


void BillBoardGroup::update(float dt, Graphics*& gfx)
{
	this->CompConstBuff.time.element = dt;
	this->CompConstBuff.rand.element = RandomNumber(-1.f, 1.f);
	anim.update(dt);

	//dispathc shit
	gfx->get_IMctx()->CSSetShader(cUpdate, nullptr, 0);

	gfx->get_IMctx()->CSSetConstantBuffers(0, 1, &Cg_pConstantBuffer);

	gfx->get_IMctx()->CSSetUnorderedAccessViews(0, 1, &billUAV, nullptr);

	gfx->get_IMctx()->Dispatch((UINT)this->billboards.size()/8, 1, 1);//calc how many groups we need

	//nulla unorderedaccesview
	ID3D11UnorderedAccessView* nullUAV = nullptr;
	gfx->get_IMctx()->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
}

void BillBoardGroup::updateShader(Graphics*& gfx, vec3 camPos)
{
	//doesn't make a difference
	//DirectX::XMMATRIX rot(DirectX::XMMatrixRotationRollPitchYaw(this->getRot().x, this->getRot().y, this->getRot().z));
	vec3 scale(1, 1, 1);
	DirectX::XMMATRIX scal(
		scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	//transformPotion//this does
	DirectX::XMMATRIX trans(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		position.x, position.y, position.z, 1.0f
	);

	DirectX::XMMATRIX rts =  (scal * trans);

	gfx->getVertexconstbuffer()->transform.element = rts;

	//changing vertex Shader cBuffer
	D3D11_MAPPED_SUBRESOURCE resource;

	gfx->get_IMctx()->Map(Vg_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, gfx->getVertexconstbuffer(), sizeof(Vcb));
	gfx->get_IMctx()->Unmap(Vg_pConstantBuffer, 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//GCB
	gfx->getGeometryconstbuffer()->cameraPos.element[0] = -camPos.x;
	gfx->getGeometryconstbuffer()->cameraPos.element[1] = -camPos.y;
	gfx->getGeometryconstbuffer()->cameraPos.element[2] = -camPos.z;

	//uv
	gfx->getGeometryconstbuffer()->uvCords.element[0] = anim.uv().xyz.x;
	gfx->getGeometryconstbuffer()->uvCords.element[1] = anim.uv().xyz.y;
	gfx->getGeometryconstbuffer()->uvCords.element[2] = anim.uv().xyz.z;
	gfx->getGeometryconstbuffer()->uvCords.element[3] = anim.uv().w;

	gfx->get_IMctx()->Map(Gg_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, gfx->getGeometryconstbuffer(), sizeof(Gcb));
	gfx->get_IMctx()->Unmap(Gg_pConstantBuffer, 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));


	//update computeshader const buffer
	gfx->get_IMctx()->Map(Cg_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &CompConstBuff, sizeof(CTCB));
	gfx->get_IMctx()->Unmap(Cg_pConstantBuffer, 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
}

void BillBoardGroup::changeBehavior(ID3D11ComputeShader* cUpdate)
{
	this->cUpdate->Release();
	this->cUpdate = cUpdate;
}

void BillBoardGroup::changeNumberOfParticles(int nrOf)
{
	if (nrOf <= billboards.size()) {
		this->nrOfParticles = nrOf;
	}
	else {
		std::cout << "Tried to exced the maxnumber of particles set in particle group" << std::endl;
	}
}

void BillBoardGroup::draw(Graphics*& gfx)
{
	UINT offset = 0;
	static UINT strid = sizeof(point);

	gfx->get_IMctx()->IASetInputLayout(gfx->getInputLayout()[1]);
	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	gfx->get_IMctx()->VSSetShader(gfx->getVS()[1], nullptr, 0);
	gfx->get_IMctx()->PSSetShader(gfx->getPS()[1], nullptr, 0);
	gfx->get_IMctx()->GSSetShader(gfx->getGS()[0], nullptr, 0);

	gfx->get_IMctx()->PSSetShaderResources(1, 1, &SRV);

	gfx->get_IMctx()->VSSetConstantBuffers(0, 1, &Vg_pConstantBuffer);
	gfx->get_IMctx()->GSSetConstantBuffers(0, 1, &Gg_pConstantBuffer);
	gfx->get_IMctx()->PSSetConstantBuffers(0, 1, &Pg_pConstantBuffer);
	
	gfx->get_IMctx()->IASetVertexBuffers(0, 1, &this->buff, &strid, &offset);
	gfx->get_IMctx()->Draw(this->nrOfParticles, 0);

}
