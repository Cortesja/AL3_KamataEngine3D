#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	textureHandle_ = TextureManager::Load("sample.png");
	sprite_ = Sprite::Create(textureHandle_, { 100, 50 });
	soundDataHandle_ = audio_->LoadWave("mokugyo.wav");
	
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);

	model_ = Model::Create();

	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {
	Vector2 position = sprite_->GetPosition();

	position.x += 2.0f;
	position.y += 1.0f;

	sprite_->SetPosition(position);

	debugCamera_->Update();

	if (input_->TriggerKey(DIK_SPACE)) {
		if (audio_->IsPlaying(voiceHandle_)) {
			audio_->StopWave(voiceHandle_);
		}
		else {
			voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);
		}
	}

#ifdef _DEBUG

	ImGui::Begin("PlayerParameter");
	ImGui::Text("Name : Kamata Tarou %d.%d.%d", 2050, 12, 31);
	ImGui::Text("HP : %d", 50);
	ImGui::Text("Magic : %d", 100);
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::End();

	ImGui::ShowDemoWindow();

#endif // DEBUG

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
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	int lineMeshWidth = 60;
	int lineMeshHeight = 60;
	int lineMeshSpanH = 3;
	int lineMeshSpanW = 3;

	for (int h = -lineMeshWidth / 2; h <= lineMeshWidth / 2; h += lineMeshSpanH) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
			{ (float)h, (float)-lineMeshWidth / 2,0},
			{ (float)h, (float)lineMeshWidth / 2,0 },
			{ 0.0f,0.0f,1.0f,1.0f }
		);
	}

	for (int v = -lineMeshHeight / 2; v <= lineMeshHeight / 2; v += lineMeshSpanW) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
			{ (float)-lineMeshHeight / 2, (float)v, 0},
			{ (float)lineMeshHeight / 2, (float)v, 0},
			{ 1.0f,0.0f,0.0f,1.0f }
		);
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
	sprite_->Draw();


	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
