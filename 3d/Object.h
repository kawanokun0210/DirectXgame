#pragma once
#include "DirectX.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "../Vertex.h"
#include "MatrixCalculation.h"
#include "Quaternion.h"
#include "../Camera.h"
#include <span>
#include <array>

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

//ここからスケルトン用
struct Joint {
	QuaternionTransform transform;
	Matrix4x4 localMatrix;
	Matrix4x4 skeletonSpaceMatrix;
	std::string name;
	std::vector<int32_t> children;
	int32_t index;
	std::optional<int32_t> parent;
};

struct Skeleton {
	int32_t root;
	std::unordered_map<std::string, int32_t> jointMap;
	std::vector<Joint> joints;
};

//Skinning
const uint32_t kNumMaxInfluence = 4;

struct VertexInfluence
{
	std::array<float, kNumMaxInfluence> weights;
	std::array<int32_t, kNumMaxInfluence> jointIndices;
};

struct WellForGPU
{
	Matrix4x4 skeletonSpaceMatrix;
	Matrix4x4 skeletonSpaceInverseTransposeMatrix;
};

struct SkinCluster {
	std::vector<Matrix4x4> inverseBindPoseMatrices;
	Microsoft::WRL::ComPtr<ID3D12Resource> influenceResource;
	D3D12_VERTEX_BUFFER_VIEW influenceBufferView;
	std::span<VertexInfluence> mappedInfluence;
	Microsoft::WRL::ComPtr<ID3D12Resource> paletteResource;
	std::span<WellForGPU> mappedPalette;
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> paletteSrvHandle;
};

class Object
{
public:
	static Object* Create(const std::string& filename, bool isAnimationFile, int index);

	void Initialize(const std::string& filename, bool isAnimationFile, int index);

	void Draw(const Vector4& material, const Transform& transform, uint32_t index, Camera* cameraTransform, const DirectionalLight& light, bool isLighting);

	void Finalize();

	Animation LoadAnimationFile(const std::string& filename);

	Transform uvTransformSprite{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	SkinCluster CreateSkinCluster(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const Skeleton& skeleton, const ModelData& modelData, const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, int index);

private:
	void SettingVertex();

	void SettingColor();

	void SettingDictionalLight();

	void TransformMatrix();

	void CameraResource();

	void SettingIndex();

	void SkinUpdate();

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

	//ここからスケルトン
	Skeleton CreateSkeleton(const Node& rootNode);
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);
	void SkeletonUpdate(Skeleton& skeleton);
	void ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime);
	//void NodeInitialize();

	//Node SResult;
	Animation animationData;
	Skeleton skeletonData;

	Vector3 translate_;
	Quaternion rotate_;
	Vector3 scale_;
	Matrix4x4 localMatrix;

	Animation animation;
	float animationTimer = 0.0f;
	bool isAnimationFile_;

	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource_;
	Vector3* cameraData_;

	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;
	uint32_t* indexData_ = nullptr;

	WellForGPU* paletteData_ = nullptr;

	SkinCluster skinCluster;

};

