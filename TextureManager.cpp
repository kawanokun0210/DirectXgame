#include "TextureManager.h"

void TextureManager::Initialize(MyEngine* engine) {
	engine_ = engine;

	//0と1番は使えない
	engine_->SettingTexture("Resource/uvChecker.png", 2);

	engine_->SettingTexture("Resource/skydome/skydomeTex.png", 3);

	engine_->SettingTexture("Resource/circle.png", 4);

	engine_->SettingTexture("Resource/particle.png", 5);

}