#pragma once

#include"GameObject.h"
#include "Skill.h"

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
	_int		iOrder;			// ����(�߽ɿ������� 0~3)
}STAT;

enum class EAnimState
{
	// ������
	IDLE,
	WALK,

	// ����
	COMBAT,			// ���� �� IDLE
	BESHOT,			// �ǰ�
	DEATH,			// ���

	// ���Ϳ�
	CORPSE,			// ��ü

	// ������
	AFFLICTION,		// �ر�
	VIRTUE,			// ���
	ENUM_END
};

class CCreature : public CGameObject
{
public:
	explicit CCreature(LPDIRECT3DDEVICE9 pGraphicDev);

	// ��ų���� ������ ��
	CCreature(LPDIRECT3DDEVICE9 pGraphicDev, STAT _tCommonStat, _int _iPosition,
		shared_ptr<vector<shared_ptr<CSkill>>> _pVecSkill);

	// ���� ��ġ�� ��ų�� ���߿� ������ ��	
	CCreature(LPDIRECT3DDEVICE9 pGraphicDev, STAT _tCommonStat);

	CCreature(const CCreature& rhs);
	virtual ~CCreature();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

public:
	// �ε����� ��ų �ҷ����� (���� �� �غ��� �ҵ�,, ��ų�� �ǳʶٰ� ������ �� �־)
	shared_ptr<CSkill> GetSkill(_int _iIdx) { return (*m_pVecSkill)[_iIdx]; }
	// �̸����� ��ų �ҷ����� (�ϴ� �̰� ���)
	shared_ptr<CSkill> GetSkill(tstring _strSkillName);

	HRESULT	SetSkill(shared_ptr<vector<shared_ptr<CSkill>>> _pSkill);	// ��ų vector�� �� ���� �־��ֱ�

	STAT	GetCommonStat() { return m_tCommonStat; }
	void	SetCommonStat(STAT _tStat) { m_tCommonStat = _tStat; }

	_int	GetPosition() { return m_iPosition; }
	void	SetPosition(_int _iPosition) { m_iPosition += _iPosition; }

	void	SetHitted(_bool _bHitted) { m_bHitted = _bHitted; }
	void	SetEffectOn(_bool _bEffectOn) { m_bEffectOn = _bEffectOn; }

	void	SetBlight(_bool _bBlight) { m_bState[0] = true; }
	void	SetBleed(_bool _bBleed) { m_bState[1] = true; }
	void	SetStun(_bool _bStun) { m_bState[2] = true; }

	_int	GetHp() { return m_tCommonStat.iHp; }
	void	SetHp(_int _iValue) { m_tCommonStat.iHp = _iValue; }
	void	IncreaseHP(_int _iValue) { m_tCommonStat.iHp += _iValue; }
	void	DecreaseHP(_int _iValue) { m_tCommonStat.iHp -= _iValue; }

	_int	GetDodge() { return m_tCommonStat.iDodge; }
	void	SetDodge(_int _iValue) { m_tCommonStat.iDodge = _iValue; }

	_int	GetSpeed() { return m_tCommonStat.iSpeed; }
	void	SetSpeed(_int _iValue) { m_tCommonStat.iSpeed = _iValue; }

	_int	GetAttackPower() { return m_tCommonStat.iAttackPower; }
	void	SetAttackPower(_int _iValue) { m_tCommonStat.iAttackPower = _iValue; }

	_int	GetOrder() { return m_tCommonStat.iOrder; }
	void	SetOrder(_int _iValue) { m_tCommonStat.iOrder = _iValue; }




	// ������??
	_bool	GetDone() { return m_bDone; }
	_bool	GetAbleAct() { return m_bAbleAct; }
	void	SetTurn(_bool _bTurn) { m_bMyTurn = _bTurn; }
	void	SetDone(_bool _bDone) { m_bDone = _bDone; }
	void	SetAbleAct(_bool _bAbleAct) { m_bAbleAct = _bAbleAct; }

protected:
	// �� ���۽�
	virtual void	StartTurn();

	// �ٸ� ũ��ó ���ݽ�
	virtual void	AttackCreature(shared_ptr<CCreature> _pCreature, shared_ptr<CSkill> _pSkill);

	// ���� �����
	virtual void	EndAttack(shared_ptr<CGameObject> _pCreature);

	// �ߵ� ����
	virtual void	BlightAttack(_int* _iDotDam);

	// ���� ����
	virtual void	BleedAttack(_int* _iDotDam);

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

protected:
	STAT		m_tCommonStat;				// ����

	_int		m_iPosition;				// ��ġ (0~3)

	_bool		m_bDeath = false;			// ��� ����
	_bool		m_bState[4] = { false };	// ������� �ߵ�, ����, ����, ��ü ����

	_int		m_bBlightDot[4] = { 0 };	// �ϸ��� �ߵ� ��Ʈ��
	_int		m_bBleedDot[4] = { 0 };		// �ϸ��� ���� ��Ʈ��

	_bool		m_bHitted = false;			// �ڽ��� �ǰݻ��� ����
	_bool		m_bEffectOn = false;		// ����Ʈ ����
	// m_bHitted && m_bEffectOn -> �ǰ� �ִϸ��̼� on
	// !m_bHitted && m_bEffectOn -> Ÿ�� �ִϸ��̼� on
	// ����) ���� ���ݽ� ������ ����Ʈ ���, ���� �ǰ� ����Ʈ ���

	shared_ptr<vector<shared_ptr<CSkill>>>	m_pVecSkill;	// ��ų

	EAnimState m_eAnimState = EAnimState::IDLE;	// �ִϸ��̼� ������ ���� ���°�
	tstring		m_strAnimKey = L"";				// �ִϸ��̼� Ű
	tstring		m_strEffectKey = L"";				// ����Ʈ Ű

	// ������??
	_bool		m_bMyTurn = false;			// �ڽ��� �� ���� 
	_bool		m_bDone = false;			// �̹� �Ͽ� �ൿ �ߴ��� ����
	_bool		m_bAbleAct = true;			// �̹� �Ͽ� �ൿ �������� ����(�����̳� ��ü������� �Ұ���)

protected:
	virtual void	Free();
};
