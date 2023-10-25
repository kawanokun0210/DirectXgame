#include "TextureManager.h"

void TextureManager::Initialize(MyEngine* engine) {
	engine_ = engine;

	engine_->SettingTexture("Resource/uvChecker.png", 0);

	engine_->SettingTexture("Resource/cube.jpg", 1);

	engine_->SettingTexture("Resource/haikei.png", 2);

}

void TextureManager::Finalize() {
	delete engine_;
}