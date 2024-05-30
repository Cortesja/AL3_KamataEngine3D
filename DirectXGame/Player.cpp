#include "Player.h"
#include <cassert>
#include <numbers>
#include <algorithm>
#include "DirectXCommon.h"
#include "Input.h"
#include "Vector3f.h"
using MyNamespace::Matrix4x4f;

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
	worldTransform_.UpdateMatrix();
	bool inputRight = Input::GetInstance()->PushKey(DIK_RIGHT);
	bool inputLeft = Input::GetInstance()->PushKey(DIK_LEFT);
	Vector3f calc = {};

	if (inputRight || inputLeft) {
		//左右加速
		Vector3 acceleration = {};
		if (inputRight) {

			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;
			}

			if (velocity_.x < 0.0f) {
				//速度と逆方向に入力は急ブレーキ
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x += kAcceleration;
			//加速/減速 velocity_ += acceleration
			velocity_ = calc.Add(velocity_, acceleration);
			//最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		}
		if (inputLeft) {
			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
			}

			if (velocity_.x > 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x -= kAcceleration;
			//加速/減速 velocity_ += acceleration
			velocity_ = calc.Add(velocity_, acceleration);
			//最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		}
	}
	else {
		//？入力時は移動減衰をかける
		velocity_.x *= (1.0f - kAttenuation);
		//資料にないやつ
		if (velocity_.x * velocity_.x < 0.001f) {
			velocity_.x = 0;
		}
	}
	//移動 (worldTransform_.translation_ += velocity_;)
	worldTransform_.translation_ = calc.Add(worldTransform_.translation_, velocity_);
	//debugチェックworldTransform_ data
	playerPosition_ = worldTransform_.translation_;
#pragma region
	float destinationRotationYTable[] = {
		std::numbers::pi_v<float> / 2.0f,
		std::numbers::pi_v<float> *3.0f / 2.0f
	};

	float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
	float timeRatio = 1 - turnTimer_ / kTimeTurn;
	float easing = 1 - powf(1 - timeRatio, 3);
	float newRotationY = std::lerp(
		turnFirstRotationY_,
		destinationRotationY,
		easing);
	worldTransform_.rotation_.y = newRotationY;
#pragma endregion
}

const WorldTransform& Player::GetWorldTransform()
{
	return worldTransform_;
}

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}