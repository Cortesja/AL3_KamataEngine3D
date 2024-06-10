#define NOMINMAX
#include "Player.h"
#include <cassert>
#include <numbers>
#include <algorithm>
#include "DirectXCommon.h"
#include "Input.h"
#include "Vector3f.h"
#include "MapChipField.h"
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
	/*for (int i = 0; i < mapChipField_->GetNumBlockVertical(); i++) {
		for (int j = 0; j < mapChipField_->GetNumBlockHorizontal(); i++) {
		
		}
	}*/
	if (Input::GetInstance()->PushKey(DIK_UP) && onGround_) {
		static const float upAccel = 1.9f;
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
	//移動入力
	//移動量を加味して衝突判定する
	//判定結果を反映して移動させる
	//天井に接触している場合の処理
	//壁に接触している場合の処理
	//接地状態の切り替え
	//旋回制御 (rotation )
	//行列計算
	CollisionMapInfo info;
	worldTransform_.UpdateMatrix();
	Movement();
	Rotation();
	Jump();
	MapCollisionInfo(info);
	CollisionMapTop(info);
}

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}

void Player::MapCollisionInfo(CollisionMapInfo& info)
{
	info.move = velocity_;
	info.landing = onGround_;
	info.ceiling = false;
	info.hitWall = false;



	CollisionMapTop(info);
	//CollisionMapBottom(info);
	//CollisionMapRight(info);
	//CollisionMapLeft(info);
}

Vector3 Player::CornerPostion(const Vector3& center, Corner corner)
{
	Vector3f calc = {};
	Vector3 offsetTable[static_cast<uint32_t>(Corner::kNumCorner)] = {
		{-kWidth / 2.0f, +kHeight / 2.0f, 0}, //leftTop
		{+kWidth / 2.0f, +kHeight / 2.0f, 0}, //rightTop
		{-kWidth / 2.0f, -kHeight / 2.0f, 0}, //leftBottom
		{+kWidth / 2.0f, -kHeight / 2.0f, 0} //rightBottom
	};

	return calc.Add(center,offsetTable[static_cast<uint32_t>(corner)]);
}

void Player::CollisionMapTop(CollisionMapInfo& info)
{
	Vector3f calc = {};
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPostion(calc.Add(worldTransform_.translation_, info.move), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[static_cast<uint32_t>(Corner::kLeftTop)]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[static_cast<uint32_t>(Corner::kRightTop)]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
}

//void Player::CollisionMapBottom(CollisionMapInfo& info)
//{
//}
//
//void Player::CollisionMapRight(CollisionMapInfo& info)
//{
//}
//
//void Player::CollisionMapLeft(CollisionMapInfo& info)
//{
//}
