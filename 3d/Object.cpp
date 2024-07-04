#include "Object.h"
#include "Engine.h"
#include <cmath>

void Object::Initialize(const std::string& directoryPath, const std::string& filename, bool isAnimationFile, int index)
{
	dxCommon_ = DirectXCommon::GetInstance();
	engine_ = MyEngine::GetInstance();
	modelData = engine_->LoadObjFile(directoryPath, filename);
	isAnimationFile_ = isAnimationFile;
	if (isAnimationFile == true) {
		animationData = LoadAnimationFile(directoryPath, filename);
		//NodeInitialize();
		skeletonData = CreateSkeleton(modelData.rootNode);
		CreateSkinCluster(dxCommon_->GetDevice(), skeletonData, modelData, dxCommon_->GetSrvDescriptiorHeap(), engine_->GetdescriptorSizeSRV(), index);
	}
	SettingVertex();
	SettingColor();
	SettingDictionalLight();
	TransformMatrix();
	CameraResource();
	SettingIndex();
}

void Object::Draw(const Vector4& material, const Transform& transform, uint32_t index, Camera* cameraTransform, const DirectionalLight& light, bool isLighting)
{
	camera_ = cameraTransform;

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(camera_->GetTransform().scale, camera_->GetTransform().rotate, camera_->GetTransform().translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 scaleMatrix = Inverse(worldMatrix);
	Matrix4x4 worldInverseTranspose = Transpose(scaleMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(dxCommon_->GetWin()->kClientWidth) / float(dxCommon_->GetWin()->kClientHeight), 0.1f, 100.0f);

	Matrix4x4 wvpMatrix_ = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	uvTransformMatrix = MakeScaleMatrix(uvTransformSprite.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZmatrix(uvTransformSprite.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformSprite.translate));

	if (isAnimationFile_ == true) {
		animationTimer += 1.0f / 60.0f;
		animationTimer = std::fmod(animationTimer, animation.duration);
		/*NodeAnimation& rootNodeAnimation = animation.NodeAnimations[modelData.rootNode.name];
		translate_ = CalculateValue(rootNodeAnimation.translate, animationTimer);
		rotate_ = CalculateValue(rootNodeAnimation.rotate, animationTimer);
		scale_ = CalculateValue(rootNodeAnimation.scale, animationTimer);
		localMatrix = MakeAffineMatrix(scale_, rotate_, translate_);*/

		ApplyAnimation(skeletonData, animationData, animationTimer);
		SkeletonUpdate(skeletonData);
		SkinUpdate();

		*materialData_ = { material,isLighting };
		materialData_->uvTransform = uvTransformMatrix;
		*wvpData_ = { wvpMatrix_,worldMatrix,worldInverseTranspose };
		wvpData_->WVP = wvpMatrix_;
		wvpData_->World = worldMatrix;
		*directionalLight_ = light;
		materialData_->shininess = 50.0f;
		*cameraData_ = camera_->GetTransform().translate;
	}
	else {
		*materialData_ = { material,isLighting };
		materialData_->uvTransform = uvTransformMatrix;
		*wvpData_ = { wvpMatrix_,worldMatrix,scaleMatrix };
		wvpData_->WVP = Multiply(modelData.rootNode.localMatrix, wvpMatrix_);
		wvpData_->World = Multiply(modelData.rootNode.localMatrix, worldMatrix);
		*directionalLight_ = light;
		materialData_->shininess = 50.0f;
		*cameraData_ = camera_->GetTransform().translate;
	}

	//RootSignatureを設定。PS0とは別途設定が必要
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(engine_->GetRootSignature().Get());
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(engine_->GetRootSignature3().Get());

	//PS0を設定
	dxCommon_->GetCommandList()->SetPipelineState(engine_->GetGraphicsPipelineState().Get());
	dxCommon_->GetCommandList()->SetPipelineState(engine_->GetGraphicsPipelineState3().Get());

	//VBVを設定
	D3D12_VERTEX_BUFFER_VIEW vbvs[2] = {
			vertexBufferView,
			skinCluster.influenceBufferView
	};
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 2, vbvs);
	//index
	dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

	//形状を設定。PS0に設定しているものとはまた別。同じものを設定する
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//マテリアルCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(6, camera_->GetConstBuffer()->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(5, cameraResource_->GetGPUVirtualAddress());

	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]のこと
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, engine_->textureSrvHandleGPU_[index]);
	if (isAnimationFile_ == true) {
		dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(4, skinCluster.paletteSrvHandle.second);
	}

	//描画
	//dxCommon_->GetCommandList()->DrawInstanced(vertexCount, 1, 0, 0);
	if (isAnimationFile_ == true) {
		dxCommon_->GetCommandList()->DrawIndexedInstanced(UINT(modelData.indices.size()), 1, 0, 0, 0);
	}
	else {
		dxCommon_->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);
	}
}

Animation Object::LoadAnimationFile(const std::string& directoryPath, const std::string& filename) {
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);
	assert(scene->mNumAnimations != 0);
	aiAnimation* animationAssimp = scene->mAnimations[0];
	animation.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);

	for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
		aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
		NodeAnimation& nodeAnimation = animation.NodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
			KeyframeVector3 keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
			keyframe.value = { -keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };
			nodeAnimation.translate.push_back(keyframe);
		}

		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex)
		{
			aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
			KeyframeQuaternion keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
			keyframe.value = { keyAssimp.mValue.x,-keyAssimp.mValue.y,-keyAssimp.mValue.z,keyAssimp.mValue.w };
			nodeAnimation.rotate.push_back(keyframe);
		}

		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex)
		{
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
			KeyframeVector3 keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
			keyframe.value = { keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };
			nodeAnimation.scale.push_back(keyframe);
		}


	}

	return animation;
}

void Object::Finalize()
{
	/*vertexResource->Release();
	materialResource_->Release();
	directionalLightResource_->Release();
	wvpResource_->Release();*/
}

SkinCluster Object::CreateSkinCluster(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const Skeleton& skeleton, const ModelData& modelData, const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, int index) {
	//palette用のResourceを確保
	skinCluster.paletteResource = dxCommon_->CreateBufferResource(device, sizeof(WellForGPU) * skeleton.joints.size());
	skinCluster.paletteResource->Map(0, nullptr, reinterpret_cast<void**>(&paletteData_));
	skinCluster.mappedPalette = { paletteData_,skeleton.joints.size() };
	skinCluster.paletteSrvHandle.first = engine_->GetCPUDescriptorHandle(dxCommon_->GetSrvDescriptiorHeap(), engine_->GetdescriptorSizeSRV(), index);
	skinCluster.paletteSrvHandle.second = engine_->GetGPUDescriptorHandle(dxCommon_->GetSrvDescriptiorHeap(), engine_->GetdescriptorSizeSRV(), index);

	//palette用のSRVを作成
	D3D12_SHADER_RESOURCE_VIEW_DESC paletteSrvDesc{};
	paletteSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	paletteSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	paletteSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	paletteSrvDesc.Buffer.FirstElement = 0;
	paletteSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	paletteSrvDesc.Buffer.NumElements = UINT(skeleton.joints.size());
	paletteSrvDesc.Buffer.StructureByteStride = sizeof(WellForGPU);
	dxCommon_->GetDevice()->CreateShaderResourceView(skinCluster.paletteResource.Get(), &paletteSrvDesc, skinCluster.paletteSrvHandle.first);

	//influence用のResourceを確保
	skinCluster.influenceResource = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexInfluence) * modelData.vertices.size());
	VertexInfluence* mappedInfluence = nullptr;
	skinCluster.influenceResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
	std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * modelData.vertices.size());
	skinCluster.mappedInfluence = { mappedInfluence,modelData.vertices.size() };

	//Influence用のVBVを作成
	skinCluster.influenceBufferView.BufferLocation = skinCluster.influenceResource->GetGPUVirtualAddress();
	skinCluster.influenceBufferView.SizeInBytes = UINT(sizeof(VertexInfluence) * modelData.vertices.size());
	skinCluster.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);

	//InverseBindPoseMatrixを格納する場所を作成して、単位行列で埋める
	skinCluster.inverseBindPoseMatrices.resize(skeleton.joints.size());
	std::generate(skinCluster.inverseBindPoseMatrices.begin(), skinCluster.inverseBindPoseMatrices.end(), MakeIdentity4x4);

	for (const auto& jointWeight : modelData.skinClusterData) {
		auto it = skeleton.jointMap.find(jointWeight.first);
		if (it == skeleton.jointMap.end()) {
			continue;
		}
		skinCluster.inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
		for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
			auto& currentInfluence = skinCluster.mappedInfluence[vertexWeight.vertexIndex];
			for (uint32_t index = 0; index < kNumMaxInfluence; ++index) {
				if (currentInfluence.weights[index] == 0.0f) {
					currentInfluence.weights[index] = vertexWeight.weight;
					currentInfluence.jointIndices[index] = (*it).second;
					break;
				}
			}
		}
	}

	return skinCluster;
}

void Object::SkinUpdate() {
	for (size_t jointIndex = 0; jointIndex < skeletonData.joints.size(); ++jointIndex) {
		assert(jointIndex < skinCluster.inverseBindPoseMatrices.size());
		skinCluster.mappedPalette[jointIndex].skeletonSpaceMatrix = Multiply(skinCluster.inverseBindPoseMatrices[jointIndex], skeletonData.joints[jointIndex].skeletonSpaceMatrix);
		skinCluster.mappedPalette[jointIndex].skeletonSpaceInverseTransposeMatrix = Transpose(Inverse(skinCluster.mappedPalette[jointIndex].skeletonSpaceMatrix));
	}
}

void Object::SettingVertex()
{
	//vertexResource = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * vertexCount);
	vertexResource = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * modelData.vertices.size());
	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();

	//vertexBufferView.SizeInBytes = sizeof(VertexData) * vertexCount;
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());

	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	std::memcpy(vertexData_, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
}

void Object::TransformMatrix()
{
	wvpResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(TransformationMatrix));
	wvpResource_->Map(0, NULL, reinterpret_cast<void**>(&wvpData_));
	wvpData_->WVP = MakeIdentity4x4();
}

void Object::SettingColor()
{
	materialResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	materialData_->uvTransform = MakeIdentity4x4();
}

void Object::SettingDictionalLight()
{
	directionalLightResource_ = DirectXCommon::CreateBufferResource(dxCommon_->GetDevice(), sizeof(DirectionalLight));
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLight_));
}

void Object::CameraResource() {
	cameraResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(CameraForGPU));

	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
}

//クォータニオンの線形補間
Quaternion Object::CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time)
{
	assert(!keyframes.empty());	//キーがないものは返す値がわからないのでダメ
	if (keyframes.size() == 1 || time <= keyframes[0].time)
	{
		return keyframes[0].value;
	}
	for (size_t index = 0; index < keyframes.size() - 1; ++index)
	{
		size_t nextIndex = index + 1;
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time)
		{
			//範囲内を補完する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Slerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}

	return (*keyframes.begin()).value;
}

Vector3 Object::CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time) {
	assert(!keyframes.empty());
	if (keyframes.size() == 1 || time <= keyframes[0].time) {
		return keyframes[0].value;
	}

	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}
	return (*keyframes.rbegin()).value;
}

int32_t Object::CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints) {
	Joint joint;
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix = MakeIdentity4x4();
	joint.transform = node.transform;
	joint.index = int32_t(joints.size());
	joint.parent = parent;
	joints.push_back(joint);
	for (const Node& child : node.children) {
		int32_t childIndex = CreateJoint(child, joint.index, joints);
		joints[joint.index].children.push_back(childIndex);
	}
	return joint.index;
}

Skeleton Object::CreateSkeleton(const Node& rootNode) {
	Skeleton skeleton;
	skeleton.root = CreateJoint(rootNode, {}, skeleton.joints);

	for (const Joint& joint : skeleton.joints) {
		skeleton.jointMap.emplace(joint.name, joint.index);
	}

	return skeleton;
}

void Object::SkeletonUpdate(Skeleton& skeleton) {
	int i = 0;
	for (Joint& joint : skeleton.joints) {
		joint.localMatrix = MakeAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);
		if (joint.parent) {
			joint.skeletonSpaceMatrix = Multiply(joint.localMatrix, skeleton.joints[*joint.parent].skeletonSpaceMatrix);
		}
		else {
			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
		++i;
	}
}

void Object::ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime) {
	for (Joint& joint : skeleton.joints) {
		if (auto it = animation.NodeAnimations.find(joint.name); it != animation.NodeAnimations.end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			joint.transform.translate = CalculateValue(rootNodeAnimation.translate, animationTime);
			joint.transform.rotate = CalculateValue(rootNodeAnimation.rotate, animationTime);
			joint.transform.scale = CalculateValue(rootNodeAnimation.scale, animationTime);
		}
	}
}

//void Object::NodeInitialize() {
//	aiVector3D scale, translete;
//	aiQuaternion rotate;
//	aiNode node;
//	node.mTransformation.Decompose(scale, rotate, translete);
//	SResult.children = modelData.rootNode.children;
//	SResult.transform.scale = { scale.x,scale.y,scale.z };
//	SResult.transform.rotate = { rotate.x,-rotate.y,-rotate.z,rotate.w };
//	SResult.transform.translate = { -translete.x,translete.y,translete.z };
//	SResult.localMatrix = MakeAffineMatrix(SResult.transform.scale, SResult.transform.rotate, SResult.transform.translate);
//}

void Object::SettingIndex() {
	indexResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(uint32_t) * modelData.indices.size());

	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * UINT(modelData.indices.size());
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	std::memcpy(indexData_, modelData.indices.data(), sizeof(uint32_t) * modelData.indices.size());
}