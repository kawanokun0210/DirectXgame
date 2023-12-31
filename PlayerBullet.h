#pragma once
#include "MatrixCalculation.h"
#include "Object.h"
#include "Engine.h"

class PlayerBullet
{
public:

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon);

	void Update();

	void Draw(Transform camera, DirectionalLight directionalLight);

	void Finalize();

public:

	bool IsDead() const { return isDead_; }

	void SetBullet(Transform player);

private:
	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	Object* object_;

	Transform bullet;

	Vector4 material;

	static const int32_t kLifeTime = 120;

	int32_t deathTimer_ = kLifeTime;

	bool isDead_ = false;

};

