#pragma once

using namespace FMOD;

class SoundTool
{
public:
	SoundTool();
	~SoundTool();
							   //파일경로		 //무한반복
	void AddSound(string name, string sounFile, bool bLoop = false);//외부에서 음원 추가
	void Play(string name, float volume = 1.0f);
	void Stop(string name);
	void Pause(string name);
	void Resume(string name);

	bool Playing(string name);
	bool Paused(string name);

	void Update();
	void Volume(string name, float volume);
	float Volume() { return volume; }

private:
	System * system; //사운드 장치 접근
	Sound** sound; //동적 배열을 위한 더블포인터
	Channel** channel; //통로 //DX에서의 buffer 같은느낌

	UINT buffer; //동시출력 갯수

	map<string, Sound**> sounds; //<음원이름 , 음원>
	map<string, Sound**>::iterator iter;

	float volume; //볼륨

};