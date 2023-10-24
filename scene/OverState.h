#pragma once
#include "Engine.h"
#include "DirectX.h"
#include "Sprite.h"

class OverState
{
public:

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon);

	void Update();

	void Draw();

	void Finalize();

private:
	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	Sprite* sprite_[1];

	Transform cameraTransform_;

	Matrix4x4 worldMatrix_;

	DirectionalLight directionalLight_;

	SpriteData spriteData_;
	Transform spriteTransform_;

	uint32_t cubeResourceNum_;

};

