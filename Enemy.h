#pragma once
#include "MatrixCalculation.h"
#include "Object.h"
#include "Engine.h"
#include <random>

class Enemy
{
public:
	~Enemy();

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon);

	void Update();

	void Draw(Transform camera, DirectionalLight directionalLight);

	void Finalize();

public:

	bool IsDead() const { return isDead_; }

	Transform GetPosition() { return enemy; }

	bool GetAlive() { return isAlive_; }

	bool SetAlive(bool a);

private:
	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	Object* object_;

	Transform enemy;

	Vector4 material;

	std::random_device seedGenerator;

	bool isDead_ = false;

	bool isAlive_;

};

