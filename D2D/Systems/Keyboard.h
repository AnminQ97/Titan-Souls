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

//[]배열 기호로도 사용 가능함
//set(i) : 인덱스의 값을 true, false로 세팅
//resut() : 인덱스의 값을 false로 세팅
//flip() : false -> true, true -> false 로 전환
//all() : 모든 원소의 값이 true이면 true 반환
//none() : 모든 원소의 값이 false이면 true 반환
//any() : 1개라도 true면 true
//count() : true의 개수를 리턴