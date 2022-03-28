#include "Transforms.hlsli"
RWTexture2DArray<unorm float4> backBuffer;
//another gbuffer for shadowMapCoords?
Texture2D<float4> gTexPosition : register(t0);
Texture2D<float4> gTexNormal : register(t1);
Texture2D<float4> gTexDiffuse : register(t2);
Texture2D<float4> gTexAmbient : register(t3);
Texture2D<float4> gTexSpecular : register(t4);
//maps
Texture2DArray<float4> shadowMapping : register(t5);
SamplerState theSampler;

[numthreads(32, 8, 1)] //32 16
void main(uint3 DTid : SV_DispatchThreadID)
{
    int wh = 320;
    float x = 1920 / wh;
    float y = 1080 / wh;
    float4 fragPos =   gTexPosition.Load(int3(DTid.x * 1, DTid.y * 1, 0));
    float4 normal =    gTexNormal.Load  (int3(DTid.x * 1, DTid.y * 1, 0));
    float4 color =     gTexDiffuse.Load (int3(DTid.x * 1, DTid.y * 1, 0));
    float4 gAmbient =  gTexAmbient.Load (int3(DTid.x * 1, DTid.y * 1, 0));
    float4 gSpecular = gTexSpecular.Load(int3(DTid.x * 1, DTid.y * 1, 0));
    
    const float SMWIDTH = 1920;
    const float SMHEIGHT = 1080;
    
    if (length(normal.xyz) > 0.2f)//check if there is any object at all
    { //nroflights
        //gets wrong fragpos???
        float4 lightning = float4(0, 0, 0, 0);
        for (int i = 0; i < nrOfLight; i++)
        {
            float3 lightColor = float3(1, 1, 1);

            float3 lightDir = normalize(lightPos[i].xyz - fragPos.xyz);
			//calculate if we are in shadow
            const float4 shadowCamera = fragPos;
            const float4 shadowHomo = mul(shadowCamera, lightViewProj[i]);
            float4 shadowMapCoords = shadowHomo * float4(0.5, -0.5, 1.0f, 1.0f) + (float4(0.5f, 0.5f, 0.0f, 0.0f) * shadowHomo.w);
            shadowMapCoords.xyz = shadowMapCoords.xyz / shadowMapCoords.w;
            //float4 SM = shadowMapping.Load(int4(shadowMapCoords.x * SMWIDTH, shadowMapCoords.y * SMHEIGHT, i, 0));
            float4 SM = shadowMapping.Load(int4(shadowMapCoords.x * SMWIDTH, shadowMapCoords.y * SMHEIGHT, i, 0));
            //ambient
            float3 ambient_light = gAmbient.xyz * lightColor;
            if (SM.r > shadowMapCoords.z - 0.0001 &&
				shadowMapCoords.x < 1 && shadowMapCoords.x > 0 &&
				shadowMapCoords.y < 1 && shadowMapCoords.y > 0 &&
                dot(normal.xyz, lightDir.xyz) > -0.1
               )
            {
                //if (dot(normal.xyz, lightDir) > -1.0)//don't know if this works with normal maps (haha it doesn't)
               // {
                float3 viewDir = normalize(cameraPos.xyz - fragPos.xyz);
                float3 halfWayDir = normalize(lightDir - viewDir);
                
				//////calc lightning//////
                
                
				//defuse
                float3 defuse_light;
                float ammount_diffuse = max(dot(normal.xyz, lightDir), 0.0f);
                defuse_light = ammount_diffuse * color.xyz * lightColor.xyz;
                
                //specular
                float3 reflectDir = reflect(-lightDir, normal.xyz);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), gSpecular.w);
                float3 specular = gSpecular.xyz * spec;
                
                lightning.xyz += saturate(ambient_light + defuse_light) + specular;
            }
            else
            {
				//we are in shadow
                lightning += float4((gAmbient.xyz * color.xyz * lightColor), 0);
            }
        }
        backBuffer[DTid.xyz] = float4(lightning.xyz, 1);
    }
}
