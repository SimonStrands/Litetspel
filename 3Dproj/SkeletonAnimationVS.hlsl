//hereChange
struct VertexShaderInput {
	float3 position : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	int4 in_BoneID : BONEID;
	float4 in_BoneWeight: WEIGHTS;
};

struct VertexShaderOutput {
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float4 fragpos: FRAG_POS;
	float4 shadowMapCoords : SM_COORDS;
};

cbuffer CBuf
{
	row_major matrix transform;//model
	row_major matrix view;
	row_major matrix projection;
	row_major matrix lightView;
	row_major matrix jointTransforms[50];//max bones 50;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;

	float4x4 MVP = mul(mul(transform, view), projection);
	output.uv = input.uv;
	float4 bw = float4(0, 0, 0, 0);
	float4x4 boneTransform = {
		1.f,0.f,0.f,0.f,
		0.f,1.f,0.f,0.f,
		0.f,0.f,1.f,0.f,
		0.f,0.f,0.f,1.f 
	};
	boneTransform += mul(input.in_BoneWeight.x, jointTransforms[input.in_BoneID.x]);
	boneTransform += mul(input.in_BoneWeight.y, jointTransforms[input.in_BoneID.y]);
	boneTransform += mul(input.in_BoneWeight.z, jointTransforms[input.in_BoneID.z]);
	boneTransform += mul(input.in_BoneWeight.w, jointTransforms[input.in_BoneID.w]);

	float4 pos = mul(float4(input.position.xyz, 1.0f), boneTransform);
	output.position = mul(pos, mul(boneTransform, MVP));
	output.fragpos = mul(pos, transform);

	output.bitangent = normalize((mul(float4(input.bitangent, 0.0f), transform)).xyz);
	output.tangent = normalize((mul(float4(input.tangent, 0.f), transform)).xyz);
	output.normal = normalize((mul(float4(input.normal.xyz, 0.f), transform)).xyz);

	const float4 shadowCamera = mul(float4(output.position.xyz, 1.0f), transform);
	const float4 shadowHomo = mul(shadowCamera, mul(lightView, projection));
	output.shadowMapCoords = shadowHomo * float4(0.5, -0.5, 1.0f, 1.0f) + (float4(0.5f, 0.5f, 0.0f, 0.0f) * shadowHomo.w);

	return output;
}