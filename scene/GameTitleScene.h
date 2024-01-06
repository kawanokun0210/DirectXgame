#pragma once
#include "BaseScene.h"
#include "Input.h"
#include "MatrixCalculation.h"
#include "Quaternion.h"
#include "Sprite.h"
#include "../TextureManager.h"

class GameTitleScene : public BaseScene 
{
public:

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon) override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:
	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	Input* input_;

	Sprite* sprite_;

	SpriteData spriteData_;
	Transform spriteTransform_;

	TextureManager* textureManager_;

	DirectionalLight directionalLight_;

	int time;

	Vector3 from0;
	Vector3 to0;
	Vector3 from1;
	Vector3 to1;

	//MT
	Quaternion rotation;
	Matrix4x4 rotateMatrix;
	Vector3 rotateByQuaternion;
	Vector3 rotateByMatrix;
};