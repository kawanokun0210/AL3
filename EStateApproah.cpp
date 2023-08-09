#include "EStateApproah.h"

void EnemyStateApproah::Update() {
	Vector3 appSpeed(0, 0, -0.2f);
	enemy_->SetPosition(appSpeed);
	if (enemy_->GetWT().translation_.z < 0.0f) {
		enemy_->ChangeState(new EnemyStateLeave);
	}
}