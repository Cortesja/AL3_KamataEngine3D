#include "Skydome.h"

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) {
	playerModel_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
}

void Skydome::Update() {
}

void Skydome::Draw() {
	playerModel_->Draw(worldTransform_, *viewProjection_);
}