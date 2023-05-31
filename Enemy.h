#pragma once
#include "newMath.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include <list>
#include "EnemyBullet.h"


class Enemy;

enum class Phase {
	Approach,
	Leave,
};

class EnemyState {

protected:

	Enemy* enemy_ = nullptr;

public:

	virtual void SetEnemy(Enemy* enemy) { enemy_ = enemy; }
	virtual void Update(){};

};

class EnemyStateApproah : public EnemyState {

public:

	void Update();

};


class EnemyStateLeave : public EnemyState {

public:

	void Update();

};


class Enemy {

public:

	~Enemy();

	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& view);

	void ChangeState(EnemyState* newEnemyState);

	WorldTransform GetWT() { return worldTransform_; }

	void SetPosition(Vector3 speed);

	// 攻撃
	void Attack();

private:

	WorldTransform worldTransform_;
	Model* model_;
	uint32_t texturehandle_;

	Phase phase_ = Phase::Approach;

	EnemyState* state; 

	// 弾
	std::list<EnemyBullet*> bullets_;

	int count = 0;

	int timer = 0;

};
