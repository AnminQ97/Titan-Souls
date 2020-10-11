#include "stdafx.h"
#include "JumpStatic.h"

JumpStatic::JumpStatic(Vector2 position, Vector2 scale)
	:Player(position,scale), bJump(false), groundY(0)
{

}

JumpStatic::~JumpStatic()
{

}

void JumpStatic::Update(Matrix & V, Matrix & P)
{
	Vector2 position = animation->Position();
	bool bMove = false; //���� = �������� 

	float speed = 160.0f;
	float gravity = -0.15f; //�߷�
	float jumpAccel = 0.25f; //���� �ӵ�

	if (bJump == false) //���� ������
	{
		groundY = position.y; //���� ������ �÷��̾� y��ǥ ����
		moveSpeed.x = 0.0f; //�ӵ� �ٽ� �ʱ�ȭ
		moveSpeed.y = 0.0f; //�ӵ� �ٽ� �ʱ�ȭ

		//����Ű
		if (Key->Down(VK_SPACE))
		{
			bJump = true;
			bMove = true;
			state = PState::Up;
			moveSpeed.y = gravity; //�߷� ����
		}

		if (Key->Press('D')) //������
		{
			bMove = true;
			moveSpeed.x = speed;
			if (bJump == false) 
				state = PState::Walk;
			else
				state = PState::Up; 
			animation->RotationDegree(0, 0, 0);
		}
		else if (Key->Press('A'))
		{
			bMove = true;
			moveSpeed.x = -speed;
			if (bJump == false) 
				state = PState::Walk;
			else
				state = PState::Up;
			animation->RotationDegree(0, 180, 0);
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

		if (Key->Press('W'))
		{
			bMove = true;
			if (bJump == false) 
			{
				moveSpeed.y = speed;
				state = PState::Up;
			}
			else
			{ 
				state = PState::Up;
			}
			animation->RotationDegree(0, 0, 0);
		}
		else if (Key->Press('S'))
		{
			bMove = true;
			if (bJump == false) 
			{
				moveSpeed.y = -speed;
				state = PState::Down;
			}
			else
			{
				state = PState::Up;
			}
		}
		if (Key->Up('W'))
		{
			state = PState::UpIdle;
			animation->RotationDegree(0, 180, 0);
		}
		if (Key->Up('S'))
		{
			state = PState::DownIdle;
			animation->RotationDegree(0, 180, 0);
		}
		
	

	}
	else if (bJump == true) //���� ������
	{
		//�߷�		//-0.15 + 0.25 * Time::Delta();
		moveSpeed.y += jumpAccel * Time::Delta();
		position.y -= moveSpeed.y;  //�÷��̾� - �߷�

		//���� ���̿� ��� ����
		//���� ����
		if (moveSpeed.y < 0.0f) 
		{
			bMove = true;
			state = PState::Up;
		}
		//�������� ���
		if (moveSpeed.y > 0.0f) 
		{
			bMove = true;
			state = PState::WA;
		}
		//����
		if (moveSpeed.y > 0 && position.y <= groundY)
		{
			bJump = false;
			position.y = groundY;
		}
	}

	position += moveSpeed * Time::Delta();

	
	animation->Play((UINT)state);
	animation->Position(position);
	animation->Update(V, P);
}
