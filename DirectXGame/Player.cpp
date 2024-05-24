#include "Player.h"
#include <cassert>

Player::Player()
{
}

Player::~Player() {
}

void Player::Initialize(Model* model, uint32_t textureHandler, ViewProjection* viewProjection) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandler;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
}

void Player::Update() {
	worldTransform_.TransferMatrix();
}

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}