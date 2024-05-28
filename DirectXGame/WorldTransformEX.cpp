#include "WorldTransform.h"
using MyNamespace::Matrix4x4f;

void WorldTransform::UpdateMatrix()
{
	Matrix4x4f temp = {};
	matWorld_ = temp.MakeAffineMatrix(scale_, rotation_, translation_);
	TransferMatrix();
}