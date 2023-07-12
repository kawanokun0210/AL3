#include "EnemyBullet.h"
#include <assert.h>
#include "newMath.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

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

	//Z方向に伸びた形状
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;

	//Y軸周り角度
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	Math::MakeRotateYMatrix(worldTransform_.rotation_.y);
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, velocity_.z);

}

void EnemyBullet::Update() {

	worldTransform_.UpdateMatrix();
	// 座標を移動
	worldTransform_.translation_ = Math::Add(worldTransform_.translation_, velocity_);

	// 時間経過でデス
	if (--deathtimer_ <= 0) {
		isDead_ = true;
	}

}

void EnemyBullet::Draw(const ViewProjection& view) {
	model_->Draw(worldTransform_, view, texturehandle_);
}
