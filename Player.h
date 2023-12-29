#pragma once
#include "MatrixCalculation.h"
#include "Object.h"
#include "Engine.h"
#include "Input.h"

class Player
{
public:

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon);

	void Update();

	void Draw(Transform camera, DirectionalLight directionalLight);

	void Finalize();

private:

	void Move();

private:
	Object* object_;
	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	Input* input_;

	Transform player;

	Vector4 material;

};

