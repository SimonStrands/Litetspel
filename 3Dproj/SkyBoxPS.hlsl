struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float4 fragpos : FRAG_POS;
};

TextureCube tex : register(t0);
SamplerState testSampler;

float4 main(PixelShaderInput input) : SV_TARGET
{
    return float4(tex.Sample(testSampler, input.fragpos.xyz));
    //return float4(1,0,0,1);
}