#pragma once
#include "Monster.h"

class CBrigandCutthroat : public CMonster
{
public:
	explicit CBrigandCutthroat(LPDIRECT3DDEVICE9 pGraphicDev);
	CBrigandCutthroat(const CCreature& rhs);
	virtual ~CBrigandCutthroat();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

protected:
	// 턴 시작시
	virtual void	StartTurn();

	// 다른 크리처 공격시
	virtual void	AttackCreature(shared_ptr<CGameObject> _pCreature, _float _fSkillRatio, EAttackType _eAttackTYPE,
		_int _iDotDamage = 0, _int _iTurn = 0);

	// 공격 종료시
	virtual void	EndAttack(shared_ptr<CGameObject> _pCreature);

	// 중독 공격
	virtual void	BlightAttack(_int _iBlightDmg, _int _iTurn);

	// 출혈 공격
	virtual void	BleedAttack(_int _iBleedDmg, _int _iTurn);

	// 중독 치료
	virtual void	BlightCure();

	// 출혈 치료
	virtual void	BleedCure();

private:
	virtual void			AddComponent();
	virtual void			ClimbingTerrain();

private:
	virtual void	Free();
};

