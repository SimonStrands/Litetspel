//git
//#include "MVP.hlsli"
struct VertexShaderInput
{
    float3 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;

    output.position = float4(input.position, 1.0f);
    output.uv = input.uv;
    output.bitangent = normalize(input.bitangent);
    output.tangent = normalize(input.tangent);
    output.normal = normalize(input.normal);


    return output;
}