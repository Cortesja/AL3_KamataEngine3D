#ifndef PI
#define PI 3.14159265358979323846
#endif
#pragma once
#include "Model.h"
#include "Matrix4x4f.h"
#include "Vector3f.h"
#include "WorldTransform.h"
using MyNamespace::Matrix4x4f;

class WorldTransform;
class MapChipField;

class Enemy
{
public:
	Enemy(){}
	virtual ~Enemy() {}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="model"></param>
	/// <param name="viewProjection"></param>
	/// <param name="position"></param>
	/// <param name="textureHandler"></param>
	void Initialize(Model* model, ViewProjection* viewProjection, Vector3 position, uint32_t textureHandler);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="mapChipField"></param>
	void SetMapChipField(MapChipField* mapChipField) { mapChipfield_ = mapChipField; }
	/// <summary>
	/// 敵の移動
	/// </summary>
	virtual void Movement();
	/// <summary>
	/// 度数法 * pi/180 = 弧度法
	/// </summary>
	/// <param name="angle">度数法</param>
	/// <returns>弧度法</returns>
	float ConvertAtoR(float angle) { float radian = angle * (float(PI) / 180.0f); return radian; }
private:
	//Vector3 単純な計算をできるための変数
	Vector3f calc{};
	//Matrix4x4　関数を使えるための変数
	Matrix4x4f func{};

	//エネミーのモデル
	Model* model_ = nullptr;
	//ゲームシーンのviewProjectionをうける
	ViewProjection* viewProjection_ = nullptr;
	//ゲームシーンに映る座標変数
	WorldTransform worldTransform_;
	//マップチップデータ
	MapChipField* mapChipfield_ = nullptr;
	//textureHandler
	uint32_t textureHandler_;
	//マップチップデータを提供するためのVector3
	Vector3 position_;
	//移動量の一部
	Vector3 velocity_;
	//時間経過管理数変
	float movementTimer_;
	bool fullRotateFlag_ = false;
};

