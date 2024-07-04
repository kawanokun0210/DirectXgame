#include "CollisionManager.h"
#include "MatrixCalculation.h"

CollisionManager::CollisionManager() { this->Initialize(); }

CollisionManager::~CollisionManager() {}

void CollisionManager::Initialize() {}

void CollisionManager::Update() { this->CheckAllCollisions(); }

void CollisionManager::Draw() {}

void CollisionManager::CheckAllCollisions() {

	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = m_colliders.begin();
	for (; itrA != m_colliders.end(); ++itrA) {

		// イテレーターからコライダーを取得
		Collider* colliderA = (*itrA);

		// このイテレーターは前のイテレーターの次の要素から回す
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != m_colliders.end(); ++itrB) {

			// イテレーターからコライダーを取得
			Collider* colliderB = (*itrB);

			// 当たり判定
			CheckCollisionPair(colliderA, colliderB);

		}
	}
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {

	// 衝突フィルタリング
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0) {
		// 属性があっていなければ早期リターン
		return;
	}

	// コライダーの種類を確認


	// コライダーのワールド座標を取得
	Vector3 posA = colliderA->GetWorldPos();
	Vector3 radA = colliderA->GetRadius();
	Vector3 posB = colliderB->GetWorldPos();
	Vector3 radB = colliderB->GetRadius();

	// 距離の判定
	if ((posA.x + radA.x) > (posB.x - radB.x) && (posA.x - radA.x) < (posB.x + radB.x) &&
		(posA.y + radA.y) > (posB.y - radB.y) && (posA.y - radA.y) < (posB.y + radB.y) &&
		(posA.z + radA.z) > (posB.z - radB.z) && (posA.z - radA.z) < (posB.z + radB.z)) {

		colliderA->OnCollision(colliderB);
		colliderB->OnCollision(colliderA);

	}
}
