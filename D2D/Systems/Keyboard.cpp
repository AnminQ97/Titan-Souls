#include "stdafx.h"
#include "Keyboard.h"

Keyboard::Keyboard()
{
	for (int i = 0; i < KEYMAX; i++)
	{
		up.set(i, false);
		down.set(i, false);
	}
}

Keyboard::~Keyboard()
{
}

bool Keyboard::Down(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (down[key] == false)
		{
			down.set(key, true);
			return true;
		}
	}
	else
	{
		down.set(key, false);		
	}

	return false;
}

bool Keyboard::Up(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		up.set(key, true);
	}
	else
	{
		if (up[key] == true)
		{
			up.set(key, false);
			return true;
		}
	}

	return false;
}

bool Keyboard::Press(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
		return true;

	return false;
}

bool Keyboard::Toggle(int key)
{
	if (GetAsyncKeyState(key) & 0x0001)
		return true;

	return false;
}
