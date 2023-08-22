﻿#pragma once

#include <cassert>
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "PlayerBullet.h"
#include <list>
#include "Collider.h"
#include "CollisionConfig.h"

class Player : public Collider {
public:
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model,uint32_t textureHandle);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

	//攻撃
	void Attack();

	Vector3 GetWorldPosition();

	//衝突を検証したら呼び出される関数
	void OnCollision();
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	//デストラクタ
	~Player();
	
private:

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//キーボード入力
	Input* input_ = nullptr;
	//ImGuiで値を入力する
	float inputFloat3[3] = {0, 0, 0};
	//弾
	//PlayerBullet* bullet_ = nullptr;
	//弾
	std::list<PlayerBullet*> bullets_;

	int count = 0;

};
