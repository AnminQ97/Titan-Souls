#pragma once
#include "Objects/Player.h"

class PlayerMove : public Player
{
public:
	PlayerMove(Vector2 position, Vector2 scale);
	~PlayerMove();

	void Update(Matrix&V, Matrix&P);

private:
	Vector2 moveSpeed; //�̵��ӵ� , ������ �ӵ�
	bool roll;
	int a;
};