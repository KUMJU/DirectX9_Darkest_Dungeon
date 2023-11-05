#pragma once
#include"Engine_Define.h"
#include<fmod.h>

enum class CHANNELID {
	BGM, PLAYER, HERO1, HERO2 , HERO3 , HERO4, MONSTER, BOSS, EFFECT, ENUM_END
};

class CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)

public:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize();
	void Release();

public:
	void PlaySound(const TCHAR* _pSoundKey, CHANNELID eID, _float _fVolume);
	void PlayBGM(const TCHAR* _pSoundKey, _float _fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();

private:
	void LoadSoundFile();

private:
	
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[(unsigned long long)CHANNELID::ENUM_END];
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;

};

