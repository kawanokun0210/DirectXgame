#pragma once
#include "DirectX.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "../Vertex.h"
#include "MatrixCalculation.h"
#include "Quaternion.h"
#include "../Camera.h"
#include "../Animation.h"

class MyEngine;

class Object
{
public:
	void Initialize(const std::string& directoryPath, const std::string& filename, bool isgltfFile);

	void Draw(const Vector4& material, const Transform& transform, uint32_t index, Camera* cameraTransform, const DirectionalLight& light, bool isLighting);

	void Finalize();

	Transform uvTransformSprite{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

private:
	void SettingVertex();

	void SettingColor();

	void SettingDictionalLight();

	void TransformMatrix();

	void CameraResource();

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

	Camera* camera_;

	Vector3 translate_;
	Quaternion rotate_;
	Vector3 scale_;
	Matrix4x4 localMatrix;

	float animationTimer = 0.0f;

	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource_;
	Vector3* cameraData_;
};

