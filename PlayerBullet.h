#pragma once
#include "Model.h"
#include "WorldTransform.h"

class PlayerBullet {
public:

	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t texturehandle_;

};
