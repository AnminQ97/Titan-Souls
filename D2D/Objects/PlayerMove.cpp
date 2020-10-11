#include "stdafx.h"
#include "PlayerMove.h"

PlayerMove::PlayerMove(Vector2 position, Vector2 scale)
	:Player(position,scale), roll(false),a(0)
{

}

PlayerMove::~PlayerMove()
{

}

void PlayerMove::Update(Matrix & V, Matrix & P)
{
	Vector2 position = animation->Position();
	bool bMove = false; //���� = �������� 

	float speed = 160.0f;
	float jumpAccel = 0.25f; //���� �ӵ�

	if (Key->Down(VK_SPACE)) //������
	{
		roll = true;
	}
	
	if (roll)
	{
		switch (a)
		{
		case 0:	state = PState::Rolling; break;
		case 1:	state = PState::RollingUp; break;
		}
		if (animation->GetClip()->EndFrame() == true)
		{
			roll = false;
		}
	}
	

	if (roll == false) //���� ������
	{
		moveSpeed.x = 0.0f; //�ӵ� �ٽ� �ʱ�ȭ
		moveSpeed.y = 0.0f; //�ӵ� �ٽ� �ʱ�ȭ

		
		// �� ��
		if (Key->Press('W') && Key->Press('A'))
		{
			bMove = true;
			moveSpeed.y = speed;
			moveSpeed.x = -speed;
			state = PState::WA;
			animation->RotationDegree(0, 180, 0);
		}
		//���� ��
		else if (Key->Press('W') && Key->Press('D'))
		{
			bMove = true;
			moveSpeed.y = speed;
			moveSpeed.x = speed;
			state = PState::WA;
			animation->RotationDegree(0, 0, 0);
		}
		// �޾Ʒ�
		else if (Key->Press('S') && Key->Press('A'))
		{
			bMove = true;
			moveSpeed.y = -speed;
			moveSpeed.x = -speed;
			state = PState::SA;
			animation->RotationDegree(0, 0, 0);
		}
		//���� �Ʒ�
		else if (Key->Press('S') && Key->Press('D'))
		{
			bMove = true;
			moveSpeed.y = -speed;
			moveSpeed.x = speed;
			state = PState::SA;
			animation->RotationDegree(0, 180, 0);
		}
		//----
		//�¿�					
		else if (Key->Press('D')) //������
		{
			bMove = true;
			moveSpeed.x = speed;
			state = PState::Walk;

			a = 0;

			animation->RotationDegree(0, 0, 0);
		}
		else if (Key->Press('A'))
		{
			bMove = true;
			moveSpeed.x = -speed;
			state = PState::Walk;

			a = 0;

			animation->RotationDegree(0, 180, 0);
		}
		//----
		//����
		else if (Key->Press('W'))
		{
			bMove = true;
			moveSpeed.y = speed;
			state = PState::Up;
			a = 1;
		}
		else if (Key->Press('S'))
		{
			bMove = true;
			moveSpeed.y = -speed;
			state = PState::Down;
		}

		//Idle
		if (Key->Up('W'))
		{
			state = PState::UpIdle;
		}
		if (Key->Up('S'))
		{
			state = PState::DownIdle;
		}
		if (Key->Up('A'))
		{
			state = PState::stop;
			animation->RotationDegree(0, 180, 0);
		}
		if (Key->Up('D'))
		{
			state = PState::stop;
			animation->RotationDegree(0, 0, 0);
		}

	}

	position += moveSpeed * Time::Delta();


	animation->Play((UINT)state);
	animation->Position(position);
	animation->Update(V, P);
}
