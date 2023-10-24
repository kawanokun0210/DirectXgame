#pragma once
#include "Engine.h"

class TextureManager
{
public:

	void Initialize(MyEngine* engine);

	void Finalize();

private:
	MyEngine* engine_;

	uint32_t uvResourceNum_;
	uint32_t cubeResourceNum_;

};

