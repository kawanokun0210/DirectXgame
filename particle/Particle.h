#pragma once
#include "DirectX.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "../Vertex.h"
#include "WinApp.h"
#include "MatrixCalculation.h"
#include "../externals/DirectXTex/DirectXTex.h"
#include <dxcapi.h>
#include <cstdlib>
#include <dxgi1_6.h>
#pragma comment(lib,"dxcompiler.lib")

class MyEngine;

struct ParticleData {
	Transform transform;
	Vector3 speed;
	Vector4 color;
};

class Particle
{
public:
	void Initialize(DirectXCommon* dxCommon, MyEngine* engine, const std::string& directoryPath, const std::string& filename);

	void Draw(const Vector4& material, ParticleData* transforms, uint32_t index, const Transform& cameraTransform, const DirectionalLight& light);

	void Finalize();

	void SettingInstance();

	Transform uvTransformSprite{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_;

	TransformationMatrix* GetInstancingData() { return instancingData; }

private:
	void SettingVertex();

	void SettingColor();

	void SettingDictionalLight();

	void TransformMatrix();

	void SetInstance();

private:
	DirectXCommon* dxCommon_;
	MyEngine* engine_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	VertexData* vertexData_;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_;
	TransformationMatrix* wvpData_;

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;

	DirectionalLight* directionalLight_;
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_;

	Matrix4x4 uvTransformMatrix;

	ModelData modelData;

	uint32_t instanceCount = 10;

	const uint32_t kNumInstance = 10;

	TransformationMatrix* instancingData;
	
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;
};

