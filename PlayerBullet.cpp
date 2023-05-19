#include "PlayerBullet.h"
#include <assert.h>

void PlayerBullet::Initialize(Model* model, const Vector3& position) {

	assert(model);
	model_ = model;
	//テクスチャ読み込み
	texturehandle_ = TextureManager::Load("white1x1.png");

	//ワールドトランスフォーム
	worldTransform_.Initialize();
	//引数で受け取った初期座標
	worldTransform_.translation_ = position;

}

void PlayerBullet::Update() { 
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& view) {
	model_->Draw(worldTransform_, view, texturehandle_);
}
