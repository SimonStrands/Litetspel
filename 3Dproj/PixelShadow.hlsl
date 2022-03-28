struct PixelShaderInput
{
   float4 position : SV_POSITION;
};

Texture2D SM : register(t4);

float4 main(PixelShaderInput input) : SV_TARGET
{
	float4 a = input.position;
	return float4(a.x,a.y,a.z,1);
}

