#pragma once
#include "newMath.h"
#include "Model.h"
#include "EnemyState.h"

class Enemy;
class EnemyState;

enum class Phase {
	Approach,
	Leave,
};

class Enemy {

public:

	

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
