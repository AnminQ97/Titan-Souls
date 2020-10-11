#pragma once

class Canvas
{
public:
	Canvas(Vector2 position, Vector2 scale);
	~Canvas();

	void Update(Matrix& V, Matrix& P); //화면 속 월드계산
	void Render();

	//backBuffer 정보 받기
	void SRV(ID3D11ShaderResourceView* srv); //srv통째로 받기
	
public:
	void Position(float x, float y);
	void Position(Vector2& vec);
	Vector2& Position() { return position; }

	void Scale(float x, float y);
	void Scale(Vector2& vec);
	Vector2& Scale() { return scale; }

	Shader* GetShader() { return shader; }

private:
	void CreateBuffer(); //버퍼 만드는 함수

private:
	struct Vertex
	{
		Vector3 Position;
		Vector2 Uv;
	}vertices[6];

	//화면 계속 따라오게 별도 카메라
	struct ViewProjectionDesc
	{
		Matrix View;
		Matrix Projection;
	}vpDesc;

private:
	Shader * shader;
	ID3D11Buffer* vertexBuffer;//버택스를 보낼 버퍼

	Vector2 position;
	Vector2 scale;

	ID3DX11EffectMatrixVariable* sWorld, *sView, *sProjection;
	ID3DX11EffectShaderResourceVariable* sDiffuseMap; //shader->AsShaderResource("DiffuseMap")
};