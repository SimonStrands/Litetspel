
struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float4 fragpos : FRAG_POS;
};

cbuffer CBuf : register(b5) //some place special? YES
{
    float4 camPos;
};

TextureCube mirrorTex : register(t0);
SamplerState testSampler;

float4 main(PixelShaderInput input) : SV_TARGET
{   
    float3 viewVector = input.fragpos.xyz - camPos.xyz;
    float3 reflectVec = reflect(viewVector, normalize(input.normal.xyz));
    
    float4 dtex = mirrorTex.Sample(testSampler, reflectVec);
    dtex.w = 1;//we force it
    return dtex;
}