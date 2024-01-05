#pragma once
#include "BaseScene.h"
#include "Input.h"

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

	Input* input_;

	int time;

};