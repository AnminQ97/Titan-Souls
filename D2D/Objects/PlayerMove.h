#pragma once
#include "Objects/Player.h"

class PlayerMove : public Player
{
public:
	PlayerMove(Vector2 position, Vector2 scale);
	~PlayerMove();

	void Update(Matrix&V, Matrix&P);

private:
	Vector2 moveSpeed; //이동속도 , 구르기 속도
	bool roll;
	int a;
};