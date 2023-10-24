#include "TextureManager.h"

void TextureManager::Initialize(MyEngine* engine) {
	engine_ = engine;

	uvResourceNum_ = 0;
	engine_->SettingTexture("Resource/uvChecker.png", uvResourceNum_);

	cubeResourceNum_ = 1;
	engine_->SettingTexture("Resource/cube.jpg", cubeResourceNum_);

}

void TextureManager::Finalize() {
	delete engine_;
}