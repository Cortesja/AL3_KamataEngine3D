#include "Matrix4x4f.h"
#include "Novice.h"
#include <math.h>
#include <cassert>

void MyNamespace::Matrix4x4f::MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* str) {
	Novice::ScreenPrintf(x, y + kRowHeight, "%s", str);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(
				(x + column * kColumnWidth) + kRowHeight, (y + row * kRowHeight) + kRowHeight * 2, "%6.02f", matrix.m[row][column]);
		}
	}
}

Matrix4x4 MyNamespace::Matrix4x4f::Add(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}

	return result;
}

Matrix4x4 MyNamespace::Matrix4x4f::Subtract(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return result;
}

Matrix4x4 MyNamespace::Matrix4x4f::Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

Matrix4x4 MyNamespace::Matrix4x4f::Inverse(const Matrix4x4& m)
{
	Matrix4x4 result = {};
	float determinant = m.m[0][0] * (m.m[1][1] * m.m[2][2] * m.m[3][3] +
		m.m[2][1] * m.m[3][2] * m.m[1][3] +
		m.m[3][1] * m.m[1][2] * m.m[2][3] -
		m.m[3][1] * m.m[2][2] * m.m[1][3] -
		m.m[2][1] * m.m[1][2] * m.m[3][3] -
		m.m[1][1] * m.m[3][2] * m.m[2][3]) -
		m.m[0][1] * (m.m[1][0] * m.m[2][2] * m.m[3][3] +
			m.m[2][0] * m.m[3][2] * m.m[1][3] +
			m.m[3][0] * m.m[1][2] * m.m[2][3] -
			m.m[3][0] * m.m[2][2] * m.m[1][3] -
			m.m[2][0] * m.m[1][2] * m.m[3][3] -
			m.m[1][0] * m.m[3][2] * m.m[2][3]) +
		m.m[0][2] * (m.m[1][0] * m.m[2][1] * m.m[3][3] +
			m.m[2][0] * m.m[3][1] * m.m[1][3] +
			m.m[3][0] * m.m[1][1] * m.m[2][3] -
			m.m[3][0] * m.m[2][1] * m.m[1][3] -
			m.m[2][0] * m.m[1][1] * m.m[3][3] -
			m.m[1][0] * m.m[3][1] * m.m[2][3]) -
		m.m[0][3] * (m.m[1][0] * m.m[2][1] * m.m[3][2] +
			m.m[2][0] * m.m[3][1] * m.m[1][2] +
			m.m[3][0] * m.m[1][1] * m.m[2][2] -
			m.m[3][0] * m.m[2][1] * m.m[1][2] -
			m.m[2][0] * m.m[1][1] * m.m[3][2] -
			m.m[1][0] * m.m[3][1] * m.m[2][2]);



	if (determinant != 0) {
		result.m[0][0] = (m.m[1][1] * m.m[2][2] * m.m[3][3] +
			m.m[2][1] * m.m[3][2] * m.m[1][3] +
			m.m[3][1] * m.m[1][2] * m.m[2][3] -
			m.m[3][1] * m.m[2][2] * m.m[1][3] -
			m.m[2][1] * m.m[1][2] * m.m[3][3] -
			m.m[1][1] * m.m[3][2] * m.m[2][3]) /
			determinant;

		result.m[0][1] = -(m.m[0][1] * m.m[2][2] * m.m[3][3] +
			m.m[2][1] * m.m[3][2] * m.m[0][3] +
			m.m[3][1] * m.m[0][2] * m.m[2][3] -
			m.m[3][1] * m.m[2][2] * m.m[0][3] -
			m.m[2][1] * m.m[0][2] * m.m[3][3] -
			m.m[0][1] * m.m[3][2] * m.m[2][3]) /
			determinant;

		result.m[0][2] = (m.m[0][1] * m.m[1][2] * m.m[3][3] +
			m.m[1][1] * m.m[3][2] * m.m[0][3] +
			m.m[3][1] * m.m[0][2] * m.m[1][3] -
			m.m[3][1] * m.m[1][2] * m.m[0][3] -
			m.m[1][1] * m.m[0][2] * m.m[3][3] -
			m.m[0][1] * m.m[3][2] * m.m[1][3]) /
			determinant;

		result.m[0][3] = -(m.m[0][1] * m.m[1][2] * m.m[2][3] +
			m.m[1][1] * m.m[2][2] * m.m[0][3] +
			m.m[2][1] * m.m[0][2] * m.m[1][3] -
			m.m[2][1] * m.m[1][2] * m.m[0][3] -
			m.m[1][1] * m.m[0][2] * m.m[2][3] -
			m.m[0][1] * m.m[2][2] * m.m[1][3]) /
			determinant;


		result.m[1][0] = -(m.m[1][0] * m.m[2][2] * m.m[3][3] +
			m.m[2][0] * m.m[3][2] * m.m[1][3] +
			m.m[3][0] * m.m[1][2] * m.m[2][3] -
			m.m[3][0] * m.m[2][2] * m.m[1][3] -
			m.m[2][0] * m.m[1][2] * m.m[3][3] -
			m.m[1][0] * m.m[3][2] * m.m[2][3]) /
			determinant;

		result.m[1][1] = (m.m[0][0] * m.m[2][2] * m.m[3][3] +
			m.m[2][0] * m.m[3][2] * m.m[0][3] +
			m.m[3][0] * m.m[0][2] * m.m[2][3] -
			m.m[3][0] * m.m[2][2] * m.m[0][3] -
			m.m[2][0] * m.m[0][2] * m.m[3][3] -
			m.m[0][0] * m.m[3][2] * m.m[2][3]) /
			determinant;

		result.m[1][2] = -(m.m[0][0] * m.m[1][2] * m.m[3][3] +
			m.m[1][0] * m.m[3][2] * m.m[0][3] +
			m.m[3][0] * m.m[0][2] * m.m[1][3] -
			m.m[3][0] * m.m[1][2] * m.m[0][3] -
			m.m[1][0] * m.m[0][2] * m.m[3][3] -
			m.m[0][0] * m.m[3][2] * m.m[1][3]) /
			determinant;

		result.m[1][3] = (m.m[0][0] * m.m[1][2] * m.m[2][3] +
			m.m[1][0] * m.m[2][2] * m.m[0][3] +
			m.m[2][0] * m.m[0][2] * m.m[1][3] -
			m.m[2][0] * m.m[1][2] * m.m[0][3] -
			m.m[1][0] * m.m[0][2] * m.m[2][3] -
			m.m[0][0] * m.m[2][2] * m.m[1][3]) /
			determinant;


		result.m[2][0] = (m.m[1][0] * m.m[2][1] * m.m[3][3] +
			m.m[2][0] * m.m[3][1] * m.m[1][3] +
			m.m[3][0] * m.m[1][1] * m.m[2][3] -
			m.m[3][0] * m.m[2][1] * m.m[1][3] -
			m.m[2][0] * m.m[1][1] * m.m[3][3] -
			m.m[1][0] * m.m[3][1] * m.m[2][3]) /
			determinant;

		result.m[2][1] = -(m.m[0][0] * m.m[2][1] * m.m[3][3] +
			m.m[2][0] * m.m[3][1] * m.m[0][3] +
			m.m[3][0] * m.m[0][1] * m.m[2][3] -
			m.m[3][0] * m.m[2][1] * m.m[0][3] -
			m.m[2][0] * m.m[0][1] * m.m[3][3] -
			m.m[0][0] * m.m[3][1] * m.m[2][3]) /
			determinant;

		result.m[2][2] = (m.m[0][0] * m.m[1][1] * m.m[3][3] +
			m.m[1][0] * m.m[3][1] * m.m[0][3] +
			m.m[3][0] * m.m[0][1] * m.m[1][3] -
			m.m[3][0] * m.m[1][1] * m.m[0][3] -
			m.m[1][0] * m.m[0][1] * m.m[3][3] -
			m.m[0][0] * m.m[3][1] * m.m[1][3]) /
			determinant;

		result.m[2][3] = -(m.m[0][0] * m.m[1][1] * m.m[2][3] +
			m.m[1][0] * m.m[2][1] * m.m[0][3] +
			m.m[2][0] * m.m[0][1] * m.m[1][3] -
			m.m[2][0] * m.m[1][1] * m.m[0][3] -
			m.m[1][0] * m.m[0][1] * m.m[2][3] -
			m.m[0][0] * m.m[2][1] * m.m[1][3]) /
			determinant;

		result.m[3][0] = -(m.m[1][0] * m.m[2][1] * m.m[3][2] +
			m.m[2][0] * m.m[3][1] * m.m[1][2] +
			m.m[3][0] * m.m[1][1] * m.m[2][2] -
			m.m[3][0] * m.m[2][1] * m.m[1][2] -
			m.m[2][0] * m.m[1][1] * m.m[3][2] -
			m.m[1][0] * m.m[3][1] * m.m[2][2]) /
			determinant;

		result.m[3][1] = (m.m[0][0] * m.m[2][1] * m.m[3][2] +
			m.m[2][0] * m.m[3][1] * m.m[0][2] +
			m.m[3][0] * m.m[0][1] * m.m[2][2] -
			m.m[3][0] * m.m[2][1] * m.m[0][2] -
			m.m[2][0] * m.m[0][1] * m.m[3][2] -
			m.m[0][0] * m.m[3][1] * m.m[2][2]) /
			determinant;

		result.m[3][2] = -(m.m[0][0] * m.m[1][1] * m.m[3][2] +
			m.m[1][0] * m.m[3][1] * m.m[0][2] +
			m.m[3][0] * m.m[0][1] * m.m[1][2] -
			m.m[3][0] * m.m[1][1] * m.m[0][2] -
			m.m[1][0] * m.m[0][1] * m.m[3][2] -
			m.m[0][0] * m.m[3][1] * m.m[1][2]) /
			determinant;

		result.m[3][3] = (m.m[0][0] * m.m[1][1] * m.m[2][2] +
			m.m[1][0] * m.m[2][1] * m.m[0][2] +
			m.m[2][0] * m.m[0][1] * m.m[1][2] -
			m.m[2][0] * m.m[1][1] * m.m[0][2] -
			m.m[1][0] * m.m[0][1] * m.m[2][2] -
			m.m[0][0] * m.m[2][1] * m.m[1][2]) /
			determinant;
	}

	return result;
}

Matrix4x4 MyNamespace::Matrix4x4f::Transpose(const Matrix4x4& m)
{
	Matrix4x4 result;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m.m[j][i];
		}
	}
	return result;
}

Matrix4x4 MyNamespace::Matrix4x4f::MakeIdentity4x4()
{
	Matrix4x4 result{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};
	return result;
}

Matrix4x4 MyNamespace::Matrix4x4f::MakeTranslateMatrix(const Vector3& translate)
{
	Matrix4x4 result = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	translate.x, translate.y, translate.z, 1.0f
	};
	return result;
}

Matrix4x4 MyNamespace::Matrix4x4f::MakeScaleMatrix(const Vector3& scale)
{
	Matrix4x4 result = {
		scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return result;
}

Vector3 MyNamespace::Matrix4x4f::Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

Matrix4x4 MyNamespace::Matrix4x4f::RotateX(const float& theta)
{
	Matrix4x4 result = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosf(theta), sinf(theta), 0.0f,
		0.0f, -sinf(theta), cosf(theta), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return result;
}

Matrix4x4 MyNamespace::Matrix4x4f::RotateY(const float& theta)
{
	Matrix4x4 result = {
		cosf(theta), 0.0f, -sinf(theta), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sinf(theta), 0.0f, cosf(theta), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return result;
}

Matrix4x4 MyNamespace::Matrix4x4f::RotateZ(const float& theta)
{
	Matrix4x4 result = {
		cosf(theta), sinf(theta), 0.0f, 0.0f,
		-sinf(theta), cosf(theta), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return result;
}

Matrix4x4 MyNamespace::Matrix4x4f::MakeRotateXYZ(const Vector3& theta)
{
	Matrix4x4 rotX = RotateX(theta.x);
	Matrix4x4 rotY = RotateY(theta.y);
	Matrix4x4 rotZ = RotateZ(theta.z);
	Matrix4x4 rotXY = Multiply(rotX, rotY);
	Matrix4x4 result = Multiply(rotXY, rotZ);

	return result;
}

Matrix4x4 MyNamespace::Matrix4x4f::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 scaleMat = MakeScaleMatrix(scale);
	Matrix4x4 rotateXYZ = MakeRotateXYZ(rotate);
	Matrix4x4 translateMat = MakeTranslateMatrix(translate);
	Matrix4x4 mulRotateScale = Multiply(rotateXYZ, scaleMat);
	Matrix4x4 result = Multiply(mulRotateScale, translateMat);
	return result;
}

Matrix4x4 MyNamespace::Matrix4x4f::MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
{
	Matrix4x4 result = {
		float(2.0f / (right - left)), 0.0f, 0.0f, 0.0f,
		0.0f, float(2.0f / (top - bottom)), 0.0f, 0.0f,
		0.0f, 0.0f, float(1.0f / (farClip - nearClip)), 0.0f,
		float((left + right) / (left - right)), float((top + bottom) / (bottom - top)), float((nearClip) / (nearClip - farClip)), 1.0f
	};
	return result;
}

Matrix4x4 MyNamespace::Matrix4x4f::MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
	Matrix4x4 result = {
		((1 / aspectRatio) * (1.0f / tanf(fovY / 2))), 0.0f, 0.0f, 0.0f,
		0.0f, (1.0f / tanf(fovY / 2)), 0.0f, 0.0f,
		0.0f, 0.0f, (farClip / (farClip - nearClip)),1.0f,
		0.0f, 0.0f, ((-nearClip * farClip) / (farClip - nearClip)),0.0f
	};

	return result;
}

Matrix4x4 MyNamespace::Matrix4x4f::MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	Matrix4x4 result = {
		(width / 2), 0.0f, 0.0f, 0.0f,
		0.0f, float(-(height/2)), 0.0f, 0.0f,
		0.0f, 0.0f, (maxDepth - minDepth), 0.0f,
		(left + (width / 2)), (top + (height / 2)), minDepth, 1.0f
	};

	return result;
}
