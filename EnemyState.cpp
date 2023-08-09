#pragma once
#include "EnemyState.h"

void EnemyStateApproah::Update() {
	Vector3 appSpeed(0, 0, -0.2f);
	enemy_->SetPosition(appSpeed);
	if (enemy_->GetWT().translation_.z < 0.0f) {
		enemy_->ChangeState(new EnemyStateLeave);
	}
}

void EnemyStateLeave::Update() {
	Vector3 leaveSpeed(-0.2f, 0.2f, 0.2f);
	enemy_->SetPosition(leaveSpeed);
}