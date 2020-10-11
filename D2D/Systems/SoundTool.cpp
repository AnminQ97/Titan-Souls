#include"stdafx.h"
#include"SoundTool.h"

SoundTool::SoundTool()
	:buffer(15), volume(1.0f)
{
	System_Create(&system);

	system->init(buffer, FMOD_INIT_NORMAL, NULL);
	sound = new Sound* [buffer];
	channel = new Channel*[buffer];
	
	memset(sound, NULL, sizeof(Sound*) * buffer); //�����ּҺ��� ���� ������ null�� �ֱ�
	memset(channel, NULL, sizeof(Channel*) * buffer);
}

SoundTool::~SoundTool()
{
	if (channel != NULL)
	{
		for (UINT i = 0; i < buffer; i++)
			if (channel[i]) channel[i]->stop(); //���鼭 ������� ä�� ����
	}

	if (sound != NULL)
	{
		for (UINT i = 0; i < buffer; i++)
			if (sound[i]) sound[i]->release(); //���鼭 ������� sound ����
	}


	SAFE_DELETE_ARRAY(channel);
	SAFE_DELETE_ARRAY(sound);

	if (system != NULL)
	{
		system->release();
		system->close();
	}

	sounds.clear();
}

void SoundTool::AddSound(string name, string sounFile, bool bLoop)
{
	if (bLoop == true) //�ݺ���� �̸�
	{
		system->createStream
		(
			sounFile.c_str(),
			FMOD_LOOP_NORMAL,
			NULL,
			&sound[sounds.size()]
		);
		return;
	}
	
	system->createStream
	(
		sounFile.c_str(),
		FMOD_DEFAULT,
		NULL,
		&sound[sounds.size()]
	);

	sounds[name] = &sound[sounds.size()]; //������ �̸� �ֱ�

}

void SoundTool::Play(string name, float volume)
{
	int count = 0;

	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter, count++)
	{
		if (name == iter->first)
		{
			system->playSound(FMOD_CHANNEL_FREE,*iter->second,false,&channel[count]); //����ִ� ä���� ����ؼ� ���
			
			channel[count]->setVolume(volume); //���� ä�� ���� ����
		}
	}
}

void SoundTool::Stop(string name)
{
	int count = 0;

	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter, count++)
	{
		if (name == iter->first)
		{
			channel[count]->stop();
			break;
		}
	}
}

void SoundTool::Pause(string name) //�Ͻ�����
{
	int count = 0;

	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter, count++)
	{
		if (name == iter->first)
		{
			channel[count]->setPaused(true);
			break;
		}
	}
}

void SoundTool::Resume(string name) //�ٽ����
{
	int count = 0;

	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter, count++)
	{
		if (name == iter->first)
		{
			channel[count]->setPaused(false);
			break;
		}
	}
}

bool SoundTool::Playing(string name)
{
	bool bPlay = false;
	int count = 0;

	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter, count++)
	{
		if (name == iter->first)
		{
			channel[count]->isPlaying(&bPlay); 
			break;
		}
	}
	return bPlay;//��������� �ƴ���
}

bool SoundTool::Paused(string name)
{
	bool bPaused = false;
	int count = 0;

	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter, count++)
	{
		if (name == iter->first)
		{
			channel[count]->getPaused(&bPaused); 
			break;
		}
	}
	return bPaused;//����������
}

void SoundTool::Update()
{
	system->update();
}

void SoundTool::Volume(string name, float volume)
{
	int count = 0;

	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter, count++)
	{
		if (name == iter->first)
		{
			channel[count]->setVolume(volume);
			break; //break���� ���Ҹ� ����
		}
	}
}
