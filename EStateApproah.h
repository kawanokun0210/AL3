#pragma once
#include "EnemyState.h"
#include "EStateLeave.h"

class Enemy;

class EnemyStateApproah : public EnemyState {

public:
	void Update();
};