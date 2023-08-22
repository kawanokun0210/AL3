#pragma once
#include "Vector3.h"

class Collider {
public:
	// 半径を取得
	float GetRadius() const { return radius_; }
	// 半径を設定
	void SetRadius(float& Radius) { radius_ = Radius; }
	// 衝突時に呼ばれる関数
	virtual void OnCollision() = 0;
	virtual Vector3 GetWorldPosition() = 0;

	uint32_t GetCOllisionAttribute() { return collisionAttribute_; }
	void SetCollisionAttribute(uint32_t attribute) { collisionAttribute_ = attribute; }
	uint32_t GetCollosionMask() { return CollisionMask_; }
	void SetCollisionMask(uint32_t mask) { CollisionMask_ = mask; }

private:
	/// <summary>
	/// 半径
	/// 初期値は1.0f
	/// </summary>
	float radius_ = 1.0f;

	uint32_t collisionAttribute_ = 0xffffffff;
	uint32_t CollisionMask_ = 0xffffffff;

};