#include "WorldTransform.h"

WorldTransform::~WorldTransform()
{
}

void WorldTransform::Initialize(DirectXCommon* dxCommon)
{
	dxCommon_ = dxCommon;
	matWorld_ = MakeIdentity4x4();
	CreateConstBuffer();
	Map();
	TransferMatrix();

}


void WorldTransform::CreateConstBuffer()
{
	if (dxCommon_) {
		constBuffer_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(TransformationMatrix));
	}
	else {
		assert(dxCommon_);
	}
}

void WorldTransform::Map()
{
	constBuffer_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&constMap));
}

void WorldTransform::TransferMatrix()
{
	constMap->matWorld = matWorld_;
}

void WorldTransform::UpdateWorldMatrix()
{
	Matrix4x4 AffineMatrix = MakeAffineMatrix(scale_, rotation_, translation_);
	matWorld_ = AffineMatrix;
	//親があれば親のワールド行列を掛ける
	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}

	TransferMatrix();
}