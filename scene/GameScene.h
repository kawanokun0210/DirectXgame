#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "../Vertex.h"
#include "Engine.h"
#include "Triangle.h"
#include "Sprite.h"
#include "Sphere.h"
#include "Object.h"
#include "Input.h"
#include "Sound.h"

struct AABB {
	Vector3 min;
	Vector3 max;
};

class GameScene
{
public:
	void Initialize(MyEngine* engine, DirectXCommon* dxCommon);

	void Update();

	void Draw();

	void Finalize();

	void TDInitialize(DirectXCommon* dxCommon, MyEngine* engine);

	void MatrixUpdate();

	static bool IsCollision(const AABB& aabb1, const AABB& aabb2);

private:

	AABB aabb1{

		.min{-1.0f * 0.0f,-1.0f * -1.0f,-1.0f * 0.0f},
		.max{1.0f * 0.5f,1.0f * 0.5f,1.0f * 0.5f},
	};

	AABB aabb2{
		.min{1.0f * -5.0f,1.0f * -2.8f,-1.0f * 0.0f},
		.max{-1.0f * 0.5f,1.0f * 0.5f,1.0f *0.5f},
	};

	MyEngine* engine_;
	DirectXCommon* dxCommon_;

	Triangle* triangle_[2];
	TriangleData triangleData_[2];
	Transform transform_[2];
	Matrix4x4 worldMatrix_;

	Sprite* sprite_[2];
	SpriteData spriteData_;
	Transform spriteTransform_;

	Sphere* sphere_;
	Transform sphereTransform_;
	Vector4 sphereMaterial_;
	Matrix4x4 sphereMatrix_;

	static const int kMaxObject = 10;

	Object* object_[kMaxObject];
	Transform objectTransform_[kMaxObject];
	Vector4 objectMaterial_[kMaxObject];
	Matrix4x4 objectMatrix_;

	Transform playerTransform_;
	Vector4 playerMaterial_;
	Matrix4x4 playerMatrix_;

	Transform cameraTransform_;

	uint32_t uvResourceNum_;
	uint32_t monsterBallResourceNum_;

	DirectionalLight directionalLight_;

	Sound* sound_;
	SoundData soundDataHandle_;

	Input* input_;

	bool texture_;

	bool drop;

	bool triangleDrawA_;
	bool triangleDrawB_;
	int sphereDraw_;
	int spriteDraw_;
	int objectDraw_;
};