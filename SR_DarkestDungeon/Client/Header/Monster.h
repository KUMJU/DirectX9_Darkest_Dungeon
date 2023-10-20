#pragma once
#include "Creature.h"

class CMonster : public CCreature
{
public:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	CMonster(const CCreature& rhs);
	virtual ~CMonster();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

protected:
	// �� ���۽�
	virtual void	StartTurn();

	// �ٸ� ũ��ó ���ݽ�
	virtual void	AttackCreature(shared_ptr<CGameObject> _pCreature, _float _fSkillRatio, ATTACKTYPE _eAttackTYPE,
		_int _iDotDamage = 0, _int _iTurn = 0);

	// ���� �����
	virtual void	EndAttack(shared_ptr<CGameObject> _pCreature);

	// �ߵ� ����
	virtual void	BlightAttack(_int _iBlightDmg, _int _iTurn);

	// ���� ����
	virtual void	BleedAttack(_int _iBleedDmg, _int _iTurn);

	// �ߵ� ġ��
	virtual void	BlightCure();

	// ���� ġ��
	virtual void	BleedCure();

protected:
	virtual void			AddComponent();
	virtual void			ClimbingTerrain();

protected:
	virtual void	Free();
};

