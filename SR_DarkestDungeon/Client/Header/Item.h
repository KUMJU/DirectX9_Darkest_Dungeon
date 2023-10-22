#pragma once

#include"GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimator;

END

class CItem : public CGameObject
{
public:
	explicit CItem(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CItem();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

public:
	//true일 경우 필드 위에 배치된 아이템이고, false일 시 인벤토리에 들어간 아이템
	void SetOnField(_bool _bValue) { 
		m_bOnField = _bValue; 
	}
	_bool GetOnFieldState() { return m_bOnField; }

	//몬스터가 죽고 아이템이 드랍될 때 포지션, 텍스쳐 값을 지정해줄 수 있음(필수로 해줄 것)
	void SetDropItemInfo(_vec3 _vPosition, const tstring& _strName);
	
protected:
	void AddComponent();
	//필드에 떠있을때 위 아래로 둥둥 띄워주는 효과
	void FloatingOnField();

private:
	//Componenet
	shared_ptr<CTransform> m_pTransCom = nullptr;
	shared_ptr<CRcTex> m_pBufCom = nullptr;
	shared_ptr<CTexture> m_pTextureCom = nullptr;

private:
	//Member variable
	tstring m_strItemKey = L"";
	_bool	m_bOnField = true; // 필드에 있는 아이템인지 인벤토리에 있는 아이템인지 식별
	_vec3	m_vPosition;


	_float m_fTime = 0.f;
	_float m_CurrentDir = 1.f;
	_float m_fActTime = 0.f;
	_float m_fTotalHeight = 0.f;

};

