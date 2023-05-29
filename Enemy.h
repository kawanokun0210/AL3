#pragma once
#include "newMath.h"
#include "Model.h"

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

	~Enemy() { delete state; }

	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& view);

	void ChangeState(EnemyState* newEnemyState);

	WorldTransform GetWT() { return worldTransform_; }

	void SetPosition(Vector3 speed);

private:

	WorldTransform worldTransform_;
	Model* model_;
	uint32_t texturehandle_;

	Phase phase_ = Phase::Approach;

	EnemyState* state; 

};
