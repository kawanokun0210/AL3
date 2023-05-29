#pragma once
#include "newMath.h"
#include "Model.h"

enum class Phase {
	Approach,
	Leave,
};

class Enemy {

public:

	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& view);

	void ApproachMove();

	void LeaveMove();

private:

	WorldTransform worldTransform_;
	Model* model_;
	uint32_t texturehandle_;

	Phase phase_ = Phase::Approach;

	static void (Enemy::*spFuncTable[])();

	void(Enemy::*pApproachMove)();


};
