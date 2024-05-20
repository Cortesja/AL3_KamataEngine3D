#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <MapChipField.h>


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	void GenerateBlocks();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	//プレイヤーを初期化するための
	uint32_t playerTextureHandler_ = 0;
	Model* model_ = nullptr;
	ViewProjection viewProjection_;

	Player* player_ = nullptr;
	//プレイヤー使用する変数は終了

	Model* blockModel_ = nullptr;
	uint32_t blockTextureHandler_ = 0;
	ViewProjection blockViewProjection_;

	WorldTransform* worldTransformBlocks_;
	std::vector<WorldTransform*> worldTransformBlocks_;

	MapChipField* mapChipField_;
};
