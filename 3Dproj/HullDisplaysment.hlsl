// Input control point
struct HS_INPUT_FVS
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

// Output patch constant data.
struct HS_CONSTANT_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
};

cbuffer CBuf : register(b5) //some place special? YES
{
    float4 camPos;
};

cbuffer CBuf
{
    row_major matrix transform; //model
    row_major matrix view;
    row_major matrix projection;
};

#define NUM_CONTROL_POINTS 3

float GetTessLevel(float Distance0, float Distance1)
{
    float AvgDistance = (Distance0 + Distance1) / 2.0;
   // float MaxDistance = 50.f;
    float MaxDistance = 50.f;
    float McDDistance = 5.f; //maccers is oppisete of max
    float MaxLod = 12;
    float mcDLod = 1;

    if (AvgDistance > MaxDistance)
    {
        AvgDistance = MaxDistance - 0.1;
    }
    AvgDistance -= MaxDistance;
    AvgDistance = -AvgDistance;
    
    float proc = AvgDistance / MaxDistance;
    return proc * MaxLod;
}

// Patch Constant Function
HS_CONSTANT_OUTPUT CalcHSPatchConstants(
	InputPatch<HS_INPUT_FVS, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
    HS_CONSTANT_OUTPUT Output;
    float4x4 tv = mul(transform, view);
    float DistToVertex0 = mul(ip[0].position, tv).z; //what wrong here
    float DistToVertex1 = mul(ip[1].position, tv).z;
    float DistToVertex2 = mul(ip[2].position, tv).z;
    //float DistToVertex0 = distance(frag1.xyz, camPos.xyz);
    //float DistToVertex1 = distance(frag2.xyz, camPos.xyz);
    //float DistToVertex2 = distance(frag3.xyz, camPos.xyz);
	
    Output.EdgeTessFactor[0] = GetTessLevel(DistToVertex1, DistToVertex2);
    Output.EdgeTessFactor[1] = GetTessLevel(DistToVertex2, DistToVertex0);
    Output.EdgeTessFactor[2] = GetTessLevel(DistToVertex0, DistToVertex1);
    
    Output.InsideTessFactor = Output.EdgeTessFactor[2];

    return Output;
}

[domain("tri")]
[partitioning("fractional_odd")]
//[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_INPUT_FVS main(
	InputPatch<HS_INPUT_FVS, NUM_CONTROL_POINTS> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
    HS_INPUT_FVS Output;
	
    return ip[i];

}
