#pragma once
#include "GameObject.h"

class CCardGame :public CGameObject
{
public:
	explicit CCardGame(LPDIRECT3DDEVICE9 pGraphicDev);
	~CCardGame();

public:
	void	Start();
	void	End();

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
};

