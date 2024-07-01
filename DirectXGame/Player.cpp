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
	if (Input::GetInstance()->PushKey(DIK_UP) && onGround_) {
		static const float upAccel = 1.9f;
		velocity_.y = upAccel;
	}
	if (!onGround_) {
		velocity_.y += kGravityAcceleration;
		velocity_.y = std::max(velocity_.y, kLimitFallSpeed);
		if (onGround_) {
			onGround_ = true;
			velocity_.y = 0.0f;
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
	worldTransform_.UpdateMatrix();
	Movement();
	Rotation();
	MapCollisionInfo(info_);
	Jump();
}

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}

void Player::MapCollisionInfo(CollisionMapInfo& info)
{
	//info.move = velocity_;
	//info.landing = onGround_;
	//info.ceiling = false;
	//info.hitWall = false;

	CollisionMapTop(info);
	info.ceiling = false;
	CollisionMapBottom(info);
	if (info.landing) {
		onGround_ = true;
		info.landing = false;
	}
	CollisionMapRight(info);
	CollisionMapLeft(info);
}

Vector3 Player::CornerPostion(const Vector3& center, Corner corner)
{
	Vector3f calc = {};
	//{-kWidth / 2.0f, +kHeight / 2.0f, 0}, //leftTop
	//{+kWidth / 2.0f, +kHeight / 2.0f, 0}, //rightTop
	//{ -kWidth / 2.0f, -kHeight / 2.0f, 0 }, //leftBottom
	//{ +kWidth / 2.0f, -kHeight / 2.0f, 0 } //rightBottom
	Vector3 offsetTable[static_cast<uint32_t>(Corner::kNumCorner)] = {
		{0, 0, 0}, //leftTop
		{+kWidth, 0, 0}, //rightTop
		{0, -kHeight, 0}, //leftBottom
		{+kWidth, -kHeight, 0} //rightBottom
	};

	return calc.Add(center,offsetTable[static_cast<uint32_t>(corner)]);
}

void Player::CollisionMapTop(CollisionMapInfo& info)
{
	info.move = velocity_;
	if (info.move.y <= 0.0f) { return; } //動いてないなら処理を飛ばす
	Vector3f calc = {};
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPostion(worldTransform_.translation_, static_cast<Corner>(i));
	}
	
	MapChipType mapChipType1;
	MapChipType mapChipType2;
	bool hit1 = false;
	bool hit2 = false;

	IndexSet indexSet1;
	IndexSet indexSet2;
	indexSet1 = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[static_cast<uint32_t>(Corner::kLeftTop)]);
	indexSet2 = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[static_cast<uint32_t>(Corner::kRightTop)]);
	//playerIndexX = indexSet1.xIndex; //debug
	//playerIndexY = indexSet1.yIndex; //debug
	mapChipType1 = mapChipField_->GetMapChipTypeByIndex(indexSet1.xIndex, indexSet1.yIndex);
	mapChipType2 = mapChipField_->GetMapChipTypeByIndex(indexSet2.xIndex, indexSet2.yIndex);

	if (mapChipType1 == MapChipType::kBlock) {
		hit1 = true;
	}
	if (mapChipType2 == MapChipType::kBlock) {
		hit2 = true;
	}

	if (hit1 || hit2) {
		info.ceiling = true;
		worldTransform_.translation_.y = mapChipField_->GetMapChipPositionByIndex(indexSet2.xIndex, indexSet2.yIndex + 1).y;
		if (velocity_.y > 0.0f) {
			velocity_.y = 0.0f;
		}
	}
}

void Player::CollisionMapBottom(CollisionMapInfo& info)
{
	Vector3f calc = {};
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPostion(worldTransform_.translation_, static_cast<Corner>(i));
	}

	MapChipType mapChipTypeLeft;
	MapChipType mapChipTypeRight;
	bool hitLeft = false;
	bool hitRight = false;
	IndexSet indexSet1;
	IndexSet indexSet2;
	indexSet1 = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[static_cast<uint32_t>(Corner::kLeftBottom)]);
	indexSet2 = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[static_cast<uint32_t>(Corner::kRightBottom)]);
	mapChipTypeLeft = mapChipField_->GetMapChipTypeByIndex(indexSet1.xIndex, indexSet1.yIndex);
	mapChipTypeRight = mapChipField_->GetMapChipTypeByIndex(indexSet2.xIndex, indexSet2.yIndex);

	if (mapChipTypeLeft == MapChipType::kBlock) {
		hitLeft = true;
	}
	if (mapChipTypeRight == MapChipType::kBlock) {
		hitRight = true;
	}
	if (hitRight || hitLeft) {
		info.landing = true;
		velocity_.y *= (1.0f - kAttenuation);
		velocity_.y = 0.0f;
		worldTransform_.translation_.y = mapChipField_->GetMapChipPositionByIndex(indexSet2.xIndex, indexSet2.yIndex - 1).y;
	}
	if (mapChipTypeLeft == MapChipType::kBlank && mapChipTypeRight == MapChipType::kBlank) {
		onGround_ = false;
	}
}

void Player::CollisionMapRight(CollisionMapInfo& info)
{
	info.move = velocity_;
	if (info.move.x <= 0.0f) { return; }

	Vector3f calc = {};
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPostion(worldTransform_.translation_, static_cast<Corner>(i));
	}

	MapChipType mapChipTypeTop;
	MapChipType mapChipTypeBottom;
	bool hitTop = false;
	bool hitBottom = false;
	IndexSet indexSetTop;
	IndexSet indexSetBottom;
	//TOP
	indexSetTop = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[static_cast<uint32_t>(Corner::kRightTop)]);
	mapChipTypeTop = mapChipField_->GetMapChipTypeByIndex(indexSetTop.xIndex, indexSetTop.yIndex);
	//BOTTOM
	indexSetBottom = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[static_cast<uint32_t>(Corner::kRightBottom)]);
	mapChipTypeBottom = mapChipField_->GetMapChipTypeByIndex(indexSetTop.xIndex, indexSetBottom.yIndex);

	if (mapChipTypeTop == MapChipType::kBlock) {
		hitTop = true;
	}
	if (mapChipTypeBottom == MapChipType::kBlock) {
		hitBottom = true;
	}
	if (hitTop) {
		info.hitWall = true;
		velocity_.x = 0.0f;
		//worldTransform_.translation_.x = mapChipField_->GetMapChipPositionByIndex(indexSetBottom.xIndex - 1, indexSetBottom.yIndex).x;
	}
}

void Player::CollisionMapLeft(CollisionMapInfo& info)
{
	info.move = velocity_;
	if (info.move.x > 0.0f) { return; }

	Vector3f calc = {};
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPostion(calc.Add(playerPosition_, info.move), static_cast<Corner>(i));
	}
	
	MapChipType mapChipTypeTop;
	MapChipType mapChipTypeBottom;
	bool hitTop = false;
	bool hitBottom = false;
	IndexSet indexSetTop;
	IndexSet indexSetBottom;

	indexSetTop = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[static_cast<uint32_t>(Corner::kLeftTop)]);
	mapChipTypeTop = mapChipField_->GetMapChipTypeByIndex(indexSetTop.xIndex, indexSetTop.yIndex);

	indexSetBottom = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[static_cast<uint32_t>(Corner::kLeftBottom)]);
	mapChipTypeBottom = mapChipField_->GetMapChipTypeByIndex(indexSetBottom.xIndex, indexSetBottom.yIndex);

	if (mapChipTypeTop == MapChipType::kBlock) {
		hitTop = true;
	}
	if (mapChipTypeBottom == MapChipType::kBlock) {
		hitBottom = true;
	}
	if (hitTop) {
		info.hitWall = true;
		velocity_.x = 0.0f;
		//worldTransform_.translation_.x = mapChipField_->GetMapChipPositionByIndex(indexSetBottom.xIndex + 1, indexSetBottom.yIndex).x;
	}
}