#include "Player.h"
#include <cassert>

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(Model* model, uint32_t textureHandler) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandler;
	worldTransform_.Initialize();
}

void Player::Update() {

}

void Player::Draw() {

}