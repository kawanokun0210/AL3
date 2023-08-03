#include "PlayerBullet.h"
#include "newMath.h"
#include <assert.h>

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	assert(model);
	model_ = model;
	// テクスチャ読み込み
	texturehandle_ = TextureManager::Load("white1x1.png");

	// ワールドトランスフォーム
	worldTransform_.Initialize();
	// 引数で受け取った初期座標
	worldTransform_.translation_ = position;
	// 引数で受け取った速度をメンバに代入
	velocity_ = velocity;
}

Vector3 PlayerBullet::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void PlayerBullet::OnCollision() { isDead_ = true; }

void PlayerBullet::Update() {
	// 座標を移動
	worldTransform_.translation_.x = worldTransform_.translation_.x + velocity_.x;
	worldTransform_.translation_.y = worldTransform_.translation_.y + velocity_.y;
	worldTransform_.translation_.z = worldTransform_.translation_.z + velocity_.z;
	worldTransform_.UpdateMatrix();
	// 時間経過でデス
	if (--deathtimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& view) {
	model_->Draw(worldTransform_, view, texturehandle_);
}

// Vector3 PlayerBullet::Add(Vector3& a, Vector3& b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
