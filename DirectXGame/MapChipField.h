#pragma once
#include <iostream>
#include <vector>
#include "Vector3.h"

enum class MapChipType {
	kBlank,
	kBlock
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

/// <summary>
/// マップチップフィールド
/// </summary>
class MapChipField
{
private:
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;
	MapChipField* mapChipField_;
public:
	MapChipField() {};
	~MapChipField() { delete mapChipField_; };

	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filePath);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
};