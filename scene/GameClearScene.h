#pragma once
#include "BaseScene.h"
#include "Input.h"
#include "Sprite.h"
#include "../TextureManager.h"

class GameClearScene : public BaseScene
{
public:

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon) override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:
	MyEngine* engine_;
	DirectXCommon* dxCommon_;

	Sprite* sprite_[6];
	SpriteData spriteData_[6];
	Transform spriteTransform_[6];

	TextureManager* textureManager_;

	DirectionalLight directionalLight_;

	Input* input_;

	int eachScoar[6] = {};

};