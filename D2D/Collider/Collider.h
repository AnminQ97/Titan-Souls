#pragma once

class Collider
{
public:
	Collider();
	~Collider();

	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	void World(D3DXMATRIX& world) { this->world = world; }
	D3DXMATRIX World() { return world; }

	static bool Aabb(D3DXMATRIX& world, D3DXVECTOR2& position);
	static bool Aabb(D3DXMATRIX& world1, D3DXMATRIX& world2);

	void DrawColliderColor(bool val) { bDrawColliderColor = val; }

private:
	void CreateBuffer();

private:
	struct Vertex
	{
		D3DXVECTOR3 Positon;
	};
	
private:
	struct ObbDesc
	{
		D3DXVECTOR2 HalfSize;

		D3DXVECTOR2 Right;
		D3DXVECTOR2 Up;

		D3DXVECTOR2 Length_Right;
		D3DXVECTOR2 Length_Up;
	};

public:
	static bool Obb(Sprite* a, Sprite* b);

private:
	static void CreateObb(ObbDesc* out, D3DXVECTOR2& half, D3DXMATRIX& transform);
	static float SeparateAxis(D3DXVECTOR2& init, D3DXVECTOR2& e1, D3DXVECTOR2& e2);

private:
	Shader* shader;
	ID3D11Buffer* vertexBuffer;
	
	D3DXMATRIX world;

	D3DXCOLOR initLineColor;
	D3DXCOLOR collidedColor;
	bool bDrawColliderColor;

	ID3DX11EffectMatrixVariable* sWorld;
	ID3DX11EffectMatrixVariable* sView;
	ID3DX11EffectMatrixVariable* sProjection;
	ID3DX11EffectVectorVariable* sColor;
};