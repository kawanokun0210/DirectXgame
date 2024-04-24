#pragma once
#include "MatrixCalculation.h"
#include "Object.h"
#include "Engine.h"
#include "../WorldTransform.h"

class EnemyBullet
{
public:
	~EnemyBullet();

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon);

	void Update();

	void Draw(Camera* camera, DirectionalLight directionalLight);

	void Finalize();

public:

	bool IsDead() const { return isDead_; }

	void SetBullet(Vector3 enemy);

	Transform GetBullet() { return bullet; }

	void SetSpeed(Vector3 speed);

private:
	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	Object* object_;

	Transform bullet;

	Vector4 material;

	Vector3 speed_;

	static const int32_t kLifeTime = 120;

	int32_t deathTimer_ = kLifeTime;

	bool isDead_ = false;

};

