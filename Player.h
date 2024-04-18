#pragma once
#include "MatrixCalculation.h"
#include "Object.h"
#include "Engine.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "../WorldTransform.h"

class Player
{
public:
	~Player();

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon);

	void Update();

	void Draw(Camera* camera, DirectionalLight directionalLight);

	void Finalize();

	WorldTransform GetPosition() { return player; }

private:

	void Move();

	void Gimmick();

private:
	Object* object_;
	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	Input* input_;

	WorldTransform player;

	Vector4 material;

};

