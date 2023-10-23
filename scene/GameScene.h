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

class GameScene
{
public:
	void Initialize(MyEngine* engine, DirectXCommon* dxCommon);

	void Update();

	void Draw();

	void Finalize();

	void TDInitialize(DirectXCommon * dxCommon, MyEngine * engine);

	void MatrixUpdate();

private:
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

	Object* object_;
	Transform objectTransform_;
	Vector4 objectMaterial_;
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

	bool triangleDrawA_;
	bool triangleDrawB_;
	int sphereDraw_;
	int spriteDraw_;
	int objectDraw_;
};