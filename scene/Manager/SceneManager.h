#pragma once

#include <memory>
#include "BaseScene.h"
#include "GameTitleScene.h"
#include "GamePlayScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"

class SceneManager 
{
private:
	//シーンを保持するメンバ変数
	std::unique_ptr<BaseScene> sceneArr_[4];

	//度のステージを呼び出すかを管理する変数
	int currentSceneNo_;
	int prevSceneNo_;

	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	BaseScene* baseScene;
public:
	SceneManager(MyEngine* engine, DirectXCommon* dxCommon);
	~SceneManager();

	int Run();
};