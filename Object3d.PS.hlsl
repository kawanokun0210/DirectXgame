#include "Object3d.hlsli"

ConstantBuffer<Material> gMaterial : register(b0);

Texture2D<float32_t4> gTexture : register(t0);

SamplerState gSampler : register(s0);

ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

ConstantBuffer<Camera> gCamera : register(b2);

PixelShaderOutput main(VertexShaderOutput input)
{
	PixelShaderOutput output;
	float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	//float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);

	float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

	output.color = gMaterial.color * textureColor;

	float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);

	float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));

	if (textureColor.a <= 0.5) {
		discard;
	}

	//if (textureColor.a == 0.0) {
	//	discard;
	//}

	//if (output.color.a == 0.0) {
	//	discard;
	//}

	if (gMaterial.enableLighting != 0)
	{
		//float cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
		float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
		float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
		float RdotE = dot(reflectLight, toEye);
		float specularPow = pow(saturate(RdotE), gMaterial.shininess);

		float32_t3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
		float32_t3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
		output.color.rgb = diffuse + specular;
		output.color.a = gMaterial.color.a * textureColor.a;

		//output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
		//output.color.a = gMaterial.color.a * textureColor.a;
	}
	else {
		output.color = gMaterial.color * textureColor;
	}
	return output;
}