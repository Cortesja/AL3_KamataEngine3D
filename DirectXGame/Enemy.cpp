#define NOMINMAX
#define _USING_MATH_DEFINES
#include "Enemy.h"
#include <cassert>
#include <numbers>
#include <algorithm>
#include "DirectXCommon.h"
#include "Input.h"
#include "Vector3f.h"
#include "MapChipField.h"
#include <math.h>
using MyNamespace::Matrix4x4f;

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, Vector3 position, uint32_t textureHandler){
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	textureHandler_ = textureHandler;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	worldTransform_.rotation_.x = std::sinf(static_cast<float>(PI) / 2.0f);

	movementTimer_ = 0.0f;
}

void Enemy::Update(){
	worldTransform_.UpdateMatrix();
	Movement();
}

void Enemy::Draw(){
	model_->Draw(worldTransform_, *viewProjection_, textureHandler_);
}

void Enemy::Movement(){
	float pi = float(PI);

	movementTimer_ += 1.0f / 60.0f;
	if (movementTimer_ >= 1.0f) { 
		movementTimer_ = 0.0f;
		if (fullRotateFlag_) {
			fullRotateFlag_ = false;
		}
		else {
			fullRotateFlag_ = true;
		}
	}

	//歩くスピード
	static const float kWalkSpeed = -0.03f;
	static const float kWalkMotionAngleStart = 1.2f; //度数法
	static const float kWalkMotionAngleEnd = 2.0f; //度数法
	static const float kWalkMotionTime = 0.0f;

	float param = std::sinf(pi);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	Vector3 rotation{};
	rotation.x = ConvertAtoR(radian);

	velocity_.x = kWalkSpeed;
	//最終処理
	worldTransform_.translation_ = calc.Add(worldTransform_.translation_, velocity_);
	if (fullRotateFlag_) {
		worldTransform_.rotation_ = calc.Add(worldTransform_.rotation_, rotation);
	}
	else {
		worldTransform_.rotation_ = calc.Subtract(worldTransform_.rotation_, rotation);
	}
}
