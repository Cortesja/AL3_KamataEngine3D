#pragma once
/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;
};

class MyVector3 {
public:
	Vector3 vector;
	MyVector3() {
		vector = {};
	}
	MyVector3(Vector3 num) {
		vector = num;
	}
	MyVector3& operator+=(const MyVector3& num) {
		vector.x += num.vector.x;
		vector.y += num.vector.y;
		vector.z += num.vector.z;
		return *this;
	}
};