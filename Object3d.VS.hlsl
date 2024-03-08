#include "Object3d.hlsli"

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
ConstantBuffer<CameraData> gCameraData : register(b2);

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	float32_t4x4 ViewProjectionMatrix = mul(gCameraData.view, gCameraData.projection);
    float32_t4x4 WorldViewProjectionMatrix = mul(gTransformationMatrix.World, ViewProjectionMatrix);

	output.position = mul(input.position, gTransformationMatrix.WVP);
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float32_t3x3)gTransformationMatrix.WorldInverseTranspose));
	output.worldPosition = mul(input.position, gTransformationMatrix.World).xyz;
	return output;
}