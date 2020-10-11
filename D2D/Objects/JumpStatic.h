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
	Vector2 moveSpeed; //�̵��ӵ� , �����ӵ�
	bool bJump;
	float groundY; //�ٱ� �� �÷��̾� y��
	int a = 0;
};