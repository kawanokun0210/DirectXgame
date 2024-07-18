#include "Skybox.hlsli"

TextureCube<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<Material> gMaterial : register(b0);
TextureCube<float32_t4> gEnvironmentTexture : register(t1);
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b1);

PixelShaderOutput main(VertexShaderOutput input){
	PixelShaderOutput output;
	float32_t3 cameraToPosition = mul(input.position, gTransformationMatrix.WVP).xyz;
	float32_t3 reflectedVector = reflect(cameraToPosition,normalize(input.normal));
	float32_t4 environmentColor = gEnvironmentTexture.Sample(gSampler,reflectedVector);

	float32_t4 textureColor = gTexture.Sample(gSampler,input.texcoord);
	output.color = textureColor * gMaterial.color;
	output.color.rgb += environmentColor.rgb;
	return output;
}