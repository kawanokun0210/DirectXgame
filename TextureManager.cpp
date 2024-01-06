#include "TextureManager.h"

void TextureManager::Initialize(MyEngine* engine) {
	engine_ = engine;

	//0と1番は使えない
	engine_->SettingTexture("Resource/uvChecker.png", 2);

	engine_->SettingTexture("Resource/skydome/skydomeTex.png", 3);

	engine_->SettingTexture("Resource/circle.png", 4);

	engine_->SettingTexture("Resource/particle.png", 5);

	//ここからナンバーズ
	engine_->SettingTexture("Resource/numbers/count1.png", 6);
	engine_->SettingTexture("Resource/numbers/count2.png", 7);
	engine_->SettingTexture("Resource/numbers/count3.png", 8);
	engine_->SettingTexture("Resource/numbers/count4.png", 9);
	engine_->SettingTexture("Resource/numbers/count5.png", 10);
	engine_->SettingTexture("Resource/numbers/count6.png", 11);
	engine_->SettingTexture("Resource/numbers/count7.png", 12);
	engine_->SettingTexture("Resource/numbers/count8.png", 13);
	engine_->SettingTexture("Resource/numbers/count9.png", 14);
	engine_->SettingTexture("Resource/numbers/count10.png", 15);

	//ここからシーン別
	engine_->SettingTexture("Resource/skyTitle.png", 16);
	engine_->SettingTexture("Resource/skyTitle2.png", 17);

}