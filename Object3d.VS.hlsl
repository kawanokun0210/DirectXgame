float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}

struct VertexShaderOutput {
	float32_t4 position : SV_POSITION;
};

struct VertexShaderInput {
	float32_t4 position : POSITION0;
};

VertexShaderOutPut main(VertexShaderInput input) {
	VertexShaderOutput output;
	output.position = input.positon;
	return output;
}