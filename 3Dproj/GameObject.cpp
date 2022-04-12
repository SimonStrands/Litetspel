#include "GameObject.h"

GameObject::GameObject(ModelObj*file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale, std::string otherTex)
{
	this->setPos(pos);
	this->setScale(scale);
	this->setRot(rot);
	this->model = file;
	CreateVertexConstBuffer(gfx, this->getVertexConstBuffer());
	object::setModel(model);
	drawed = false;
	this->setHeightWidthDepth();
}

GameObject::~GameObject()
{
	
}

void GameObject::draw(Graphics *&gfx, bool sm)
{
	drawed = true;
	gfx->get_IMctx()->VSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	model->draw(gfx, sm);
}

vec3 GameObject::getlastPosition()
{
	return this->_lastPosition;
}


void GameObject::getBoundingBox(DirectX::XMVECTOR theReturn[])
{
	//rotations
	DirectX::XMMATRIX rot(DirectX::XMMatrixRotationRollPitchYaw(this->getRot().x, this->getRot().y, this->getRot().z));

	vec3 high(model->getBoundingBox()[1].x, model->getBoundingBox()[1].y, model->getBoundingBox()[1].z);
	vec3 low(model->getBoundingBox()[0].x, model->getBoundingBox()[0].y, model->getBoundingBox()[0].z);

	DirectX::XMMATRIX modelMatrix = (scaleMat * rot * transMat);

	DirectX::XMVECTOR bbPoints[8] = {
		{high.x,high.y,high.z,1},
		{high.x,high.y,low.z,1},
		{high.x,low.y,high.z,1},
		{high.x,low.y,low.z,1},
		{low.x,high.y,high.z,1},
		{low.x,high.y,low.z,1},
		{low.x,low.y,high.z,1},
		{low.x,low.y,low.z,1}
	};
	
	bbPoints[0] = DirectX::XMVector4Transform(bbPoints[0], modelMatrix);
	theReturn[0] = bbPoints[0];
	theReturn[1] = bbPoints[0];

	for (int i = 1; i < 8; i++) {
		bbPoints[i] = DirectX::XMVector4Transform(bbPoints[i], modelMatrix);
		if (bbPoints[i].m128_f32[0] > theReturn[1].m128_f32[0]) {
			theReturn[1].m128_f32[0] = bbPoints[i].m128_f32[0];
		}
		if (bbPoints[i].m128_f32[1] > theReturn[1].m128_f32[1]) {
			theReturn[1].m128_f32[1] = bbPoints[i].m128_f32[1];
		}
		if (bbPoints[i].m128_f32[2] > theReturn[1].m128_f32[2]) {
			theReturn[1].m128_f32[2] = bbPoints[i].m128_f32[2];
		}

		if (bbPoints[i].m128_f32[0] < theReturn[0].m128_f32[0]) {
			theReturn[0].m128_f32[0] = bbPoints[i].m128_f32[0];
		}
		if (bbPoints[i].m128_f32[1] < theReturn[0].m128_f32[1]) {
			theReturn[0].m128_f32[1] = bbPoints[i].m128_f32[1];
		}
		if (bbPoints[i].m128_f32[2] < theReturn[0].m128_f32[2]) {
			theReturn[0].m128_f32[2] = bbPoints[i].m128_f32[2];
		}
	}
}

DirectX::BoundingBox GameObject::getDirectXBoundingBoxFromModel()
{
	//rotations
	DirectX::XMMATRIX rot(DirectX::XMMatrixRotationRollPitchYaw(this->getRot().x, this->getRot().y, this->getRot().z));

	DirectX::XMVECTOR low = { model->getBoundingBox()[1].x ,model->getBoundingBox()[1].y ,model->getBoundingBox()[1].z , 1 };
	DirectX::XMVECTOR high = { model->getBoundingBox()[0].x ,model->getBoundingBox()[0].y ,model->getBoundingBox()[0].z , 1 };

	//datemp[0] = DirectX::XMVectorAdd(DirectX::XMVector4Transform(DirectX::XMVectorMultiply(low, this->getScale().toXMvector()), rot), this->getPos().toXMvector());
	//datemp[1] = DirectX::XMVectorAdd(DirectX::XMVector4Transform(DirectX::XMVectorMultiply(high, this->getScale().toXMvector()), rot), this->getPos().toXMvector());
	//
	//datemp[1] = { fmax(datemp[1].m128_f32[0],datemp[0].m128_f32[0]),fmax(datemp[1].m128_f32[1],datemp[0].m128_f32[1]),fmax(datemp[1].m128_f32[2],datemp[0].m128_f32[2]) };
	//datemp[0] = { std::min(datemp[1].m128_f32[0],datemp[0].m128_f32[0]),std::min(datemp[1].m128_f32[1],datemp[0].m128_f32[1]),std::min(datemp[1].m128_f32[2],datemp[0].m128_f32[2]) };
	vec3 themid = this->getPos() + model->getBoundingBox()[0];
	DirectX::XMFLOAT3 midpoint(
		themid.toXMFloat3()
		);//mid pos
	DirectX::XMFLOAT3 sizes(
		model->getBoundingBox()[1].toXMFloat3()
	);//sizes pos
	return DirectX::BoundingBox(midpoint, sizes);
}

DirectX::BoundingBox GameObject::getDirectXBoundingBoxFromObject()
{
	DirectX::XMFLOAT3 newpos(this->BBpos.x + getPos().toXMFloat3().x, 
		this->BBpos.y + getPos().toXMFloat3().y, 
		this->BBpos.z + getPos().toXMFloat3().z);
	return DirectX::BoundingBox(newpos, BBsizes);
}

void GameObject::setBoundingBox(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 sizes)
{
	this->BBpos = pos;
	this->BBsizes = sizes;
}

const bool GameObject::isDrawed()
{
	return this->drawed;
}

void GameObject::clearDrawed()
{
	this->drawed = false;
}

vec3 GameObject::getWidthHeightDepth()
{
	return this->WHD;
}

void GameObject::setHeightWidthDepth()
{
	DirectX::XMVECTOR theReturn[2];
	//rotations
	getBoundingBox(theReturn);

	WHD.x = theReturn[1].m128_f32[0] - theReturn[0].m128_f32[0];
	WHD.y = theReturn[1].m128_f32[1] - theReturn[0].m128_f32[1];
	WHD.z = theReturn[1].m128_f32[2] - theReturn[0].m128_f32[2];

}

void GameObject::Updateshaders(Graphics*& gfx, bool vertex)
{
	if (vertex) {
		this->updateVertexShader(gfx);
	}
}
