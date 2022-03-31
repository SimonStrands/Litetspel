#include "Transforms.hlsli"
RWTexture2D<unorm float4> backBuffer;
Texture2D<float4> gTexPosition : register(t0);
Texture2D<float4> gTexNormal : register(t1);
Texture2D<float4> gTexDiffuse : register(t2);
Texture2D<float4> gTexAmbient : register(t3);
Texture2D<float4> gTexSpecular : register(t4);
//maps
Texture2DArray<float4> shadowMapping : register(t5);
SamplerState testSampler;

//[numthreads(32, 8, 1)]//32 16
[numthreads(32, 16, 1)]//32 16
void main( uint3 DTid : SV_DispatchThreadID )
{
    const float4 color = gTexDiffuse.Load(int3(DTid.xy, 0));    
    const float4 fragPos  = gTexPosition.Load( int3(DTid.xy, 0));
    const float4 normal   = gTexNormal.Load(   int3(DTid.xy, 0));
    const float4 gAmbient  = gTexAmbient.Load(  int3(DTid.xy, 0));
    const float4 gSpecular = gTexSpecular.Load( int3(DTid.xy, 0));
    
    const int nrOfTempLight = 1;
    if (length(normal.xyz) > 0.2f)//check if there is any object at all
    {
        float4 lightning = float4(0, 0, 0, 0);
        for (int i = 0; i < nrOfLight; i++)
        {
            float3 lightDir = normalize(lightPos[i].xyz - fragPos.xyz);
			//calculate if we are in shadow
            const float4 shadowHomo = mul(fragPos, lightViewProj[i]);
            float4 shadowMapCoords = shadowHomo * float4(0.5, -0.5, 1.0f, 1.0f) + (float4(0.5f, 0.5f, 0.0f, 0.0f) * shadowHomo.w);
            shadowMapCoords.xyz = shadowMapCoords.xyz / shadowMapCoords.w;
            
            float4 SM = shadowMapping.SampleLevel(testSampler, float3(shadowMapCoords.x, shadowMapCoords.y, i), 0);
            
            //ambient
            float3 ambient_light = gAmbient.xyz * lightColor[i].xyz;
            const float bias = 0.00001f;
            if (SM.r > shadowMapCoords.z - bias &&
                shadowMapCoords.z <= 1.0f &&//E
				shadowMapCoords.x < 1 && shadowMapCoords.x > 0 &&
				shadowMapCoords.y < 1 && shadowMapCoords.y > 0 &&
                dot(normal.xyz, lightDir.xyz) > -0.1
               )
            {
                float3 viewDir = normalize(cameraPos.xyz - fragPos.xyz);
				//defuse
                float3 defuse_light;
                float ammount_diffuse = max(dot(normal.xyz, lightDir), 0.0f);
                defuse_light = ammount_diffuse * color.xyz * lightColor[i].xyz;
                
                //specular
                float3 reflectDir = reflect(-lightDir, normal.xyz);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), gSpecular.w);
                float3 specular = gSpecular.xyz * spec;
                
                lightning.xyz += saturate(ambient_light + defuse_light) + specular;
            }
            else
            {
				//we are in shadow
                lightning += float4((gAmbient.xyz * color.xyz), 0);
            }
        }
        backBuffer[DTid.xy] = float4(lightning.xyz, 1);
    }	
}
