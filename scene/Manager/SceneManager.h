#pragma once

#include <memory>
#include "BaseScene.h"
#include "TitleScene.h"
#include "GameScene.h"

class SceneManager 
{
private:
	//シーンを保持するメンバ変数
	std::unique_ptr<BaseScene> sceneArr_[4];

	//度のステージを呼び出すかを管理する変数
	int currentSceneNo_;
	int prevSceneNo_;

	MyEngine* engine_ = new MyEngine();
	DirectXCommon* dxCommon_ = new DirectXCommon();
	BaseScene* baseScene;
public:
	SceneManager(const wchar_t kWindowTitle[], int32_t Hight, int32_t Right);
	~SceneManager();

	int Run();
};