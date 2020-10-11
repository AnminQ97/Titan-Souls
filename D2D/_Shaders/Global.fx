//���뽦�̴�

//WVP
cbuffer CB_PerFrame
{
	matrix View;
	matrix Projection;
}

matrix World;

//Texture
Texture2D DiffuseMap; //srv �ޱ�

//color
float4 Color;

//VertexInput
struct VertexTexture
{
	float4 Position : POSITION0;
	float2 Uv : UV0;
};

//States
SamplerState Sampler
{
    AddressU = Mirror;
    AddressV = Mirror;
    //AddressU = WRAP;
    //AddressV = WRAP;
};

RasterizerState Cull
{
	CullMode = None; // CullMode = None ���� �޸鵵(�ݽð� ����) ���̰� �Ѵ�
	DepthClipEnable = false;  //z�� �α� �Ѿ �����ְ� �Ѵ�
};

RasterizerState FillMode_WireFrame
{
	FillMode = WireFrame;
};

BlendState AlphaBlend
{
	BlendEnable[0] = true;
	DestBlend[0] = INV_SRC_ALPHA;
	SrcBlend[0] = SRC_ALPHA;
	BlendOp[0] = Add;

	SrcBlendAlpha[0] = One;
	DestBlendAlpha[0] = One;
	RenderTargetWriteMask[0] = 0x0F;
};

//��ũ��
//Pass
#define P_VP(name,vs, ps)\
pass name \
{\
	SetVertexShader(CompileShader(vs_5_0, vs()));\
	SetPixelShader(CompileShader(ps_5_0, ps()));\
\
	SetRasterizerState(Cull);\
	SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF);\
}
