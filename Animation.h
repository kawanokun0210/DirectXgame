#pragma once
#include "DirectX.h"
#include "Vector2.h"
#include "../Vector3.h"
#include "Vector4.h"
#include "../Vertex.h"
#include "MatrixCalculation.h"
#include "Quaternion.h"
#include "../Camera.h"

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

struct AnimationData {
	float duration;
	std::unordered_map<std::string, NodeAnimation>NodeAnimations;
};

class Animation
{
public:
	static Animation* GetInstance();

	AnimationData LoadAnimationFile(const std::string& directoryPath, const std::string& filename);

	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);

	//クォータニオンの線形補間
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);

	AnimationData GetAnimation() { return animation; }

private:
	Animation() = default;
	~Animation() = default;

	AnimationData animation;

};

