#pragma once
#include "BaseScene.h"
#include "Input.h"
#include "MatrixCalculation.h"
#include "Quaternion.h"
#include "Object.h"
#include "Particle.h"
#include <random>
#include "../Camera.h"

class GameTitleScene : public BaseScene 
{
public:

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon) override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:
	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	Input* input_;

	int time;
	
	//WP0
	Particle* particle_[2];

	Object* object_[2];
	Transform objectTransform_[2];
	Vector4 objectMaterial_[2];

	ParticleData particles[10];
	ParticleData particles_[10];

	DirectionalLight directionalLight_;

	Transform PTrans;

	Camera* camera_;

	std::random_device seedGenerator;
	
	//MT
	Vector3 from0;
	Vector3 to0;
	Vector3 from1;
	Vector3 to1;

	//MT
	Quaternion interpolate0;
	Quaternion interpolate1;
	Quaternion interpolate2;
	Quaternion interpolate3;
	Quaternion interpolate4;
};