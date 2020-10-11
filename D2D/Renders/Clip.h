#pragma once

enum class PlayMode { End = 0 , Loop };

struct Frame
{
	Sprite* Image;
	float Time;

	Frame(Sprite* image, float time);
	~Frame();

};

class Clip
{
public:
	Clip(PlayMode mode = PlayMode::End, float speed = 1.0f);
	~Clip();

	void Position(float x, float y);
	void Position(D3DXVECTOR2 vec);
	D3DXVECTOR2 Position() { return position; }

	void Scale(float x, float y);
	void Scale(D3DXVECTOR2 vec);
	D3DXVECTOR2 Scale() { return scale; }

	void Rotation(float x, float y, float z);
	void Rotation(D3DXVECTOR3& vec);
	D3DXVECTOR3 Rotation() { return rotation; }

	void RotationDegree(float x, float y, float z);
	void RotationDegree(D3DXVECTOR3& vec);
	D3DXVECTOR3 RotationDegree();

	void AddFrame(Sprite* sprite, float time);

	void Play();
	void Play(UINT startFrame);
	void Stop();

	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	D3DXVECTOR2 TextureSize();

	Sprite* CurrentSprite();

	bool EndFrame() { return !bPlay; } //끝까지 재생 됐는지 //! 결과 뒤집기

	void Pass(UINT val);

private:
	D3DXVECTOR2 position;
	D3DXVECTOR2 scale;
	D3DXVECTOR3 rotation;

	float length;
	float speed;
	bool bPlay;
	UINT currentFrame;
	float playTime;

	PlayMode mode;

	vector<Frame*> frames;

	
};