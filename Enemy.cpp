#include "Enemy.h"
#include <assert.h>

Enemy::~Enemy() {

	delete state;
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Attack() {

	timer--;

	if (timer < 0) {
		// 弾の速度
		const float kBulletSpeed = -1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		// 弾を登録
		// bullet_ = newBullet;
		bullets_.push_back(newBullet);

		timer = kShotInterval;

	}
}

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

	// デスフラグの立った球を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	state->Update();

	Attack();

	worldTransform_.UpdateMatrix();

	// 弾更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
}

void Enemy::Draw(const ViewProjection& view) {

	model_->Draw(worldTransform_, view, texturehandle_);

	// 弾の描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(view);
	}
}
