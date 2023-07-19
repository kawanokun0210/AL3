#pragma once
#include "Model.h"
#include <cassert>
#include "WorldTransform.h"

class Skydome {
public:
	
	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(ViewProjection& view);

private:
	//ワールド返還データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

};
