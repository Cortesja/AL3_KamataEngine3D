#pragma once
#include "Model.h"
#include "Matrix4x4f.h"
#include "WorldTransform.h"

class WorldTransform;
/// <summary>
/// 自キャラ
/// </summary>
class Player
{
public:
	Player();
	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデルのポインター</param>
	/// <param name="textureHandler">テクスチャーハンドル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Initialize(Model* model, ViewProjection *viewProjecion, const Vector3& position, uint32_t playerTextureHandler);
	/// <summary>
	/// 左右移動
	/// </summary>
	void Movement();
	/// <summary>
	/// 回転
	/// </summary>
	void Rotation();
	/// <summary>
	/// ジャンプ
	/// </summary>
	void Jump();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	const Vector3& GetVelocity() { return velocity_; }
	//debug用
	Vector3 playerPosition_ = {};
	Vector3 GetPos() { return playerPosition_; }
	bool isJump() { return onGround_; }

private:
	enum class LRDirection {
		kRight,
		kLeft,
	};
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection *viewProjection_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャーハンドル
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_ = {};
	//移動向き
	LRDirection lrDirection_ = LRDirection::kRight;

	static inline const float kAcceleration = 0.01f;//数値;
	static inline const float kAttenuation = 0.03f;

	static inline const float kLimitRunSpeed = 0.3f;

	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	static inline const float kTimeTurn = 0.3f;
	//ジャンプ
	bool onGround_ = true;
	//重力加速度 (下方向)
	static inline const float kGravityAcceleration = -0.06f;
	//最大落下速度 (下方向)
	static inline const float kLimitFallSpeed = -0.3f;
	//ジャンプ初速 (上方向)
	static inline const float kJumpAcceleration = 0.01f;
};