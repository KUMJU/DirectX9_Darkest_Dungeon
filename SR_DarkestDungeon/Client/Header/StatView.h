#pragma once

#include"GameObject.h"


BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END


class CStatView : public CGameObject
{
public:
	explicit CStatView(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CStatView();
public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

public:
	//초기화
	void	SettingInit(_vec3 _vCreaturePos, _int _iHp, _int _iMaxHp, _bool _isHero);
	void	SettingPos(_vec3 _vPos);

public:
	//실제 Creature에서 사용해야하는 수치 조정 함수 -------------------------------------------------------
	void SetStress(int _iStress);
	void SetHp(int _iHp);
	void SetMaxHp(int _iHp) { m_iMaxHp = _iHp; };

	void SetIsHero(_bool _bIsHero) { m_bHero = _bIsHero; }

	// 0 : 중독 , 1: 출혈 , 2: 기절 , 3: 기상, 4: 붕괴
	void SetAttribute(_int _iTypeNum) { m_bAttributeArr[_iTypeNum] = true;  }
	void SetAttributeOff(_int _iTypeNum) { m_bAttributeArr[_iTypeNum] = false;  }

	//------------------------------------------------------------------------------------------------------
private:
	void	AddComponent();
	//이 Stat정보를 소유한 크리쳐의 Top Position을 잡아 그에 맞춰 세팅해준다

private:

	//0~9 : 스트레스 /10~11 : 체력바 /12~16 :상태값 
	shared_ptr<CRcTex> m_pRCTexCom[17];
	shared_ptr<CTransform> m_pTransformCom[17];
	//0:스트레스X 1:스트레스o 2:빈 HP바 3:HP바 4: 중독 5:출혈 6:기절 7: 기상 8: 붕괴
	shared_ptr<CTexture> m_pTextureCom[9];

	_bool m_bAttributeArr[5] = {false, false, false, false, false};

	_vec3 m_vCenterPos; //스탯 정보창의 중점 좌표 

	_int m_iStress = 0;
	_int m_iHp;
	_int m_iMaxHp;

	//현재 스트레스 UI ON 갯수
	_int m_iStressNum = 0;
	//전체 체력에 대해 현재 체력 비율
	_float m_fHpBarRange = 1.f;

	_bool m_bHero = true;

};

