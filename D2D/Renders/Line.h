#pragma once

class Line
{
public:
	Line(Vector2 a, Vector2 b);
	~Line();

	void Update(Matrix& V, Matrix& P);
	void Render();

	void Left(Vector2& vec) { left = vec; } //왼쪽 점 set
	void Right(Vector2& vec) { right = vec; } //오른쪽 점 set

	Vector2 Left() { return left; }
	Vector2 Right() { return right; }

	void Position(float x, float y);
	void Position(Vector2& vec);
	Vector2 Position() { return position; }

	void Scale(float x, float y);
	void Scale(Vector2& vec);
	Vector2 Scale() { return scale; }

	void DrawCollision(bool val) { bDrawCollision = val; }

private:
	void CreateBuffer(Vector2 a, Vector2 b); //라인 시작점 끝점

private:
	struct Vertex
	{
		Vector3 Position; //정점위치

	} vertices[2];

private:
	Shader* shader;
	ID3D11Buffer* vertexBuffer;
	Vector2 position; //월드 위치
	Vector2 scale;

	Vector2 left; //왼쪽 끝점
	Vector2 right; //오른쪽 끝점

	ID3DX11EffectMatrixVariable* sWorld;
	ID3DX11EffectMatrixVariable* sView;
	ID3DX11EffectMatrixVariable* sProjectiion;
	
	bool bDrawCollision;
};