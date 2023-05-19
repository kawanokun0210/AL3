#include "Player.h"
#include <cassert>
#include <cmath>

void Player::Initialize(Model* model, uint32_t textureHandle) {

	//NULLポインタチェック
	assert(model);
	
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	input_ = Input::GetInstance();

};

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			result.m[x][y] = m1.m[x][0] * m2.m[0][y] + m1.m[x][1] * m2.m[1][y] +
			                 m1.m[x][2] * m2.m[2][y] + m1.m[x][3] * m2.m[3][y];
		}
	}
}

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result;
	result.m[0][0] = 1;
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[2][1] = std::sin(-radian);
	result.m[2][2] = std::cos(radian);
	result.m[3][3] = 1;
	return result;
}
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result;
	result.m[0][0] = std::cos(radian);
	result.m[1][1] = std::sin(-radian);
	result.m[1][2] = 1;
	result.m[2][1] = std::sin(radian);
	result.m[2][2] = std::cos(radian);
	result.m[3][3] = 1;
	return result;
}
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result;
	result.m[0][0] = std::cos(radian);
	result.m[1][1] = std::sin(-radian);
	result.m[1][2] = std::sin(radian);
	result.m[2][1] = std::cos(radian);
	result.m[2][2] = 1;
	result.m[3][3] = 1;
	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result;
	Matrix4x4 rotateXYZMatrix = Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y),MakeRotateZMatrix(rotate.z)));

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

void Player::Update() {
	
	worldTransform_.TransferMatrix(); 

	//キャラ移動
	Vector3 move = {0, 0, 0};

	//キャラ移動の速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_UP)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y += kCharacterSpeed;
	}

	worldTransform_.translation_ = move;

};

void Player::Draw(ViewProjection viewProjection){

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

};