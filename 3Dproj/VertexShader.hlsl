//git
//#include "MVP.hlsli"
struct VertexShaderInput {
	float3 position : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;//calc this in shader
};

struct VertexShaderOutput {
    float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float4 fragpos: FRAG_POS;
};

cbuffer CBuf
{
	row_major matrix transform;//model
	row_major matrix view;
	row_major matrix projection;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;

	float4x4 MVP = mul(mul(transform, view), projection);
	output.fragpos = mul(float4(input.position, 1.0f), transform);
	output.position = mul((float4((input.position), 1.0f)), MVP);
	output.uv = input.uv;
	output.bitangent = normalize((mul(float4(input.bitangent,0.0f), transform)).xyz);
	output.tangent = normalize((mul(float4(input.tangent,0.f), transform)).xyz);
	output.normal = normalize((mul(float4(input.normal,0.f), transform)).xyz);

	return output;
}