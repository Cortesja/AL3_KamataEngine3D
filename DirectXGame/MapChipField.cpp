#include "MapChipField.h"
#include <map>
#include <fstream>
#include <sstream>
#include <cassert>

namespace {
	std::map<std::string, MapChipType> mapChipTable = {
			{"0", MapChipType::kBlank },
			{"1", MapChipType::kBlock}
	};
}

void MapChipField::ResetMapChipData()
{
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath)
{
	//ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	//マップチップcsv
	std::stringstream mapChipCsv;
	//ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();
	//ファイルを閉じる
	file.close();

	// CSVからマップチップデータを読み込む
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		std::string line;
		getline(mapChipCsv, line);

		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++i) {

			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}
		}
	}
}

void MapChipField::Initialize()
{
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/block.csv");
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex)
{
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex)
{
	return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0);
}
