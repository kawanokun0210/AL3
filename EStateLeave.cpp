#include "EStateLeave.h"

void EnemyStateLeave::Update() {
	Vector3 leaveSpeed(-0.2f, 0.2f, 0.2f);
	enemy_->SetPosition(leaveSpeed);
}