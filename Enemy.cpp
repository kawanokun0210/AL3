#include "Enemy.h"
#include <assert.h>

void Enemy::Initialize(Model* model, const Vector3& position) {

	assert(model);
	model_ = model;
	texturehandle_ = TextureManager::Load("beam.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

}

void Enemy::ApproachMove() {

	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;

	move.z -= kCharacterSpeed;
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	worldTransform_.UpdateMatrix();

	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

}

void Enemy::LeaveMove() {

	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;

	move.x += kCharacterSpeed;
	move.y += kCharacterSpeed;
	move.z += kCharacterSpeed;
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	worldTransform_.UpdateMatrix();

	if (worldTransform_.translation_.z < 30.0f) {
		phase_ = Phase::Leave;
	}

}

void Enemy::Update() {

	switch (phase_) {
	case Phase::Approach:

		Enemy::ApproachMove();

		break;

	case Phase::Leave:

		Enemy::LeaveMove();

		break;
	
	}

}

void Enemy::Draw(const ViewProjection& view) {

	model_->Draw(worldTransform_, view, texturehandle_);

}
