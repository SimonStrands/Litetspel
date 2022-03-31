#include "Transforms.hlsli"

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float4 fragpos: FRAG_POS;
};

cbuffer CBuf
{
	float4 kd;
	float4 ks;
	float4 ka;
};

Texture2D testTex : register(t1);
SamplerState testSampler;


float4 main(PixelShaderInput input) : SV_TARGET
{
	//for culling none
    float4 lightPosB = float4(lightPos[0].xyz,1);
	float3 posToView = normalize(input.fragpos.xyz - cameraPos.xyz);
	if (dot(posToView, input.normal) > 0) {
		input.normal = -input.normal;
	}
	float3 lightColor = float3(1, 1, 1);
	//ambient									//light color
    float3 ambient_light = float3(0.1, 0.1, 0.1) * lightColor;
    ambient_light = float3(0.9, 0.9, 0.9);
	//return float4(ka.xyz, 1);

	float3 specular;
	float3 defuse_light;
	//defuse
	float3 lightDir = normalize(input.fragpos.xyz - lightPosB.xyz);
	float ammount_diffuse = max(dot(-input.normal.xyz, lightDir), 0.0f);
	defuse_light = ammount_diffuse * kd.xyz * lightColor.xyz;

	//specular
	float const_spec = 1.0f;
	float3 reflection = normalize(reflect(-lightDir, normalize(input.normal.xyz)));
	float spec = pow(max(0.f, dot(posToView, reflection)), 32);
	specular = ks.xyz * (lightColor.xyz * ammount_diffuse) * const_spec * ks.xyz * spec;


	//get final lightning
	float3 lightning = saturate(ambient_light + defuse_light) + specular;

	//add the texture
	float4 dtex = testTex.Sample(testSampler, input.uv);
	float3 final = (testTex.Sample(testSampler, input.uv).xyz) * lightning;

	return float4(final, dtex.a);

}