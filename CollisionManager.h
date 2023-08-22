#pragma once
#include "Collider.h"
#include <list>

class CollisionManager {
private:
	std::list<Collider*> colliders_;

public:
	void AddCollider(Collider* collider) { colliders_.push_back(collider); }
	void ClearCollider() { colliders_.clear(); }
	void CheckAllCollisions();
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
};