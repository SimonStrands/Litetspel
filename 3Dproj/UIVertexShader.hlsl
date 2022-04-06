struct VertexShaderInput
{
    float2 position : POSITION;
    float2 uv : UV;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
};

cbuffer CBuf
{
    float2 size;
    float2 position;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;

    output.position = float4(input.position.x * size.x + position.x, input.position.y * size.y + position.y, 0, 0);
    output.uv = input.uv;

    return output;
}