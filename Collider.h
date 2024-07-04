#pragma once
#include "MatrixCalculation.h"
#include "CollisionConfig.h"
#include <list>

class Collider
{
public: 
	virtual void OnCollision(Collider* collider) = 0;
 
	virtual Vector3 GetWorldPos() = 0;
 
	void SetRadius(Vector3 radius) { m_radius = radius; }
	Vector3 GetRadius()const { return m_radius; }
 
	virtual uint32_t GetCollisionAttribute() = 0;
	 
	virtual void SetCollisionAttribute(uint32_t collisionAttribute) = 0;
	 
	virtual uint32_t GetCollisionMask() = 0;
	
	virtual void SetCollisionMask(uint32_t collisionMask) = 0;

protected:
	uint32_t m_collisionAttribute = 0xffffffff;
	 
	uint32_t m_collisionMask = 0xffffffff;

	Vector3 m_radius = { 1.0f };


};