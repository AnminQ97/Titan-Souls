#include"stdafx.h"
#include"Line.h"

Line::Line(Vector2 a, Vector2 b)
	:position(0, 0), scale(1, 1),left(a),right(b)
{
	CreateBuffer(a, b);

	sView = shader->AsMatrix("View");
	sProjectiion = shader->AsMatrix("Projection");
	sWorld = shader->AsMatrix("World");

	if (a.x > b.x)
	{
		left = b;
		right = a;
	}
}

Line::~Line()
{
	SAFE_DELETE(shader);
	SAFE_RELEASE(vertexBuffer);
}

void Line::Update(Matrix & V, Matrix & P)
{
	//������
	sView->SetMatrix(V);
	sProjectiion->SetMatrix(P);

	Matrix W, S, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, 1.0f);
	D3DXMatrixTranslation(&T, position.x, position.y, 0.0f);
	W = S * T;

	//���ο� ���� ����
	vertices[0].Position = Vector3(left.x, left.y, 0.0f);
	vertices[1].Position = Vector3(right.x, right.y, 0.0f);

	//GPU Write(��������� �ƴ�)//���� ���� ����
	//DeviceContext->UpdateSubresource
	//(
	//	vertexBuffer, 0, NULL, vertices, sizeof(Vertex) * 2, 0
	//);

	//Map Write (CPU���) //desc.Usage = D3D11_USAGE_DYNAMIC; 
	//CPU���� D3D11_USAGE_DYNAMIC
	D3D11_MAPPED_SUBRESOURCE subResoruce;
									  //���ο� ������ �����
	DeviceContext->Map(vertexBuffer,0,D3D11_MAP_WRITE_DISCARD,0, &subResoruce); //���긮�ҽ� �°�, ���ý� ���۴� ����
	{	//memcpy �޸� ����
		memcpy(subResoruce.pData, vertices, sizeof(Vertex) * 2);  //���ľ����� �ϴ� ���� subResoruce�� ����
	}
	DeviceContext->Unmap(vertexBuffer, 0);//�ݾ��ֱ�
	sWorld->SetMatrix(W);
}

void Line::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	shader->Draw(0, bDrawCollision ? 1 : 0, 2);
}

void Line::Position(float x, float y)
{
	Position(Vector2(x, y));
}

void Line::Position(Vector2 & vec)
{
	position = vec;
}

void Line::Scale(float x, float y)
{
	Scale(Vector2(x, y));
}

void Line::Scale(Vector2 & vec)
{
	scale = vec;
}

void Line::CreateBuffer(Vector2 a, Vector2 b)
{
	shader = new Shader(L"Line.fx");

	vertices[0].Position = Vector3(a.x, a.y,0.0f);
	vertices[1].Position = Vector3(b.x, b.y,0.0f);


	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC; //CPU���� D3D11_USAGE_DYNAMIC
	desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE; //CPU ������� 
	desc.ByteWidth = sizeof(Vertex) * 2;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = vertices;//���� ���ҽ�

	HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));
}
