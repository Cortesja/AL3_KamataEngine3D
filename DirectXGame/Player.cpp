#include "Player.h"
#include <cassert>
#include <numbers>
#include <algorithm>
#include "DirectXCommon.h"
#include "Input.h"

Player::Player()
{
}

Player::~Player() {
}

void Player::Initialize(Model* model, ViewProjection *viewProjection, const Vector3& position, uint32_t playerTextureHandler) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	textureHandle_ = playerTextureHandler;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {
	worldTransform_.TransferMatrix();
	bool inputRight = Input::GetInstance()->PushKey(DIK_RIGHT);
	bool inputLeft = Input::GetInstance()->PushKey(DIK_LEFT);

	if (inputRight || inputLeft) {
		//左右加速
		Vector3 acceleration = {};
		if (inputRight) {
			if (velocity_.x < 0.0f) {
				//速度と逆方向に入力は急ブレーキ
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x += kAcceleration;
		}
		else if (inputLeft) {
			if (velocity_.x > 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x -= kAcceleration;
		}
		else {
			//？入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}
		//加速/減速 velocity += acceleration
		velocity_ = Add(velocity_, acceleration);
		//最大速度制限
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	}
	//資料にないやつ
	if (velocity_.x * velocity_.x < 0.001f) {
		velocity_.x = 0;
	}
	//移動 (worldTransform_.translation_ += velocity_;)
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	//debugチェックworldTransform_ data
	playerPosition_ = worldTransform_.translation_;

	worldTransform_.UpdateMatrix();
#pragma region
	/*float destinationRotationYTable[] = {
		std::numbers::pi_v<float> / 2.0f,
		std::numbers::pi_v<float> *3.0f / 2.0f
	};

	float destinationRotationY = destinationRotationYTable[0] + destinationRotationYTable[1];
	float timeRatio = 1 - turnTimer_ / kTimeTurn;
	float easing = 1 - powf(1 - timeRatio, 3);
	float newRotationY = std::lerp(
		turnFirstRotationY_,
		destinationRotationY,
		easing);
	worldTransform_.rotation_.y = newRotationY;*/
#pragma endregion
}

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}