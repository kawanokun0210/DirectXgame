#pragma once
#include "DirectX.h"
#include "MatrixCalculation.h"
#include "Vertex.h"

struct WorldTransform {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer_;
	// マッピング済みアドレス
	TransformationMatrix* constMap = nullptr;
	// ローカルスケール
	Vector3 scale_ = { 1, 1, 1 };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = { 0, 0, 0 };
	// ローカル座標
	Vector3 translation_ = { 0, 0, 0 };
	// ローカル → ワールド変換行列
	Matrix4x4 matWorld_;
	// 親となるワールド変換へのポインタ
	const WorldTransform* parent_ = nullptr;

	DirectXCommon* dxCommon_;

	~WorldTransform();
	
	void Initialize(DirectXCommon* dxCommon);
	
	void CreateConstBuffer();
	
	void Map();

	void TransferMatrix();

	void UpdateWorldMatrix();
};
