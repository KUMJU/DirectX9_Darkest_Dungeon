#pragma once

#include"Engine_Define.h"
#include"GameObject.h"

BEGIN(Engine)

//State는 같이 생각해봐야 할 것 같음
enum class EGameState {
	PRCESS,
	PAUSE,
	LOCK,
	ENUM_END
};

class ENGINE_DLL CGameMgr
{
	DECLARE_SINGLETON(CGameMgr)

private:
	explicit CGameMgr();
	~CGameMgr();

public:
	EGameState GetGameState() { return m_eCurrentState; }
	void SetGameState(EGameState _state) { m_eCurrentState = _state; }

public:
	shared_ptr<CGameObject> GetPlayer() { return m_pPlayer; }
	void SetPlayer(shared_ptr<CGameObject> _player) { m_pPlayer = _player; }

private:
	shared_ptr<CGameObject> m_pPlayer;
	EGameState m_eCurrentState;
};

END
