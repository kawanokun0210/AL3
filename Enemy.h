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

	void Func1();

	void Func2();

private:

	static void (Enemy::*spFuncTable[])();

	WorldTransform worldTransform_;
	Model* model_;
	uint32_t texturehandle_;

	Phase phase_ = Phase::Approach;

};
