#include "GameScene.h"
#include "Audio.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>
#include "Player.h"
#include "DebugCamera.h"
#include "Skydome.h"
#include "MapChipField.h"
#include "CameraController.h"

GameScene::~GameScene() {
	delete playerModel_;
	delete player_;
	delete blockModel_;
	delete mapChipField_;
	delete modelSkydome_;
	delete skyDome_;
	delete debugCamera_;
	delete cameraController_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::Debug()
{

#ifdef _DEBUG
	if (!isDebugCameraActive_ && input_->TriggerKey(DIK_1)) {
		isDebugCameraActive_ = true;
		isDebugWindow_ = true;
	}
	else if (isDebugCameraActive_ && input_->TriggerKey(DIK_1)) {
		isDebugCameraActive_ = false;
		isDebugWindow_ = false;
	}

	if (isDebugWindow_) {
		ImGui::Begin("Debug Window");
		ImGui::Text("cameraTarget: %f", cameraController_->GetTargetPos().x);
		ImGui::Text("currentPos: %f", cameraController_->GetCurrentPos().x);
		ImGui::Text("onGround_: %d", player_->isJump());

		ImGui::Text("playerPos: %f", player_->GetPos().x);
		ImGui::End();

		//ImGui::ShowDemoWindow();
	}
#endif // DEBUG_

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
	else {
		viewProjection_.matView = cameraController_->GetViewProjection().matView;
		viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;
		viewProjection_.UpdateMatrix();
	}
}

void GameScene::GenerateBlocks()
{
	uint32_t numBlockVertical = mapChipField_->GetNumBlockVertical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVertical);
	for (uint32_t i = 0; i < numBlockVertical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	viewProjection_.Initialize();
	//MapChipFieldの初期化
	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/block.csv");
	blockModel_ = Model::Create();
	blockTextureHandler_ = TextureManager::Load("cube/cube.jpg");
	GenerateBlocks();
	//プレイヤーの初期化
	player_ = new Player();
	playerModel_ = Model::Create();
	playerTextureHandler_ = TextureManager::Load("kamata.ico");
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(3, 18);
	player_->Initialize(playerModel_, &viewProjection_, playerPosition, playerTextureHandler_);
	//デバッグカメラ生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	//SkyDome
	skyDome_ = new Skydome();
	modelSkydome_ = Model::CreateFromOBJ("Skydome", true);
	skyDome_->Initialize(modelSkydome_, &viewProjection_);
	//CameraControllerの初期化
	cameraController_ = new CameraController();
	cameraController_->Initialize(&viewProjection_);
	cameraController_->SetTarget(player_);
	cameraController_->SetMovableArea({ 20.0f, 180.0f, 10.0f,60.0f });
	cameraController_->Reset();
}

void GameScene::Update() {
	player_->Update();
	debugCamera_->Update();
	cameraController_->Update();
	Debug();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			worldTransformBlock->UpdateMatrix();
		}
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
			if (!worldTransformBlock) {
				continue;
			}
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