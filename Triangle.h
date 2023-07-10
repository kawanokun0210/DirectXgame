#pragma once
#include"DirectX.h"
#include"Vector4.h"

class CreateEngine;

class Triangle {

public:

	void Initialize(DirectX* dxCommon);

	void Draw(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& material);

	void Finalize();

private:

	void SettingVertex();

	void SettingColor();

private:

	CreateEngine* Engine;

	DirectX* dxCommon_;

	Vector4* vertexData_;

	Vector4* materialData_;

	ID3D12Resource* vertexResource_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	ID3D12Resource* materialResource_;

	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

};

