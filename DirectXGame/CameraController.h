#pragma once
#include "ViewProjection.h"
class Player;
class WorldTransform;
/// <summary>
/// カメラコントローラ
/// </summary>
class CameraController
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ViewProjection* viewProjection);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(Player* target) { target_ = target; }
	/// <summary>
	/// 
	/// </summary>
	void Reset();

private:
	ViewProjection *viewProjection_;
	Vector3 targetOffset_ = { 0.0f, 0.0f, -15.0f };
	Player* target_ = nullptr;
};

