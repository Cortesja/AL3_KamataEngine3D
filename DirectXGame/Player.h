#pragma once
#include "Model.h"
#include "WorldTransform.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player
{
	enum class LTDirection {
		kRight,
		kLeft,
	};
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection* viewProjection_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャーハンドル
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_ = {};

	static inline const float kAcceleration = 1.0f;//数値;
	static inline const float kAttenuation = 2.0f;

	static inline const float kLimitRunSpeed = 5.0f;

	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	static inline const float kTimeTurn = 0.3f;
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
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//debug用
	Vector3 playerPosition_ = {};
	Vector3 GetPos() { return playerPosition_; }
};