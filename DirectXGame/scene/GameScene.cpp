#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete blockModel_;
	delete debugCamera_;
	delete modelSkydome_;
	delete skyDome_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::GenerateBlocks()
{
	uint32_t numBlockVertical = mapChipField_->GetNumBlockVertical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//プレイヤー用の初期化
	playerTextureHandler_ = TextureManager::Load("/cube/cube.jpg");
	model_ = Model::Create();
	viewProjection_.Initialize();

	player_ = new Player();
	player_->Initialize(model_, playerTextureHandler_, &viewProjection_);
	//プレイヤーの初期化終了

	//ブロック用な処理
	blockModel_ = Model::Create();
	blockTextureHandler_ = TextureManager::Load("cube/cube.jpg");

	const uint32_t kNumBlockHorizontal = 20;
	const uint32_t kNumBlockVertical = 10;

	const float kBlockHeight = 2.0f;
	const float kBlockWidth = 2.0f;

	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			if (i % 2 == 0) {
				worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			}
			if (j % 2 == 0) {
				worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
			}
		}
	}
	//ブロック終了

	//デバッグカメラ生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//SkyDome
	modelSkydome_ = Model::CreateFromOBJ("Skydome", true);
	skyDome_ = new Skydome();
	skyDome_->Initialize(modelSkydome_, &viewProjection_);

	//MapChipField用の下書き
	mapChipField_->Initialize();

	worldTransformBlocks_.resize()
}

void GameScene::Update() {
	player_->Update();
	debugCamera_->Update();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			worldTransformBlock->UpdateMatrix();
		}
	}

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_1)) {
		isDebugCameraActive_ = true;
	}
#endif

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		viewProjection_.TransferMatrix();
	}
	else {
		viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	player_->Draw();
	skyDome_->Draw();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			blockModel_->Draw(*worldTransformBlock, viewProjection_, blockTextureHandler_);
		}
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
