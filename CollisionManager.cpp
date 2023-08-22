#include "CollisionManager.h"

void CollisionManager::CheckAllCollisions() {
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {

		std::list<Collider*>::iterator itrB = itrA;
		++itrB;
		for (; itrB != colliders_.end(); ++itrB) {
			CheckCollisionPair(*(itrA), *(itrB));
		}
	}
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	if (!(colliderA->GetCOllisionAttribute() & colliderB->GetCollosionMask()) ||
	    !(colliderB->GetCOllisionAttribute() & colliderA->GetCollosionMask())) {
		return;
	}
	Vector3 posA = colliderA->GetWorldPosition();
	Vector3 posB = colliderB->GetWorldPosition();
	float radA = colliderA->GetRadius();
	float radB = colliderB->GetRadius();
	Vector3 distance = {
	    (posB.x - posA.x) * (posB.x - posA.x), (posB.y - posA.y) * (posB.y - posA.y),
	    (posB.z - posA.z) * (posB.z - posA.z)};
	if ((radA + radB) * (radA + radB) >= distance.x + distance.y + distance.z) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}