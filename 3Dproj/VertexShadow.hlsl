struct VertexShaderInput {
	float3 position : POSITION;
};

struct VertexShaderOutput {
	float4 position : SV_POSITION;
	//just bcs hull sahder wants it
    float2 uv        : UV;
    float3 normal    : NORMAL;
    float3 tangent   : TANGENT;
    float3 bitangent : BITANGENT;
};

cbuffer CBuf
{
	row_major matrix transform;//model
	row_major matrix view;
	row_major matrix projection;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
    
	float4x4 MVP = mul(mul(transform, view),projection);
	output.position = mul((float4((input.position), 1.0f)), MVP);
    
    output.uv = float2(0, 0);
	output.normal	 = float3(0,0,0) ;
    output.tangent	 = float3(0,0,0) ;
	output.bitangent = float3(0,0,0) ;
	
	return output;
}