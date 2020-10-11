#include"stdafx.h"
#include"SoundTool.h"

SoundTool::SoundTool()
	:buffer(15), volume(1.0f)
{
	System_Create(&system);

	system->init(buffer, FMOD_INIT_NORMAL, NULL);
	sound = new Sound* [buffer];
	channel = new Channel*[buffer];
	
	memset(sound, NULL, sizeof(Sound*) * buffer); //시작주소부터 버퍼 끝까지 null값 넣기
	memset(channel, NULL, sizeof(Channel*) * buffer);
}

SoundTool::~SoundTool()
{
	if (channel != NULL)
	{
		for (UINT i = 0; i < buffer; i++)
			if (channel[i]) channel[i]->stop(); //돌면서 사용중인 채널 중지
	}

	if (sound != NULL)
	{
		for (UINT i = 0; i < buffer; i++)
			if (sound[i]) sound[i]->release(); //돌면서 사용중인 sound 중지
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
	if (bLoop == true) //반복재생 이면
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

	sounds[name] = &sound[sounds.size()]; //음원에 이름 넣기

}

void SoundTool::Play(string name, float volume)
{
	int count = 0;

	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter, count++)
	{
		if (name == iter->first)
		{
			system->playSound(FMOD_CHANNEL_FREE,*iter->second,false,&channel[count]); //비어있는 채널을 사용해서 재생
			
			channel[count]->setVolume(volume); //현재 채널 음원 볼륨
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

void SoundTool::Pause(string name) //일시정지
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

void SoundTool::Resume(string name) //다시재생
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
	return bPlay;//재생중인지 아닌지
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
	return bPaused;//정지중인지
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
			break; //break빼면 모든소리 조절
		}
	}
}
