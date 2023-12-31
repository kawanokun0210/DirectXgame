#include "TextureManager.h"

void TextureManager::Initialize(MyEngine* engine) {
	engine_ = engine;

	//0と1番は使えない
	engine_->SettingTexture("Resource/uvChecker.png", 2);

	engine_->SettingTexture("Resource/skydome/skydomeTex.png", 3);

}