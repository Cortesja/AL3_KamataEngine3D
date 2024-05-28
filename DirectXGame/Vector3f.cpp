#include "Vector3f.h"
#include "Novice.h"
#include <math.h>

void Vector3f::VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

Vector3 Vector3f::Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result{ 0 };
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

Vector3 Vector3f::Subtract(const Vector3& v1, const Vector3& v2)
{
	Vector3 result = { 0 };
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

Vector3 Vector3f::Multiply(float scalar, const Vector3& v)
{
	Vector3 result = { 0 };
	result.x = scalar * v.x;
	result.y = scalar * v.y;
	result.z = scalar * v.z;
	return result;
}

float Vector3f::Dot(const Vector3& v1, const Vector3& v2)
{
	float result = ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
	return result;
}

float Vector3f::Length(const Vector3& v)
{
	Vector3 result;
	result.x = v.x * v.x;
	result.y = v.y * v.y;
	result.z = v.z * v.z;

	float length = sqrtf(result.x + result.y + result.z);
	return length;
}

Vector3 Vector3f::Normalize(const Vector3& v)
{
	Vector3 result;
	float len = Length(v);

	result.x = v.x / len;
	result.y = v.y / len;
	result.z = v.z / len;

	return result;
}