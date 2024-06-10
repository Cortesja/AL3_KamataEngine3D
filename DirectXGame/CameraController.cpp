#define NOMINMAX
#include "CameraController.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Vector3f.h"
#include <numbers>
#include <algorithm>

void CameraController::Initialize(ViewProjection *viewProjection) {
	viewProjection_ = viewProjection;
}

void CameraController::Update()
{
	Lerp(viewProjection_->translation_, targetDestination_);

	viewProjection_->translation_.x = std::max(viewProjection_->translation_.x, movableArea_.left);
	viewProjection_->translation_.x = std::min(viewProjection_->translation_.x, movableArea_.right);
	viewProjection_->translation_.y = std::max(viewProjection_->translation_.y, movableArea_.bottom);
	viewProjection_->translation_.y = std::min(viewProjection_->translation_.y, movableArea_.top);
	viewProjection_->UpdateMatrix();
}

void CameraController::Lerp(Vector3 &viewProjection, Vector3 &targetDestination)
{
	Vector3f calc = {};
	Vector3 targetSpeed = target_->GetVelocity();
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	if (targetSpeed.x > 0.0f) {
		Vector3 temp = calc.Add(targetSpeed, kVelocityBias);
		Vector3 temp1 = calc.Add(temp, targetOffset_);

		targetDestination = calc.Add(targetWorldTransform.translation_, temp1);
		if (viewProjection.x < targetDestination.x) {
			viewProjection.x += kInterpolationRate;
		}
	}
	if (targetSpeed.x < 0.0f) {
		Vector3 temp = calc.Add(targetSpeed, -kVelocityBias);
		Vector3 temp1 = calc.Add(temp, targetOffset_);

		targetDestination = calc.Add(targetWorldTransform.translation_, temp1);

			if (viewProjection.x > targetDestination.x) {
				viewProjection.x -= kInterpolationRate;
			}
	}
	if (targetSpeed.x == 0.0f) {
		Reset();
	}

	//debug用
	cameraTarget_ = targetDestination_;
	curretPos_ = viewProjection;
}

void CameraController::Reset()
{
	Vector3f calc = {};
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	viewProjection_->translation_ = calc.Add(targetWorldTransform.translation_, targetOffset_);
}
