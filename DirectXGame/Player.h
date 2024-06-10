#pragma once
#include "Model.h"
#include "Matrix4x4f.h"
#include "WorldTransform.h"

class WorldTransform;
class MapChipField;
/// <summary>
/// 自キャラ
/// </summary>
class Player
{
public:
	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		Vector3 move;
	};
	enum class Corner {
		kLeftTop,
		kRightTop,
		kLeftBottom,
		kRightBottom,
		kNumCorner //要素数
	};
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

	/// <summary>
	/// MapChipField型からプレイヤークラスに使えるように取得する関数
	/// </summary>
	/// <param name="mapChipField">マップチップデータが保留されている変数</param>
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	void MapCollisionInfo(CollisionMapInfo& info);
	Vector3 CornerPostion(const Vector3& center, Corner corner);

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
	//当たり判定の変数をよい
	MapChipField* mapChipField_ = nullptr;
	static inline const float kWidth = 2.0f;
	static inline const float kHeight = 2.0f;
	CollisionMapInfo info_;

	void CollisionMapTop(CollisionMapInfo& info);
	//void CollisionMapBottom(CollisionMapInfo& info);
	//void CollisionMapRight(CollisionMapInfo& info);
	//void CollisionMapLeft(CollisionMapInfo& info);
};