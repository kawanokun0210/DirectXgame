struct VertexShaderOutput{
	float32_t4 position : SV_POSITION;
	float32_t3 texcoord : TEXCOORD0;
	float32_t3 normal : NORMAL0;
};

struct VertexShaderInput
{
	float32_t4 position : POSITION0;
};

struct PixelShaderOutput
{
	float32_t4 color : SV_TARGET0;
};

struct TransformationMatrix
{
	float32_t4x4 WVP;
	float32_t4x4 World;
	float32_t4x4 WorldInverseTranspose;
};

struct Material
{
	float32_t4 color;
};