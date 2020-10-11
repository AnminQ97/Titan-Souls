
cbuffer CB_PerFrame
{
    matrix View;
    matrix Projection;
}

matrix World;

//-----------------------------------------------------------------------------
//Types
//-----------------------------------------------------------------------------
struct VertexInput
{
    float4 Position : POSITION0;
};

struct VertexOutput
{
    float4 Position : SV_POSITION0;
};

//-----------------------------------------------------------------------------
//Vertex Shader
//-----------------------------------------------------------------------------
VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    return output;
}

//-----------------------------------------------------------------------------
//Pixel Shader
//-----------------------------------------------------------------------------
Texture2D DiffuseMap;
SamplerState Sampler;
                              //float4(0, 1, 0, 1)
float4 PS(VertexOutput input, uniform float4 color) : SV_TARGET0
{
    return color;
}


//-----------------------------------------------------------------------------
//Pipe Line
//-----------------------------------------------------------------------------
technique11 T0
{
    pass P0 //0번 패스
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS(float4(0, 1, 0, 1))));//초록색
    }
  
    pass P1 //1번 패스
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS(float4(1, 0, 0, 1)))); //빨간색
    }
}