#include "stdafx.h"
#include "Canvas.h"

//메인 그림 위에 다른 그림 출력

Canvas::Canvas(Vector2 position, Vector2 scale)
	:position(position),scale(scale)
{
	shader = new Shader(L"Canvas.fx");

	//정점 서브리소스
	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.0f);
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f);
	vertices[4].Position = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[5].Position = Vector3(+0.5f, +0.5f, 0.0f);

	vertices[0].Uv = Vector2(0,1);
	vertices[1].Uv = Vector2(0,0);
	vertices[2].Uv = Vector2(1,1);
	vertices[3].Uv = Vector2(1,1);
	vertices[4].Uv = Vector2(0,0);
	vertices[5].Uv = Vector2(1,0);

	CreateBuffer(); 
	
	sDiffuseMap = shader->AsShaderResource("DiffuseMap"); //srv받아주는애
	sWorld = shader->AsMatrix("World");
	sView = shader->AsMatrix("View");
	sProjection = shader->AsMatrix("Projection");

	//카메라
	D3DXMatrixLookAtLH
	(
		&vpDesc.View,
		&Vector3(0,0,-1),
		&Vector3(0,0,0),
		&Vector3(0,1,0) //up
	); 
	
	//Projection (중심점이 있는 왼손좌표계) //다른 카메라
	D3DXMatrixOrthoOffCenterLH
	(
		&vpDesc.Projection, //저장
		0, Width,0,Height, -1,1 //z축 두깨 -1 ,1 (2픽셀)
	);


}

Canvas::~Canvas()
{
	SAFE_DELETE(shader);
	SAFE_RELEASE(vertexBuffer);
}

void Canvas::Update(Matrix & V, Matrix & P)
{
	Matrix W, S, T;

	D3DXMatrixScaling(&S, scale.x, scale.y, 1);
	D3DXMatrixTranslation(&T, position.x, position.y, 0);
	W = S * T;

	sWorld->SetMatrix(W);
	sView->SetMatrix(vpDesc.View);
	sProjection->SetMatrix(vpDesc.Projection);
}

void Canvas::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	shader->Draw(0, 0, 6);
}

void Canvas::SRV(ID3D11ShaderResourceView * srv) //srv 백버퍼 받기
{
	sDiffuseMap->SetResource(srv);//백버퍼를 다시 쉐이더로 보내기
}

void Canvas::Position(float x, float y)
{
	Position(Vector2(x, y));
}

void Canvas::Position(Vector2 & vec)
{
	position = vec;
}

void Canvas::Scale(float x, float y)
{
	Scale(Vector2(x, y));
}

void Canvas::Scale(Vector2 & vec)
{
	scale = vec;
}

void Canvas::CreateBuffer()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_IMMUTABLE; //고쳐쓰기 불가
	desc.ByteWidth = sizeof(Vertex) * 6;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = vertices; //정점 6개

	HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));
}
