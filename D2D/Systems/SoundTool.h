#pragma once

using namespace FMOD;

class SoundTool
{
public:
	SoundTool();
	~SoundTool();
							   //���ϰ��		 //���ѹݺ�
	void AddSound(string name, string sounFile, bool bLoop = false);//�ܺο��� ���� �߰�
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
	System * system; //���� ��ġ ����
	Sound** sound; //���� �迭�� ���� ����������
	Channel** channel; //��� //DX������ buffer ��������

	UINT buffer; //������� ����

	map<string, Sound**> sounds; //<�����̸� , ����>
	map<string, Sound**>::iterator iter;

	float volume; //����

};