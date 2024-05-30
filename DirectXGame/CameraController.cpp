#include "CameraController.h"
#include "WorldTransform.h"
#include "Player.h"

void CameraController::Initialize(ViewProjection* viewProjection)
{
	viewProjection_ = viewProjection;
}

void CameraController::Update()
{
}

void CameraController::Reset()
{
	const WorldTransform& targetWorldTransform_ = {};
	targetWorldTransform_ = target_->GetWorldTransform();
}