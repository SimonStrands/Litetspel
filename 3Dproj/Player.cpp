#include "Player.h"

Player::Player(ModelObj* file, Graphics*& gfx, Mouse* mouse, Keyboard* keyboard, vec3 pos, vec3 rot, vec3 scale):
	GameObject(file, gfx, pos, rot, scale)
{
	this->mouse = mouse;
	this->keyboard = keyboard;
	this->cam = cam;
	speed = 12;
}

Player::~Player()
{
}

void Player::update(float dt)
{
	cam->setRotation(this->getRot());
}

void Player::handleEvents(float dt)
{
	//change these to use keyboard
	if (!mouse->getMouseActive()) {
		if (GetKeyState(VK_RIGHT) & 0x8000) {
			this->addRot(vec3(mouse->getSense() * (float)dt, 0, 0));
		}
		if (GetKeyState(VK_LEFT) & 0x8000) {
			this->addRot(vec3(-mouse->getSense() * (float)dt, 0, 0));
		}
		if (GetKeyState(VK_UP) & 0x8000) {
			this->addRot(vec3(0, mouse->getSense() * (float)dt, 0));
		}
		if (GetKeyState(VK_DOWN) & 0x8000) {
			this->addRot(vec3(0, -mouse->getSense() * (float)dt, 0));
		}
	}
	//change values here
	DirectX::XMFLOAT3 translation = DirectX::XMFLOAT3(0, 0, 0);
	if (keyboard->isKeyPressed('W')) {
		translation = DirectX::XMFLOAT3(0, 0, -(float)dt);
	}
	if (keyboard->isKeyPressed('D')) {
		translation = DirectX::XMFLOAT3(0, 0, -(float)dt);
	}
	if (keyboard->isKeyPressed('S')) {
		translation = DirectX::XMFLOAT3(0, 0, -(float)dt);
	}
	if (keyboard->isKeyPressed('A')) {
		translation = DirectX::XMFLOAT3(0, 0, -(float)dt);
	}
	if (keyboard->isKeyPressed(VK_SPACE)) {
		translation = DirectX::XMFLOAT3(0, 0, -(float)dt);
	}
	if (keyboard->isKeyPressed('W')) {
		translation = DirectX::XMFLOAT3(0, 0, -(float)dt);
	}


}

void Player::Translate(float dt, DirectX::XMFLOAT3 translate)
{
	DirectX::XMStoreFloat3(&translate, DirectX::XMVector3Transform(
		DirectX::XMLoadFloat3(&translate),
		DirectX::XMMatrixRotationRollPitchYaw(this->getyRot(), this->getxRot(), 0) *
		DirectX::XMMatrixScaling(1, 1, 1)//this line is not neccessary but I am afraid to break things
	));
	vec2 trans(translate.x, translate.z);
	trans.Normalize();
	this->movePos(vec3(-trans.x * speed * dt, trans.y * speed * dt, 0));
}
