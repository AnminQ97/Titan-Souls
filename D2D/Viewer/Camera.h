#pragma once

class Camera
{
public:
	Camera();
	virtual ~Camera();

	D3DXMATRIX View() { return view; }

	void Position( float x, float y); 
	void Position(Vector2& vec);
	D3DXVECTOR2 Position() { return position; }

	virtual void Update();

protected:
	D3DXVECTOR2 position;

private:
	D3DXMATRIX view;
};