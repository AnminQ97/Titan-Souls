#pragma once

class SlimeShader
{

public:
	SlimeShader(D3DXVECTOR2 position, D3DXVECTOR2 scale);
	~SlimeShader();

	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();


	void Position(float x, float y);
	void Position(D3DXVECTOR2& position);
	D3DXVECTOR2 Position();

	void Scale(float x, float y);
	void Scale(D3DXVECTOR2& scale);
	D3DXVECTOR2 Scale();

	void MveSpeed(float val) { moveSpeed = val; }
	float MoveSpeed() { return moveSpeed; }

	Vector2 ScaledSize();

	Matrix World();

	void DrawBound(bool val) { bDrawBound = val; }

	Collider* GetCollider() { return collider; }

	Sprite* GetSprite();

protected:
	Animation * animation;

	Collider* collider;
	float x;
	float y;
	bool slimeIdle = true;
	float moveSpeed;

	bool bDrawBound;
};