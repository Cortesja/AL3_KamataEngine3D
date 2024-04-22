#pragma once
#include "Model.h"
#include "WorldTransform.h"

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
	void Initialize(Model *model, uint32_t textureHandler);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャーハンドル
	uint32_t textureHandle_ = 0u;
};

