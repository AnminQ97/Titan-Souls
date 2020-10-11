#pragma once
#include"Viewer/IFollowing.h"
//Player State 플레이어 상태
enum class PState
{
	start = 0, Walk, stop, Up, UpIdle,
	Down, DownIdle, WA,
	SA, Rolling,RollingUp
};

class Player : public IFollowing
{

public:
	Player(D3DXVECTOR2 position, D3DXVECTOR2 scale);
	virtual ~Player();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	virtual void Render();

	virtual void Focus(D3DXVECTOR2* position, D3DXVECTOR2* size) override;

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
	PState state;

	Collider* collider;

	float moveSpeed;
	D3DXVECTOR2 focusOffset;

	bool bDrawBound;

	UINT pass;
};