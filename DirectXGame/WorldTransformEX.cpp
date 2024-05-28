#include "WorldTransform.h"
using MyNamespace::Matrix4x4f;

void WorldTransform::UpdateMatrix()
{
	Matrix4x4f calc = {};
	matWorld_ = calc.MakeAffineMatrix(scale_, rotation_, translation_);
	TransferMatrix();
}