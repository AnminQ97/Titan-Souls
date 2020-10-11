cbuffer CB_PerFrame //전역변수
{
    matrix View;       
    matrix Projection;
}

matrix World;

Texture2D DiffuseMap; //srv 받기
SamplerState Sampler;

//-----------------------------------------------------------------------------
//Types
//-----------------------------------------------------------------------------
struct VertexInput
{
    float4 Position : POSITION0;
    float2 Uv : UV0;
};

struct VertexOutput
{
    float4 Position : SV_POSITION0;
    float2 Uv : UV0;
};

//-----------------------------------------------------------------------------
//Vertex Shader
//-----------------------------------------------------------------------------
VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    //output.Position = input.Position;
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Uv = input.Uv;

    return output;
}

//-----------------------------------------------------------------------------
//Pixel Shader
//-----------------------------------------------------------------------------
float4 PS(VertexOutput input) : SV_TARGET0
{
   float4 diffuse = DiffuseMap.Sample(Sampler, input.Uv);
   
   float4 reverseY = DiffuseMap.Sample(Sampler, float2(input.Uv.x, 1-input.Uv.y));
   
    return reverseY + float4(0.3f, 0.2f, 0.7f, 0.3f);

    //Overlay
    //return float4(input.Uv, 0, diffuse.a);

    //Reverse  색 반전
    //return 1 - diffuse;

    //Monotone 회색
    float gray = (diffuse.r + diffuse.g + diffuse.b)/3.0f;
    return float4(gray, gray, gray,diffuse.a);
}

//-----------------------------------------------------------------------------
//Pipe Line
//-----------------------------------------------------------------------------
technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
  
}