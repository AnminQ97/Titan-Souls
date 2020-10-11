#pragma once

class Canvas
{
public:
	Canvas(Vector2 position, Vector2 scale);
	~Canvas();

	void Update(Matrix& V, Matrix& P); //ȭ�� �� ������
	void Render();

	//backBuffer ���� �ޱ�
	void SRV(ID3D11ShaderResourceView* srv); //srv��°�� �ޱ�
	
public:
	void Position(float x, float y);
	void Position(Vector2& vec);
	Vector2& Position() { return position; }

	void Scale(float x, float y);
	void Scale(Vector2& vec);
	Vector2& Scale() { return scale; }

	Shader* GetShader() { return shader; }

private:
	void CreateBuffer(); //���� ����� �Լ�

private:
	struct Vertex
	{
		Vector3 Position;
		Vector2 Uv;
	}vertices[6];

	//ȭ�� ��� ������� ���� ī�޶�
	struct ViewProjectionDesc
	{
		Matrix View;
		Matrix Projection;
	}vpDesc;

private:
	Shader * shader;
	ID3D11Buffer* vertexBuffer;//���ý��� ���� ����

	Vector2 position;
	Vector2 scale;

	ID3DX11EffectMatrixVariable* sWorld, *sView, *sProjection;
	ID3DX11EffectShaderResourceVariable* sDiffuseMap; //shader->AsShaderResource("DiffuseMap")
};