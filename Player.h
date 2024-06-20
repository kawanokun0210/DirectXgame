#pragma once
#include "Object.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "../Vertex.h"
#include "Engine.h"
#include "Input.h"
#include "Camera.h"

class Player
{
public:
	//初期化処理
	void Initialize();
	//更新処理
	void Update();
	//描画処理
	void Draw(Camera* camera, DirectionalLight directionalLight);
	//解放処理
	void Finalize();

private://ここは関数のみ
	//移動用の関数
	void Move();

	//ジャンプ用の関数
	void Jump();

private://ここは変数のみ
	//基本的に必要なもの
	Object* model_;
	Transform transform_;
	
	//input用の変数
	Input* input_;

	//移動用の変数
	float speed_ = 0.01f;

	//ジャンプ用の変数
	bool isJump_;
	float fallSpeed_;
	float acceleration_;

};

