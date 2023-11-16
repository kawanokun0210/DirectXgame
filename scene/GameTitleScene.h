#pragma once
#include "BaseScene.h"
#include "Input.h"
#include "MatrixCalculation.h"

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
	Matrix4x4 rotateMatrix0;
	Matrix4x4 rotateMatrix1;
	Matrix4x4 rotateMatrix2;

};