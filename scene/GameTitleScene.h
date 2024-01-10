#pragma once
#include "BaseScene.h"
#include "Input.h"
#include "MatrixCalculation.h"
#include "Quaternion.h"

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