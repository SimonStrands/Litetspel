#include "Mesh.h"
#include <mutex>
#include <thread>
#include "flags.h"

MeshObj::MeshObj(Graphics*& gfx, std::vector<vertex> vertecies, Material *material)
{
	this->nrOfVertexes = (int)vertecies.size();
	this->matrial = material;
	//kanske?
	CreateVertexBuffer(gfx->getDevice(), vertecies, this->vertexBuffer);
	CreateVertexConstBuffer(gfx, this->Pg_pConstantBuffer);
}

MeshObj::MeshObj(Graphics*& gfx, std::vector<vertex> vertecies, std::vector<DWORD> indecies, Material* material)
{
	this->nrOfVertexes = (int)vertecies.size();
	this->nrOfIndecies = (int)indecies.size();
	this->matrial = material;
	//kanske?
	CreateVertexBuffer(gfx->getDevice(), vertecies, this->vertexBuffer);
	CreateVertexBuffer(gfx->getDevice(), indecies, this->indexBuffer, true);
	CreateVertexConstBuffer(gfx, this->Pg_pConstantBuffer);
}

void MeshObj::begone()
{
	if (this->vertexBuffer != nullptr) {
		this->vertexBuffer->Release();
	}
	if (this->indexBuffer != nullptr) {
		this->indexBuffer->Release();
	}
	if (this->Pg_pConstantBuffer != nullptr) {
		this->Pg_pConstantBuffer->Release();
	}
	for (int i = 0; i < SubMeshes.size(); i++) {
		SubMeshes[i].begone();
	}
}

MeshObj::~MeshObj()
{
}

ID3D11Buffer*& MeshObj::getVertexBuffer()
{
	return this->vertexBuffer;
}

ID3D11ShaderResourceView** MeshObj::getTextures()
{
	return this->matrial->texSRVPS;
}

int& MeshObj::getNrOfVertex()
{
	return nrOfVertexes;
}


//ns is in the 4:th element of ks
void MeshObj::getKdKaKsNs(float(&kd)[4], float(&ka)[4], float(&ks)[4])
{
	for (int i = 0; i < 3; i++) {
		kd[i] = this->matrial->Kd[i];
		ka[i] = this->matrial->Ka[i];
		ks[i] = this->matrial->Ks[i];
	}
	ks[3] = matrial->Ns;
	kd[3] = 1.f;
	ka[3] = 1.f;
}

void MeshObj::draw(ID3D11DeviceContext*& immediateContext)
{
	UINT offset = 0;
	static UINT strid = sizeof(vertex);
	if (def_rend) {
		immediateContext->PSSetShaderResources(0, 4, this->matrial->texSRVPS);
	}
	else {
		immediateContext->PSSetShaderResources(0, 1, &this->matrial->texSRVPS[0]);
	}
	
	immediateContext->PSSetConstantBuffers(0, 1, &this->Pg_pConstantBuffer);
	immediateContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &strid, &offset);
	immediateContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	immediateContext->DrawIndexed(nrOfIndecies, 0, 0);

}

void MeshObj::draw2(ID3D11DeviceContext*& immediateContext)
{
	UINT offset = 0;
	static UINT strid = sizeof(vertex);
	SetShader(immediateContext, 0);
	immediateContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &strid, &offset);
	immediateContext->Draw(this->nrOfVertexes, 0);
}



void MeshObj::SetShaders(ID3D11VertexShader* VS)
{
	this->VS = VS;
}

void MeshObj::SetShaders(ID3D11VertexShader* VS, ID3D11PixelShader* PS)
{
	this->VS = VS;
	this->PS = PS;
}


void MeshObj::SetShader(ID3D11DeviceContext*& immediateContext, int flag)
{
	if (flag == 0) {
		immediateContext->VSSetShader(this->VS, nullptr, 0);
		immediateContext->PSSetShader(this->PS, nullptr, 0);
	}
	else if (flag == 1) {
		immediateContext->VSSetShader(this->VS, nullptr, 0);
	}


}

void MeshObj::updatePS(Graphics*& gfx)
{
	getKdKaKsNs(gfx->getPixelconstbuffer()->kd.element, gfx->getPixelconstbuffer()->ka.element, gfx->getPixelconstbuffer()->ks.element);
	D3D11_MAPPED_SUBRESOURCE resource;
	gfx->get_IMctx()->Map(Pg_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, gfx->getPixelconstbuffer(), sizeof(Pcb));
	gfx->get_IMctx()->Unmap(Pg_pConstantBuffer, 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
}

Material* MeshObj::getMatrial()
{
	return matrial;
}