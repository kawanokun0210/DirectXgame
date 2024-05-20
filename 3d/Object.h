#pragma once
#include "DirectX.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "../Vertex.h"
#include "MatrixCalculation.h"
#include "Quaternion.h"
#include "../Camera.h"

class MyEngine;

struct KeyframeVector3 {
	Vector3 value;
	float time;
};

struct KeyframeQuaternion {
	Quaternion value;
	float time;
};

template <typename tValue>

struct Keyframe {
	float time;
	tValue value;
};

//using KeyframeVector3 = Keyframe<Vector3>;
//using KeyframeQuaternion = Keyframe<Quaternion>;

struct NodeAnimation {
	std::vector<KeyframeVector3>translate;
	std::vector<KeyframeQuaternion>rotate;
	std::vector<KeyframeVector3>scale;
};

//template<typename tValue>
//
//struct AnimationCurve {
//	std::vector<Keyframe<tValue>>keyframes;
//};
//
//struct NodeAnimation {
//	AnimationCurve<Vector3> translate;
//	AnimationCurve<Quaternion> rotate;
//	AnimationCurve<Vector3> scale;
//};

struct Animation {
	float duration;
	std::unordered_map<std::string, NodeAnimation>NodeAnimations;
};

class Object
{
public:
	void Initialize(const std::string& directoryPath, const std::string& filename);

	void Draw(const Vector4& material, const Transform& transform, uint32_t index, Camera* cameraTransform, const DirectionalLight& light, bool isLighting);

	void Finalize();

	Animation LoadAnimationFile(const std::string& directoryPath, const std::string& filename);

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

	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);

	//クォータニオンの線形補間
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);

	Vector3 translate_;
	Quaternion rotate_;
	Vector3 scale_;
	Matrix4x4 localMatrix;

	Animation animation;
	float animationTimer = 0.0f;

	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource_;
	Vector3* cameraData_;
};

