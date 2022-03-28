
struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float4 fragpos : FRAG_POS;
    //float4 shadowMapCoords : SM_COORDS;
};

struct PixelShaderOutput
{
    float4 Position : SV_Target0; //depth
    float4 Normal : SV_Target1; //normal
    float4 Color : SV_Target2; //map_kd * kd
    float4 ambient : SV_Target3; //map_ka * ka
    float4 specular : SV_Target4; //map_ks * ks //can I put NS as last element?
};

cbuffer CBuf
{
    float4 kd;
    float4 ks; //last element is ns
    float4 ka;
};


Texture2D diffuseTex : register(t0); // diffuse base color
Texture2D ambientTex : register(t1); //normal light(without light)
Texture2D specularTex : register(t2); // specular //shinyness
Texture2D nMap : register(t3); // normal map
SamplerState testSampler;

PixelShaderOutput main(PixelShaderInput input)
{
    PixelShaderOutput output;

    float3 nMapNormal;
    float3x3 TBN = float3x3(
		input.tangent.xyz,
		input.bitangent.xyz,
		input.normal.xyz
		);
    const float3 normalSample = nMap.Sample(testSampler, input.uv).xyz;
    nMapNormal.x = normalSample.x * 2.0f - 1.0f;
    nMapNormal.y = normalSample.y * 2.0f - 1.0f;
    nMapNormal.z = normalSample.z * 2.0f - 1.0f;
    input.normal = mul(nMapNormal, (float3x3) TBN);
    
    output.Normal = float4(input.normal.xyz, 1);
    output.Position = float4(input.fragpos);
    output.Color = diffuseTex.Sample(testSampler, input.uv) * kd;
    output.ambient = ambientTex.Sample(testSampler, input.uv) * ka;
    output.specular = specularTex.Sample(testSampler, input.uv) * ks;
    output.specular.w = ks.w;
	
    return output;
}
