#include "Enemy.h"
#include <assert.h>

void Enemy::Initialize(Model* model, const Vector3& position) {

	assert(model);
	model_ = model;
	texturehandle_ = TextureManager::Load("beam.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	state = new EnemyStateApproah();
	state->SetEnemy(this);

}

void Enemy::ChangeState(EnemyState* newEnemyState) { 
	delete state;

	state = newEnemyState;
	state->SetEnemy(this);

}

void Enemy::SetPosition(Vector3 speed) {
	worldTransform_.translation_ = Add(worldTransform_.translation_, speed);
}

void EnemyStateApproah::Update() { 
	Vector3 appSpeed(0, 0, -0.2f);
	enemy_->SetPosition(appSpeed);
	if (enemy_->GetWT().translation_.z < 0.0f) {
		enemy_->ChangeState(new EnemyStateLeave);
	}
}

void EnemyStateLeave::Update() {
	Vector3 leaveSpeed(-0.2f, 0.2f, 0.2f);
	enemy_->SetPosition(leaveSpeed);
}

void Enemy::Update() {

	state->Update();

	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& view) {

	model_->Draw(worldTransform_, view, texturehandle_);

}
