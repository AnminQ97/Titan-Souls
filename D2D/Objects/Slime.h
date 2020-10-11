#pragma once
#include"Viewer/IFollowing.h"
#include "SlimeShader.h"
//Slime State 플레이어 상태
enum class EState 
{
	Idle,walk
};

class Slime : public SlimeShader
{

public:
	Slime(D3DXVECTOR2 position, D3DXVECTOR2 scale);
	~Slime();

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

	void Pass(UINT val) { pass = val; }

	Sprite* GetSprite();

protected:
	Animation * animation;
	EState state;

	Collider* collider;

	float moveSpeed;

	bool bDrawBound;

	//float x;
	//float y;
	float size;
	bool slimeIdle = true;
	UINT pass;
};