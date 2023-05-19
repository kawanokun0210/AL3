#pragma once
#include <cmath>
#include <cassert>

Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result{};
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			result.m[x][y] = m1.m[x][0] * m2.m[0][y] + m1.m[x][1] * m2.m[1][y] +
			                 m1.m[x][2] * m2.m[2][y] + m1.m[x][3] * m2.m[3][y];
		}
	}
	return result;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result{};
	for (int i = 0; i < 4; i++) {
		for (int y = 0; y < 4; y++) {
			result.m[i][y] = 0;
		}
	}

	for (int i = 0; i < 4; i++) {
		result.m[i][i] = 1;
	}

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result{};
	for (int i = 0; i < 4; i++) {
		for (int y = 0; y < 4; y++) {
			result.m[i][y] = 0;
		}
	}

	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result{};
	result.m[0][0] = 1;
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[2][1] = std::sin(-radian);
	result.m[2][2] = std::cos(radian);
	result.m[3][3] = 1;
	return result;
}
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result{};
	result.m[0][0] = std::cos(radian);
	result.m[1][1] = std::sin(-radian);
	result.m[1][2] = 1;
	result.m[2][1] = std::sin(radian);
	result.m[2][2] = std::cos(radian);
	result.m[3][3] = 1;
	return result;
}
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result{};
	result.m[0][0] = std::cos(radian);
	result.m[1][1] = std::sin(-radian);
	result.m[1][2] = std::sin(radian);
	result.m[2][1] = std::cos(radian);
	result.m[2][2] = 1;
	result.m[3][3] = 1;
	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result{};
	Matrix4x4 rotateXYZMatrix = Multiply(
	    MakeRotateXMatrix(rotate.x),
	    Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));

	result.m[0][0] = rotateXYZMatrix.m[0][0] * scale.x;
	result.m[0][1] = rotateXYZMatrix.m[0][1] * scale.x;
	result.m[0][2] = rotateXYZMatrix.m[0][2] * scale.x;
	result.m[0][3] = 0;

	result.m[1][0] = rotateXYZMatrix.m[1][0] * scale.y;
	result.m[1][1] = rotateXYZMatrix.m[1][1] * scale.y;
	result.m[1][2] = rotateXYZMatrix.m[1][2] * scale.y;
	result.m[1][3] = 0;

	result.m[2][0] = rotateXYZMatrix.m[2][0] * scale.z;
	result.m[2][1] = rotateXYZMatrix.m[2][1] * scale.z;
	result.m[2][2] = rotateXYZMatrix.m[2][2] * scale.z;
	result.m[2][3] = 0;

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1;

	return result;
}

Vector3 TransformCord(Vector3& vector, Matrix4x4& matrix) {
	Vector3 result{};
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
	           1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
	           1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
	           1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
	          1.0f * matrix.m[3][3];

	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}