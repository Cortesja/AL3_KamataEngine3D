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

struct IndexSet {
	uint32_t xIndex;
	uint32_t yIndex;
};

/// <summary>
/// マップチップフィールド
/// </summary>
class MapChipField
{
private:
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;

	static inline const uint32_t kNumBlockVertical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;
	MapChipField* mapChipField_;
	IndexSet indexSet_;
public:
	MapChipField() = default;
	~MapChipField() = default;

	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filePath);

	uint32_t GetNumBlockVertical() { return kNumBlockVertical; }
	uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; }

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);
};