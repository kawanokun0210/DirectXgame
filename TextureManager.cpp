#include "TextureManager.h"

TextureManager* TextureManager::GetInstance(){
	static TextureManager instance;
	return &instance;
}

void TextureManager::Initialize() {
	engine_ = MyEngine::GetInstance();

	engine_->SettingTexture("Resource/uvChecker.png", 2);

	engine_->SettingTexture("Resource/fence/fence.png", 3);

	engine_->SettingTexture("Resource/circle.png", 4);

	engine_->SettingTexture("Resource/particle.png", 5);

}