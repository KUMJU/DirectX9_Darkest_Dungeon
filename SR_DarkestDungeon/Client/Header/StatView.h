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
	void	SettingPos(_vec3 _vPos, _bool _bFront);

public:
	//실제 Creature에서 사용해야하는 수치 조정 함수 -------------------------------------------------------
	void SetStress(int _iStress);
	void SetHp(int _iHp);
	void SetMaxHp(int _iHp) { m_iMaxHp = _iHp; };
	void SetCorpse(_bool _bCorpse) { m_bCorpse = _bCorpse; }

	void SetIsHero(_bool _bIsHero) { m_bHero = _bIsHero; }
	void SetIsTurn(_bool _bIsTurn) { m_bTurn = _bIsTurn; }
	void SetIsTeamTarget(_bool _bIsTurn) { m_bTargetTeam = _bIsTurn; }
	void SetIsTeamsTarget(_bool _bIsTurn) { m_bTargetTeams = _bIsTurn; }
	void SetIsEnemyTarget(_bool _bIsTurn) { m_bTargetEnemy = _bIsTurn; }
	void SetIsEnemiesTarget(_bool _bIsTurn) { m_bTargetEnemies = _bIsTurn; }
	void SetIsTurnOff(_bool _bTurnOff) { m_bTurnOff = _bTurnOff; }

	// 0 : 중독 , 1: 출혈 , 2: 기절 , 3: 기상, 4: 붕괴, 5: 버프, 6: 디버프, 7: 죽음의 일격
	void SetAttribute(_int _iTypeNum) { m_bAttributeArr[_iTypeNum] = true;  }
	void SetAttributeOff(_int _iTypeNum) { m_bAttributeArr[_iTypeNum] = false;  }

	//------------------------------------------------------------------------------------------------------
private:
	void	AddComponent();
	//이 Stat정보를 소유한 크리쳐의 Top Position을 잡아 그에 맞춰 세팅해준다

	//실시간으로 scale이 바뀌는 UI를 확인 후 조정해줍니다
	void	ChangeActiveUI(_float fTimeDelta);

private:

	//0~9 : 스트레스 /10~11 : 체력바 /12~19 :상태값/ 20: 시체 체력바/ 21: 자기차례 표시, 22: 적 타겟 표시, 23: 적들, 24: 팀, 25: 팀들 타겟, 26: 턴표시
	shared_ptr<CRcTex> m_pRCTexCom[27];
	shared_ptr<CTransform> m_pTransformCom[27];
	// 0:스트레스X 1:스트레스o 2:빈 HP바 3:HP바 4: 중독 5:출혈 6:기절 7: 기상 8: 붕괴 9: 버프
	// 10: 디버프 11: 죽음의 일격 12: 시체 HP바 13: 차례 표시, 14: 적 타겟 표시, 15: 적들 타겟 표시
	// 16: 팀 타겟 표시, 17: 팀들 타겟 표시
	shared_ptr<CTexture> m_pTextureCom[19];

	_bool m_bAttributeArr[8] = {false, false, false, false, false, false, false, false };

	_vec3 m_vCenterPos; //스탯 정보창의 중점 좌표 

	_int m_iStress = 0;
	_int m_iHp;
	_int m_iMaxHp;

	//현재 스트레스 UI ON 갯수
	_int m_iStressNum = 0;
	//전체 체력에 대해 현재 체력 비율
	_float m_fHpBarRange = 1.f;
	//체력 차감 바 비율
	_float m_fHpGapRange = 0.f;
	_int m_iGapHp = 0.f;

	_bool m_bHero = true;
	_bool m_bCorpse = false;
	_bool m_bTurn = false;

	_bool m_bTargetTeam = false;
	_bool m_bTargetTeams = false;
	_bool m_bTargetEnemy = false;
	_bool m_bTargetEnemies = false;
	_bool m_bTurnOff = false;

	_bool	m_bHpGap = false;
	_float	m_fHpRegemTime = 0.f;
};

