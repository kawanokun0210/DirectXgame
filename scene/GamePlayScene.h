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
#include "Particle.h"
#include "BaseScene.h"
#include <random>
#include "../Camera.h"
#include "../SkyBox.h"
#include "../LevelEditor.h"

class GamePlayScene : public BaseScene
{
public:
	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:
	LevelEditor* level;

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

	SkyBox* skybox_;
	Transform skyboxTransform_;
	Vector4 skyboxMaterial_;
	Matrix4x4 skyboxMatrix_;

	Object* object_[3];
	Transform objectTransform_[3];
	Vector4 objectMaterial_[3];
	Matrix4x4 objectMatrix_;

	Transform cameraTransform_;

	uint32_t uvResourceNum_;
	uint32_t monsterBallResourceNum_;

	DirectionalLight directionalLight_;

	Sound* sound_;
	SoundData soundDataHandle_;

	Input* input_ = nullptr;

	Camera* camera_ = nullptr;

	Particle* particle;
	ParticleData particles[10];

	std::random_device seedGenerator;

	const float kDeltaTime = 1.0f / 60.0f;

	bool texture_;

	bool triangleDrawA_;
	bool triangleDrawB_;
	int sphereDraw_;
	int spriteDraw_;
	int objectDraw_;
};