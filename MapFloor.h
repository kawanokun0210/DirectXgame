#pragma once
#include "DirectX.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Vertex.h"
#include "MatrixCalculation.h"
#include "Object.h"
#include "Engine.h"

class MapFloor
{
public:
	//初期化処理
	void Initialize();
	//更新処理
	void Update();
	//描画処理
	void Draw(Camera* camera, DirectionalLight light);
	//解放処理
	void Finalize();

private:
	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	Object* model_;

};

