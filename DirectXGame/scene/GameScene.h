#pragma once
#include "Input.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Matrix4x4.h"
#include <vector>

class Audio;
class DirectXCommon;
class Model;
class Player;
class DebugCamera;
class Skydome;
class MapChipField;
class CameraController;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// デバッグ用
	/// </summary>
	void Debug();

	/// <summary>
	/// ブロックの位置を定義する
	/// </summary>
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
	/// 
	/// 
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	ViewProjection viewProjection_;

	//プレイヤーを初期化するための
	uint32_t playerTextureHandler_ = 0;
	Model* playerModel_ = nullptr;
	Player* player_ = nullptr;
	//Blocks
	Model* blockModel_ = nullptr;
	uint32_t blockTextureHandler_ = 0u;
	//SkyDome
	Model* modelSkydome_ = nullptr;
	Skydome* skyDome_ = nullptr;
	//マップチップデータを待たせる変数
	MapChipField* mapChipField_;
	//debug用
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;
	bool isDebugWindow_ = false;
	//CameraController
	CameraController* cameraController_ = nullptr;
};