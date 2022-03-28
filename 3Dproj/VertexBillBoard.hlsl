struct VertexShaderInput {
	float3 position : POSITION;
};

struct VertexShaderOutput {
	float4 position : SV_POSITION;
	row_major float4x4 model : MODEL;
	row_major float4x4 view : VIEW;
	row_major float4x4 projection : PR;
	
};

cbuffer CBuf
{
	row_major matrix transform;//model
	row_major matrix view;
	row_major matrix projection;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;

	output.position = float4(input.position, 1.f);
	output.model = transform;
	output.projection = projection;
	output.view = view;

	return output;
}