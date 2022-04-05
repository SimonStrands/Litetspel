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

Texture2D diffuseTex : register(t0); // diffuse base color
//Texture2D nMap : register(t1); // normal map
Texture2DArray<float4> shadowMapping : register(t1);
SamplerState testSampler;


float4 main(PixelShaderInput input) : SV_TARGET
{
	//add the texture
	//float3 final = (diffuseTex.Sample(testSampler, input.uv).xyz);
    //
	//return float4(final, 1);
    float4 color = diffuseTex.Sample(testSampler, input.uv);

    float4 FinalPixel = float4(0, 0, 0, 0);
    for (int i = 0; i < nrOfLight; i++)
    {
        float3 lightDir = normalize(lightPos[i].xyz - input.fragpos.xyz);
        //calculate if we are in shadow
        const float4 shadowHomo = mul(input.fragpos, lightViewProj[i]);
        float4 shadowMapCoords = shadowHomo * float4(0.5, -0.5, 1.0f, 1.0f) + (float4(0.5f, 0.5f, 0.0f, 0.0f) * shadowHomo.w);
        shadowMapCoords.xyz = shadowMapCoords.xyz / shadowMapCoords.w;

        float4 SM = shadowMapping.SampleLevel(testSampler, float3(shadowMapCoords.x, shadowMapCoords.y, i), 0);

        //ambient
        float3 ambient_light = ka.xyz * lightColor[i].xyz;
        const float bias = 0.00001f;
        if (SM.r > shadowMapCoords.z - bias &&
            shadowMapCoords.z <= 1.0f &&//E
            shadowMapCoords.x < 1 && shadowMapCoords.x > 0 &&
            shadowMapCoords.y < 1 && shadowMapCoords.y > 0 &&
            dot(input.normal.xyz, lightDir.xyz) > -0.1
            )
        {
            float3 viewDir = normalize(cameraPos.xyz - input.fragpos.xyz);
            //defuse
            float3 defuse_light;
            float ammount_diffuse = max(dot(input.normal.xyz, lightDir), 0.0f);
            defuse_light = ammount_diffuse * color.xyz * lightColor[i].xyz;

            //specular
            float3 reflectDir = reflect(-lightDir, input.normal.xyz);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), ks.w);
            float3 specular = ks.xyz * spec;

            FinalPixel.xyz += saturate(ambient_light + defuse_light) + specular;
        }
        else
        {
            //we are in shadow
            FinalPixel += float4((ka.xyz * color.xyz), 0);
        }
    }
    //return float4(FinalPixel.xyz, 1);
    return float4(FinalPixel.xyz, color.a);

}