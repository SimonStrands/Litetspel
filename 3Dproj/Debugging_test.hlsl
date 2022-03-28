
struct GSInput
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float4 fragpos: FRAG_POS;
	float4 shadowMapCoords : SM_COORDS;
};
struct GSOutput
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float4 fragpos: FRAG_POS;
	float4 shadowMapCoords : SM_COORDS;
};

[maxvertexcount(3)]
void main(
	triangle GSInput input[3],
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	float4 pos[3] = {float4(1,1,0,0),float4(0,1,0,0), float4(0,0,0,0)};
	for (uint i = 0; i < 3; i++)
	{
		//element.position = input[i].position;
		element.position = pos[i];
		element.uv = input[i].uv;
		element.normal = input[i].normal;
		element.tangent = input[i].tangent;
		element.bitangent = input[i].bitangent;
		element.fragpos = input[i].fragpos;
		element.shadowMapCoords = input[i].shadowMapCoords;
		output.Append(element);
	}

}