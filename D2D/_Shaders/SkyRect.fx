cbuffer CB_PerFrame
{
    matrix View;
    matrix Projection;
}

matrix World;

struct SkyDesc
{
    float4 Center;
    float4 Apex;
    float Height;
};

SkyDesc Sky;

//Types
struct VertexInput
{
    float4 Position : POSITION0;
};

struct VertexOutput
{
    float4 Position : SV_POSITION0; //W V P ���� ��ȯ �Ѱ�
    float4 oPosition : POSITION1; //���� ��ȯ �Ǳ� �� 
};

//Vertex Shader
VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.oPosition = input.Position;
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    return output;
}

//Pixel Shader

float4 PS(VertexOutput input) : SV_TARGET0
{

    float y = saturate(input.oPosition.y) * 2.0f; //0~1 ���� ������ ����
    return lerp(Sky.Center, Sky.Apex, y * Sky.Height);
}

//Pipe Line
technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
  
}