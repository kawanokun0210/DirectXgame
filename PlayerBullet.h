#pragma once
#include "MatrixCalculation.h"
#include "Object.h"
#include "Engine.h"
#include "../WorldTransform.h"

class PlayerBullet
{
public:
	~PlayerBullet();

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon);

	void Update();

	void Draw(Camera* camera, DirectionalLight directionalLight);

	void Finalize();

public:

	bool IsDead() const { return isDead_; }

	void SetBullet(WorldTransform player);

	WorldTransform GetBullet() { return bullet; }

private:
	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	Object* object_;

	WorldTransform bullet;

	Vector4 material;

	static const int32_t kLifeTime = 120;

	int32_t deathTimer_ = kLifeTime;

	bool isDead_ = false;

};

