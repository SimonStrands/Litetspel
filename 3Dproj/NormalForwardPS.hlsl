#include "Transforms.hlsli"

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float4 fragpos : FRAG_POS;
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

float Attenuate(uniform float attConst, uniform float attLin, uniform float attQuad, const in float distFragToL)
{
    return 1.0f / (attConst + attLin * distFragToL + attQuad * (distFragToL * distFragToL));
}

float4 main(PixelShaderInput input) : SV_TARGET
{
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
        
        float dist = length(lightPos[i].xyz - input.fragpos.xyz);
        //ambient
        float3 ambient_light = ka.xyz * lightColor[i].xyz;
        static const float bias = 0.00001f;
        float attenuation = 1;
        
        if (lightPos[i].w == 2)
        {
            attenuation = Attenuate(1, 1 / lightColor[i].w, 0.001f, dist);
        }
        if (lightPos[i].w == 2 && dist < (lightColor[i].w * 2) ||
            SM.r > shadowMapCoords.z - bias &&
            shadowMapCoords.z <= 1.0f && //E
            shadowMapCoords.x < 1 && shadowMapCoords.x > 0 &&
            shadowMapCoords.y < 1 && shadowMapCoords.y > 0 &&
            dot(input.normal, lightDir) > -0.2f
            )
        {
            float3 viewDir = normalize(cameraPos.xyz - input.fragpos.xyz);
            
            //defuse
            float3 defuse_light;
            float ammount_diffuse = max(dot(input.normal.xyz, lightDir), 0.0f);
            defuse_light = ammount_diffuse * kd.xyz * lightColor[i].xyz;

            //specular
            float3 reflectDir = reflect(-lightDir, input.normal.xyz);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), ks.w);
            float3 specular = ks.xyz * spec;

            //FinalPixel.xyz += saturate(ambient_light + defuse_light) + specular;
            FinalPixel.xyz += saturate(ambient_light * attenuation + defuse_light * attenuation) + specular * attenuation;
        }
        else
        {
            //we are in shadow
            FinalPixel += float4((ka.xyz), 0);
        }
    }
    //return float4(FinalPixel.xyz, 1);
    return float4(FinalPixel.xyz * color.xyz, color.a);

}