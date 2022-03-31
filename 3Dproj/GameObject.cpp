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


void GameObject::getBoundingBox(DirectX::XMVECTOR theReturn[])
{
	//rotations
	DirectX::XMMATRIX rot(DirectX::XMMatrixRotationRollPitchYaw(this->getRot().x, this->getRot().y, this->getRot().z));

	DirectX::XMVECTOR low =  { model->getBoundingBox()[1].x ,model->getBoundingBox()[1].y ,model->getBoundingBox()[1].z , 1 };
	DirectX::XMVECTOR high = { model->getBoundingBox()[0].x ,model->getBoundingBox()[0].y ,model->getBoundingBox()[0].z , 1 };

	theReturn[0] = DirectX::XMVectorAdd(DirectX::XMVector4Transform(DirectX::XMVectorMultiply(low, this->getScale().toXMvector()), rot), this->getPos().toXMvector());
	theReturn[1] = DirectX::XMVectorAdd(DirectX::XMVector4Transform(DirectX::XMVectorMultiply(high, this->getScale().toXMvector()), rot), this->getPos().toXMvector());
	DirectX::XMVECTOR temp = {fmax(theReturn[1].m128_f32[0],theReturn[0].m128_f32[0]),fmax(theReturn[1].m128_f32[1],theReturn[0].m128_f32[1]),fmax(theReturn[1].m128_f32[2],theReturn[0].m128_f32[2]) };
	DirectX::XMVECTOR temp2 = { std::min(theReturn[1].m128_f32[0],theReturn[0].m128_f32[0]),std::min(theReturn[1].m128_f32[1],theReturn[0].m128_f32[1]),std::min(theReturn[1].m128_f32[2],theReturn[0].m128_f32[2]) };
	theReturn[0] = temp2;
	theReturn[1] = temp;
}

const bool GameObject::isDrawed()
{
	return this->drawed;
}

void GameObject::clearDrawed()
{
	this->drawed = false;
}

void GameObject::Updateshaders(Graphics*& gfx, bool vertex)
{
	if (vertex) {
		this->updateVertexShader(gfx);
	}
}
