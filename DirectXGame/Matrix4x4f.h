#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"

namespace MyNamespace {
	class Matrix4x4f
	{
	private:
		static const int kRowHeight = 20;
		static const int kColumnWidth = 60;

	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="matrix"></param>
		/// <param name="str"></param>
		void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* str);

		/// <summary>
		/// 加法
		/// </summary>
		/// <param name="m1"></param>
		/// <param name="m2"></param>
		/// <returns></returns>
		Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

		/// <summary>
		/// 減法
		/// </summary>
		/// <param name="m1"></param>
		/// <param name="m2"></param>
		/// <returns></returns>
		Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

		/// <summary>
		/// かけ算
		/// </summary>
		/// <param name="m1"></param>
		/// <param name="m2"></param>
		/// <returns></returns>
		Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

		/// <summary>
		/// 逆行列
		/// </summary>
		/// <param name="m"></param>
		/// <returns></returns>
		Matrix4x4 Inverse(const Matrix4x4& m);

		/// <summary>
		/// 転置行列
		/// </summary>
		/// <param name="m"></param>
		/// <returns></returns>
		Matrix4x4 Transpose(const Matrix4x4& m);

		/// <summary>
		/// 単位行列
		/// </summary>
		/// <returns></returns>
		Matrix4x4 MakeIdentity4x4();

		/// <summary>
		/// 平行移動行列
		/// </summary>
		/// <param name="translate"></param>
		/// <returns></returns>d
		Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
		/// <summary>
		/// 拡大縮小行列
		/// </summary>
		/// <param name="scale"></param>
		/// <returns></returns>
		Matrix4x4 MakeScaleMatrix(const Vector3& scale);
		/// <summary>
		/// 座標変換
		/// </summary>
		/// <param name="vector"></param>
		/// <param name="matrix"></param>
		/// <returns></returns>
		Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="theta"></param>
		/// <returns></returns>
		Matrix4x4 RotateX(const float& theta);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="theta"></param>
		/// <returns></returns>
		Matrix4x4 RotateY(const float& theta);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="theta"></param>
		/// <returns></returns>
		Matrix4x4 RotateZ(const float& theta);

		/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
		Matrix4x4 MakeRotateXYZ(const Vector3& theta);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="scale"></param>
		/// <param name="translate"></param>
		/// <param name="rotate"></param>
		/// <returns></returns>
		Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="left"></param>
		/// <param name="top"></param>
		/// <param name="right"></param>
		/// <param name="bottom"></param>
		/// <param name="nearClip"></param>
		/// <param name="farClip"></param>
		/// <returns></returns>
		Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="fovY"></param>
		/// <param name="aspectRatio"></param>
		/// <param name="nearClip"></param>
		/// <param name="farClip"></param>
		/// <returns></returns>
		Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="left"></param>
		/// <param name="top"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <param name="minDepth"></param>
		/// <param name="maxDepth"></param>
		/// <returns></returns>
		Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

	};
}
