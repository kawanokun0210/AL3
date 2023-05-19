#include "Player.h"
#include "newMath.h"
#include "ImGuiManager.h"
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

void Player::Update() {

	Matrix4x4 scaleMatrix = MakeScaleMatrix(worldTransform_.scale_);

	Matrix4x4 rotateMatrixX = MakeRotateXMatrix(worldTransform_.rotation_.x);
	Matrix4x4 rotateMatrixY = MakeRotateYMatrix(worldTransform_.rotation_.y);
	Matrix4x4 rotateMatrixZ = MakeRotateZMatrix(worldTransform_.rotation_.z);
	Matrix4x4 rotateMatrixXYZ = Multiply(Multiply(rotateMatrixX, rotateMatrixY), rotateMatrixZ);

	Matrix4x4 translateMatrix = MakeTranslateMatrix(worldTransform_.translation_);

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
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	worldTransform_.translation_ = TransformCord(move, translateMatrix);
	

	float pos[3]{};
	pos[0] = worldTransform_.translation_.x;
	pos[1] = worldTransform_.translation_.y;
	pos[2] = worldTransform_.translation_.z;

	ImGui::Begin("");
	ImGui::SliderFloat3("Player", pos, -500.0f, 500.0f);
	worldTransform_.translation_.x = pos[0];
	worldTransform_.translation_.y = pos[1];
	worldTransform_.translation_.z = pos[2];
	ImGui::End();

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix(); 

};

void Player::Draw(ViewProjection viewProjection){

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

};