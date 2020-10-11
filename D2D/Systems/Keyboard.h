#pragma once

#define KEYMAX 256

class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	bool Down(int key);
	bool Up(int key);
	bool Press(int key);
	bool Toggle(int key);

private:
	bitset<KEYMAX> up;
	bitset<KEYMAX> down;
};

//[]�迭 ��ȣ�ε� ��� ������
//set(i) : �ε����� ���� true, false�� ����
//resut() : �ε����� ���� false�� ����
//flip() : false -> true, true -> false �� ��ȯ
//all() : ��� ������ ���� true�̸� true ��ȯ
//none() : ��� ������ ���� false�̸� true ��ȯ
//any() : 1���� true�� true
//count() : true�� ������ ����