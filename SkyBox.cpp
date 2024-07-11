#include "SkyBox.h"
#include "Engine.h"
#include <cmath>

void SkyBox::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	engine_ = MyEngine::GetInstance();
	SettingVertex();
	SettingColor();
	SettingDictionalLight();
	TransformMatrix();
	CameraResource();
	SettingIndex();

}

void SkyBox::Draw(const Vector4& material, const Transform& transform, uint32_t index, Camera* cameraTransform, const DirectionalLight& light)
{
	camera_ = cameraTransform;

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(camera_->GetTransform().scale, camera_->GetTransform().rotate, camera_->GetTransform().translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 scaleMatrix = Inverse(worldMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(dxCommon_->GetWin()->kClientWidth) / float(dxCommon_->GetWin()->kClientHeight), 0.1f, 100.0f);

	Matrix4x4 wvpMatrix_ = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	uvTransformMatrix = MakeScaleMatrix(uvTransformSprite.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZmatrix(uvTransformSprite.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformSprite.translate));

	*cameraData_ = camera_->GetTransform().translate;

	//右面
	indexDataSprite[0] = 0; indexDataSprite[1] = 1; indexDataSprite[2] = 2;
	indexDataSprite[3] = 2; indexDataSprite[4] = 1; indexDataSprite[5] = 3;
	//左面
	indexDataSprite[6] = 4; indexDataSprite[7] = 5; indexDataSprite[8] = 6;
	indexDataSprite[9] = 6; indexDataSprite[10] = 5; indexDataSprite[11] = 7;
	//前面
	indexDataSprite[12] = 8; indexDataSprite[13] = 9; indexDataSprite[14] = 10;
	indexDataSprite[15] = 10; indexDataSprite[16] = 9; indexDataSprite[17] = 11;
	//後面
	indexDataSprite[18] = 12; indexDataSprite[19] = 14; indexDataSprite[20] = 13;
	indexDataSprite[21] = 14; indexDataSprite[22] = 15; indexDataSprite[23] = 13;
	//上面
	indexDataSprite[24] = 16; indexDataSprite[25] = 17; indexDataSprite[26] = 18;
	indexDataSprite[27] = 18; indexDataSprite[28] = 17; indexDataSprite[29] = 19;
	//下面
	indexDataSprite[30] = 20; indexDataSprite[31] = 22; indexDataSprite[32] = 21;
	indexDataSprite[33] = 22; indexDataSprite[34] = 23; indexDataSprite[35] = 21;

	//右面
	vertexData_[0].position = { 1.0f,1.0f,1.0f,1.0f };
	vertexData_[1].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData_[2].position = { 1.0f,-1.0f,1.0f,1.0f };
	vertexData_[3].position = { 1.0f,-1.0f,-1.0f,1.0f };
	//左面
	vertexData_[4].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData_[5].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData_[6].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertexData_[7].position = { -1.0f,-1.0f,1.0f,1.0f };
	//前面
	vertexData_[8].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData_[9].position = { 1.0f,1.0f,1.0f,1.0f };
	vertexData_[10].position = { -1.0f,-1.0f,1.0f,1.0f };
	vertexData_[11].position = { 1.0f,-1.0f,1.0f,1.0f };
	//後面
	vertexData_[12].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData_[13].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData_[14].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertexData_[15].position = { 1.0f,-1.0f,-1.0f,1.0f };
	//上面
	vertexData_[16].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData_[17].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData_[18].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData_[19].position = { 1.0f,1.0f,1.0f,1.0f };
	//下面
	vertexData_[20].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertexData_[21].position = { 1.0f,-1.0f,-1.0f,1.0f };
	vertexData_[22].position = { -1.0f,-1.0f,1.0f,1.0f };
	vertexData_[23].position = { 1.0f,-1.0f,1.0f,1.0f };

	vertexData_->normal = { 0.0f,0.0f,0.0f };
	vertexData_->texcoord = { 0.0f,0.0f };

	*materialData_ = { material,true, };
	materialData_->uvTransform = uvTransformMatrix;
	*wvpData_ = { wvpMatrix_,worldMatrix,scaleMatrix };
	*directionalLight_ = light;
	materialData_->shininess = 50.0f;

	//RootSignatureを設定。PS0とは別途設定が必要
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(engine_->GetRootSignature4().Get());

	//PS0を設定
	dxCommon_->GetCommandList()->SetPipelineState(engine_->GetGraphicsPipelineState4().Get());

	//VBVを設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	//ind
	dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite);

	//形状を設定。PS0に設定しているものとはまた別。同じものを設定する
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//マテリアルCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(5, cameraResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(6, camera_->GetConstBuffer()->GetGPUVirtualAddress());

	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]のこと
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, engine_->textureSrvHandleGPU_[index]);

	//描画
	dxCommon_->GetCommandList()->DrawIndexedInstanced(36, 1, 0, 0, 0);
	//dxCommon_->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);
}

void SkyBox::Finalize()
{
	//	vertexResource->Release();
	//	materialResource_->Release();
	//	directionalLightResource_->Release();
	//	wvpResource_->Release();
}

void SkyBox::SettingVertex()
{
	vertexResource = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * 24);
	//vertexResource = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * modelData.vertices.size());
	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();

	vertexBufferView.SizeInBytes = sizeof(VertexData) * 24;
	//vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());

	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	std::memcpy(vertexData_, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
}

void SkyBox::TransformMatrix()
{
	wvpResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(TransformationMatrix));
	wvpResource_->Map(0, NULL, reinterpret_cast<void**>(&wvpData_));
	wvpData_->WVP = MakeIdentity4x4();
}

void SkyBox::SettingColor()
{
	materialResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	materialData_->uvTransform = MakeIdentity4x4();
}

void SkyBox::SettingDictionalLight()
{
	directionalLightResource_ = DirectXCommon::CreateBufferResource(dxCommon_->GetDevice(), sizeof(DirectionalLight));
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLight_));
}

void SkyBox::CameraResource() {
	cameraResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(CameraForGPU));

	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
}

void SkyBox::SettingIndex() {

	indexResourceSprite_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(uint32_t) * 36);

	//リソースの先頭のアドレス
	indexBufferViewSprite.BufferLocation = indexResourceSprite_->GetGPUVirtualAddress();

	indexBufferViewSprite.SizeInBytes = sizeof(uint32_t) * 36;

	indexBufferViewSprite.Format = DXGI_FORMAT_R32_UINT;

	indexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));

}