#include "TextureManager.h"

void TextureManager::Initialize(MyEngine* engine) {
	engine_ = engine;

	//プレイヤーとゴールのテクスチャ
	engine_->SettingTexture("Resource/texture/uvChecker.png", 0);

	//ステージのリソース
	engine_->SettingTexture("Resource/texture/cube.jpg", 1);

	//タイトルの画像
	engine_->SettingTexture("Resource/texture/title.png", 2);

	//クリア画面
	engine_->SettingTexture("Resource/texture/clear.png", 3);

	//ゲームオーバー
	engine_->SettingTexture("Resource/texture/over.png", 4);

	//ゴールのテクスチャ
	engine_->SettingTexture("Resource/texture/uvChecker.png", 5);
}

void TextureManager::Finalize() {
	delete engine_;
}