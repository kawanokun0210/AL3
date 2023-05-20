#pragma once
#include "newMath.h"
#include "Model.h"

class Enemy {

public:

	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& view);

private:

	WorldTransform worldTransform_;
	Model* model_;
	uint32_t texturehandle_;

};
