#pragma once

#include"GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimator;

END

// Creature ���� ����
typedef struct tagCreatureCommonStat
{
	_int		iHp;			// ü��
	_int		iDodge;			// ȸ����
	_int		iSpeed;			// �ӵ�
	_int		iAttackPower;	// �������ݷ�(���� �������� ��ų�� ���� ���ݷ� ����� ��)

}STAT;

// Attack Ÿ��
enum class ATTACKTYPE
{ 
	ATTACK_NORMAL, 
	ATTACK_BLIGHT, 
	ATTACK_BLEED, 
	ATTACK_STRESS
};

class CCreature : public CGameObject
{
public:
	explicit CCreature(LPDIRECT3DDEVICE9 pGraphicDev);
	CCreature(const CCreature& rhs);
	virtual ~CCreature();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

public:
	STAT	GetCommonStat() { return m_tCommonStat; }
	void	SetCommonStat(STAT _tStat) { m_tCommonStat = _tStat; }
	void	SetTurn(_bool _bTurn) { m_bMyTurn = _bTurn; }
	void	SetHitted(_bool _bHitted) { m_bHitted = _bHitted; }
	void	SetEffectOn(_bool _bEffectOn) { m_bEffectOn = _bEffectOn; }
	void	SetBlight(_bool _bBlight) { m_bBlighting = true; }
	void	SetBleed(_bool _bBleed) { m_bBleeding = true; }

protected:
	// �� ���۽�
	virtual void	StartTurn();

	// �ٸ� ũ��ó ���ݽ�
	virtual void	AttackCreature(CCreature* _pCreature, _float _fSkillRatio, ATTACKTYPE _eAttackTYPE,
		_int _iDotDamage = 0, _int _iTurn = 0);

	// ���� �����
	virtual void	EndAttack(CCreature* _pCreature);

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
	// Creature Common Component
	// ��ü ���� Component
	shared_ptr<CTransform> m_pTransformCom = nullptr;
	shared_ptr<CRcTex> m_pBufCom = nullptr;
	shared_ptr<CAnimator> m_pTextureCom = nullptr;

	// ��ü �ǰ�, Ÿ�� ����Ʈ ���� Component
	shared_ptr<CTransform> m_pEffectTransformCom = nullptr;
	shared_ptr<CRcTex> m_pEffectBufCom = nullptr;
	shared_ptr<CAnimator> m_pEffectTextureCom = nullptr;

	// ����) ���� ���ݽ� ������ ����Ʈ ���, ���� �ǰ� ����Ʈ ���

protected:
	STAT		m_tCommonStat;				// ����
	_bool		m_bMyTurn = false;			// �ڽ��� �� ���� 

	_bool		m_bHitted = false;			// �ڽ��� �ǰݻ��� ����
	_bool		m_bEffectOn = false;		// ����Ʈ ����
	// m_bHitted && m_bEffectOn -> �ǰ� �ִϸ��̼� on
	// !m_bHitted && m_bEffectOn -> Ÿ�� �ִϸ��̼� on

	_bool		m_bBlighting = false;			// �ߵ�����
	_int		m_bBlightDot[4] = { 0 };	// �ϸ��� �ߵ� ��Ʈ��
	_bool		m_bBleeding = false;			// ��������
	_int		m_bBleedDot[4] = { 0 };		// �ϸ��� ���� ��Ʈ��

protected:
	virtual void	Free();
};

