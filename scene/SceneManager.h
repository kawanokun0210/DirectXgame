#pragma once
#include "GameScene.h"
#include "DirectX.h"
#include "Engine.h"
#include "TitleState.h"

enum GAMESCENE {
	TITLE,
	PLAY,
	CLEAR,
	OVER
};

class SceneManager
{
public:

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon);

	//タイトルシーン
	void TitleUpdate();

	void TitleDraw();

	//ゲームシーン
	void GameUpdate();

	void GameDraw();


	//全解放処理
	void Finalize();

public:

	GAMESCENE GetScene() { return sceneState; }

private:
	MyEngine* engine_;
	DirectXCommon* dxCommon_;

	GameScene* gameScene = new GameScene();
	TitleState* titleState = new TitleState();

	Input* input_ = new Input();

	GAMESCENE sceneState = TITLE;

};

