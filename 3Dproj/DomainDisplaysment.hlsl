Texture2D dMap : register(t0);
SamplerState testSampler;

cbuffer CBuf
{
    row_major matrix transform; //model
    row_major matrix view;
    row_major matrix projection;
};

// Input control point
float4 position : SV_POSITION;
float2 uv : UV;
float3 normal : NORMAL;
float3 tangent : TANGENT;
float3 bitangent : BITANGENT;
struct HS_INPUT_FVS
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};
struct HS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float4 fragpos : FRAG_POS;
};

// Output patch constant data.
struct HS_CONSTANT_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor; 
    float InsideTessFactor : SV_InsideTessFactor; 
};

float3 calcPlanePointPos(float3 normal, float3 planepos, float3 origin)
{
    normal = normalize(normal);
    float d = dot(normal, planepos);
    float t = (d - dot(normal, origin)) / dot(normal, normal);
    float3 thePos = origin + (normal * t);
    return thePos;
};


#define NUM_CONTROL_POINTS 3

[domain("tri")]
HS_OUTPUT main(
	HS_CONSTANT_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<HS_INPUT_FVS, NUM_CONTROL_POINTS> patch)
{
    HS_OUTPUT Output;
    float4x4 MVP = mul(mul(transform, view), projection);
    Output.uv = patch[0].uv.xy * domain.x + patch[1].uv.xy * domain.y + patch[2].uv.xy * domain.z;
    
    Output.normal = patch[0].normal * domain.x + patch[1].normal * domain.y + patch[2].normal * domain.z;
    Output.tangent = patch[0].tangent * domain.x + patch[1].tangent * domain.y + patch[2].tangent * domain.z;
    Output.bitangent = patch[0].bitangent * domain.x + patch[1].bitangent * domain.y + patch[2].bitangent * domain.z;
    
    Output.position = float4(patch[0].position * domain.x + patch[1].position * domain.y + patch[2].position * domain.z);

    float3 height = dMap.SampleLevel(testSampler, Output.uv, 0).x;
    Output.position += float4(Output.normal * (height * 0.5f), 0);
    //float3 P = float3(patch[0].positiosd
    //
    //float3 q0 = calcPlanePointPos(patch[0].normal, patch[0].position.xyz, patch[0].position.xyz * domain.x + patch[1].position.xyz * domain.y + patch[2].position.xyz * domain.z);
    //float3 q1 = calcPlanePointPos(patch[1].normal, patch[1].position.xyz, patch[0].position.xyz * domain.x + patch[1].position.xyz * domain.y + patch[2].position.xyz * domain.z);
    //float3 q2 = calcPlanePointPos(patch[2].normal, patch[2].position.xyz, patch[0].position.xyz * domain.x + patch[1].position.xyz * domain.y + patch[2].position.xyz * domain.z);
    //float3x3 PIijk = float3x3(
    //                q0, q1, q2
    //);
    //
    //Output.position = float4((1 - alpha) * P + mul((alpha * domain.xyz), PIijk), 1);
        
    Output.fragpos = mul(Output.position, transform);
    
    Output.position = mul(Output.position, MVP);
    Output.bitangent = normalize((mul(float4(Output.bitangent, 0.0f), transform)).xyz);
    Output.tangent = normalize((mul(float4(Output.tangent, 0.f), transform)).xyz);
    Output.normal = normalize((mul(float4(Output.normal, 0.f), transform)).xyz);
     
    return Output;
}
