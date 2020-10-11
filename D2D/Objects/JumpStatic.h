#pragma once
#include "Objects/Player.h"

class JumpStatic : public Player
{
public:
	JumpStatic(Vector2 position, Vector2 scale);
	~JumpStatic();

	void Update(Matrix&V,Matrix&P);

	float GetHeight() { return moveSpeed.y; }



private:
	Vector2 moveSpeed; //이동속도 , 점프속도
	bool bJump;
	float groundY; //뛰기 전 플레이어 y값
	int a = 0;
};