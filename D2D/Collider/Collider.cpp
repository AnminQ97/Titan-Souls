#include "stdafx.h"
#include "collider.h"

Collider::Collider()
	: bDrawColliderColor(false)
{
	shader = new Shader(L"Bounding.fx");
	CreateBuffer();
	
	D3DXMatrixIdentity(&world);
	sWorld = shader->AsMatrix("World");
	sView = shader->AsMatrix("View");
	sProjection = shader->AsMatrix("Projection");
	sColor = shader->AsVector("LineColor");

	initLineColor = D3DXCOLOR(0, 1, 0, 1);
	collidedColor = D3DXCOLOR(1, 0, 0, 1);
}

void Collider::CreateBuffer()
{
	Vertex vertices[5];
	vertices[0].Positon = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertices[1].Positon = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[2].Positon = D3DXVECTOR3(+0.5f, +0.5f, 0.0f);
	vertices[3].Positon = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
	vertices[4].Positon = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);

	//Create VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 5;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = vertices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}
}

bool Collider::Obb(Sprite * a, Sprite * b)
{
	ObbDesc A, B;
	D3DXVECTOR2 halfSize;

	halfSize = D3DXVECTOR2(a->Scale().x * a->TextureSize().x, a->Scale().y * a->TextureSize().y) * 0.5f;
	CreateObb(&A, halfSize, a->World());

	halfSize = D3DXVECTOR2(b->Scale().x * b->TextureSize().x, b->Scale().y * b->TextureSize().y) * 0.5f;
	CreateObb(&B, halfSize, b->World());

	D3DXVECTOR2 distance = a->Position() - b->Position();

	//A.Right축 검사
	float lengthA = D3DXVec2Length(&A.Length_Right);
	float lenghtB = SeparateAxis(A.Right, B.Length_Right, B.Length_Up);
	float length = fabsf(D3DXVec2Dot(&A.Right, &distance));
	if (length > lengthA + lenghtB) return false;

	//A.Up축 검사
	lengthA = D3DXVec2Length(&A.Length_Up);
	lenghtB = SeparateAxis(A.Up, B.Length_Right, B.Length_Up);
	length = fabsf(D3DXVec2Dot(&A.Up, &distance));
	if (length > lengthA + lenghtB) return false;

	//B.Right축 검사
	lengthA = D3DXVec2Length(&B.Length_Right);
	lenghtB = SeparateAxis(B.Right, A.Length_Right, A.Length_Up);
	length = fabsf(D3DXVec2Dot(&B.Right, &distance));
	if (length > lengthA + lenghtB) return false;

	//B.Up축 검사
	lengthA = D3DXVec2Length(&B.Length_Up);
	lenghtB = SeparateAxis(B.Up, A.Length_Right, A.Length_Up);
	length = fabsf(D3DXVec2Dot(&B.Up, &distance));
	if (length > lengthA + lenghtB) return false;

	return true;
}

void Collider::CreateObb(ObbDesc * out, D3DXVECTOR2 & half, D3DXMATRIX & transform)
{
	D3DXVec2Normalize(&out->Right, &D3DXVECTOR2(transform._11, transform._12));
	D3DXVec2Normalize(&out->Up, &D3DXVECTOR2(transform._21, transform._22));

	out->HalfSize = half;
	out->Length_Right = out->Right * out->HalfSize.x;
	out->Length_Up = out->Up * out->HalfSize.y;
}

float Collider::SeparateAxis(D3DXVECTOR2 & init, D3DXVECTOR2 & e1, D3DXVECTOR2 & e2)
{
	return fabsf(D3DXVec2Dot(&init, &e1)) + fabsf(D3DXVec2Dot(&init, &e2));
}

Collider::~Collider()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_DELETE(shader);
}

void Collider::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	sWorld->SetMatrix(world);
	sView->SetMatrix(V);
	sProjection->SetMatrix(P);
}

void Collider::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	sColor->SetFloatVector(bDrawColliderColor ? collidedColor : initLineColor);

	shader->Draw(0, 0, 5);

}

bool Collider::Aabb(D3DXMATRIX & world, D3DXVECTOR2 & position)
{
	D3DXVECTOR2 position1 = D3DXVECTOR2(world._41, world._42);
	D3DXVECTOR2 position2 = position;

	D3DXVECTOR2 distance;
	distance.x = fabsf(position1.x - position2.x);
	distance.y = fabsf(position1.y - position2.y);

	D3DXVECTOR2 halfSize;
	halfSize = D3DXVECTOR2(fabsf(world._11), fabsf(world._22)) * 0.5f;

	if (distance.x > halfSize.x) return false;
	if (distance.y > halfSize.y) return false;
	
	return true;
}

bool Collider::Aabb(D3DXMATRIX & world1, D3DXMATRIX & world2)
{
	D3DXVECTOR2 position1 = D3DXVECTOR2(world1._41, world1._42);
	D3DXVECTOR2 position2 = D3DXVECTOR2(world2._41, world2._42);

	D3DXVECTOR2 distance;
	distance.x = fabsf(position1.x - position2.x);
	distance.y = fabsf(position1.y - position2.y);

	D3DXVECTOR2 halfSize1, halfSize2;
	halfSize1 = D3DXVECTOR2(fabsf(world1._11), fabsf(world1._22)) * 0.5f;
	halfSize2 = D3DXVECTOR2(fabsf(world2._11), fabsf(world2._22)) * 0.5f;

	if (distance.x > halfSize1.x + halfSize2.x) return false;
	if (distance.y > halfSize1.y + halfSize2.y) return false;

	return true;
}


