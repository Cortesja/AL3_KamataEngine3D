#define NOMINMAX
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

void Player::Movement()
{
	bool inputRight = Input::GetInstance()->PushKey(DIK_RIGHT);
	bool inputLeft = Input::GetInstance()->PushKey(DIK_LEFT);
	Vector3f calc = {};

	if (inputRight || inputLeft) {
		//左右加速
		Vector3 acceleration = {};
		if (inputRight) {

			if (lrDirection_ != LRDirection::kRight) {
				turnTimer_ = kTimeTurn;
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
				turnTimer_ = kTimeTurn;
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
}

void Player::Rotation()
{
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;

		float destinationRotationYTable[] = {
			std::numbers::pi_v<float> / -1.0f,
			std::numbers::pi_v<float> / 1.0f
		};

		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		float timeRatio = 1 - turnTimer_ / kTimeTurn;
		float easing = 1 - powf(1 - timeRatio, 3);
		float newRotationY = std::lerp(
			turnFirstRotationY_,
			destinationRotationY,
			easing);
		worldTransform_.rotation_.y = newRotationY;
	}
}

void Player::Jump()
{
	if (Input::GetInstance()->PushKey(DIK_UP) && onGround_) {
		static const float upAccel = 0.9f;
		velocity_.y = upAccel;
	}
	if (!onGround_) {
		velocity_.y += kGravityAcceleration;
		velocity_.y = std::max(velocity_.y, kLimitFallSpeed);
		if (worldTransform_.translation_.y <= 2.001f) {
			onGround_ = true;
			velocity_.y = 0.0f;
			worldTransform_.translation_.y = 2.0f;
		}
	}
	if (velocity_.y > 0.0f) {
		onGround_ = false;
	}
}

void Player::Update() {
	worldTransform_.UpdateMatrix();
	Movement();
	Rotation();
	Jump();
}

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}