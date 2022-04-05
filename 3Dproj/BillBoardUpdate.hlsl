//another gbuffer for shadowMapCoords?
//maps
cbuffer Time
{
    float dt;
    float rand;
    float2 padding;
};
RWBuffer<float> particlePos;
[numthreads(8, 1, 1)]//change this later
void main(uint3 DTid : SV_DispatchThreadID)
{
    
    float3 currPos = float3(particlePos[DTid.x * 3 + 0], particlePos[DTid.x * 3 + 1], particlePos[DTid.x * 3 + 2]); //x
    float3 OutPut = currPos;
    
    
    OutPut.y -= (5) * dt;
    if (OutPut.y < -10)
    {
        OutPut.y = (10 * rand);
    }
    
    particlePos[DTid.x * 3 + 0] = OutPut.x;//x
    particlePos[DTid.x * 3 + 1] = OutPut.y;//y
    particlePos[DTid.x * 3 + 2] = OutPut.z;//z
}
