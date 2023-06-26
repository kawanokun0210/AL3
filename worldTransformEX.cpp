#include "WorldTransform.h"
#include "newMath.h"

    void WorldTransform::UpdateMatrix() {
	matWorld_ = Math::MakeAffineMatrix(scale_, rotation_, translation_);
	TransferMatrix();
}