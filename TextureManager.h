#pragma once
#include "Engine.h"

class TextureManager
{
public:
	static TextureManager* GetInstance();

	void Initialize();

private:
	TextureManager() = default;
	~TextureManager() = default;

	MyEngine* engine_;

};

