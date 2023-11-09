#include "pch.h"
#include "Creature.h"
#include "Hero.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include"StatView.h"

#include "EffectMgr.h"
#include "Effect.h"

CCreature::CCreature(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	memset(&m_tCommonStat, NULL, sizeof(STAT));
	ZeroMemory(m_szString, sizeof(m_szString));
	ZeroMemory(m_szString2, sizeof(m_szString2));
	ZeroMemory(m_szString3, sizeof(m_szString3));
}

CCreature::CCreature(LPDIRECT3DDEVICE9 pGraphicDev, STAT _tCommonStat, _int _iPosition,
	vector<shared_ptr<CSkill>>& _pVecSkill)
	: CGameObject(pGraphicDev), m_tCommonStat(_tCommonStat), m_iPosition(_iPosition), m_pVecSkill(_pVecSkill)
{
}

CCreature::CCreature(LPDIRECT3DDEVICE9 pGraphicDev, STAT _tCommonStat)
	: CGameObject(pGraphicDev), m_tCommonStat(_tCommonStat)
{
}

CCreature::CCreature(const CCreature& rhs)
	: Engine::CGameObject(rhs)
{
}

CCreature::~CCreature()
{
}

HRESULT CCreature::ReadyGameObject()
{
	__super::ReadyGameObject();
	SettingStatBar();

	return S_OK;
}

_int CCreature::UpdateGameObject(const _float& fTimeDelta)
{
	_int	iExit = CGameObject::UpdateGameObject(fTimeDelta);

	m_vPos = *m_pTransformCom->GetPos();

	//������ ����
	_matrix matWorld;

	matWorld = *m_pTransformCom->GetWorld();
	SetBillBoard(matWorld);
	m_pTransformCom->SetWorld(&matWorld);

	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());

	// ���Ȱ���
	m_pStatInfo->SetHp(m_tCommonStat.iHp);
	for (int i = 0; i < 3; i++)
	{
		if(m_bState[i])
			m_pStatInfo->SetAttribute(i);
		else
			m_pStatInfo->SetAttributeOff(i);
	}
	// ���, �ر�

	// ����, �����
	for (int i = 5; i < 7; i++)
	{
		if (m_bState3[i - 5])
			m_pStatInfo->SetAttribute(i);
		else
			m_pStatInfo->SetAttributeOff(i);
	}

	// ������ �ϰ� ǥ��
	if (m_bBeforeDeath)	m_pStatInfo->SetAttribute(7);
	else
		m_pStatInfo->SetAttributeOff(7);

	if (m_pStatInfo != nullptr && bStatBarOn) {
		m_pStatInfo->SettingPos(*(m_pTransformCom->GetPos()), m_bFront);
		m_pStatInfo->UpdateGameObject(fTimeDelta);
	}

	if (m_pEffect && m_pEffect->GetIsActive())
	{
		m_pEffect->UpdateGameObject(fTimeDelta);
	}

	return iExit;
}

void CCreature::LateUpdateGameObject()
{
	ClimbingTerrain();
}

void CCreature::RenderGameObject()
{
	TCHAR szBuff[32] = { };
	_stprintf_s(szBuff, TEXT("%d"), GetHp());
	lstrcpy(m_szString, szBuff);

	TCHAR szBuff2[32] = { };
	_stprintf_s(szBuff, TEXT("%d"), GetCurrentBleed());
	lstrcpy(m_szString2, szBuff);

	TCHAR szBuff3[32] = { };
	_stprintf_s(szBuff, TEXT("%d"), GetCurrentPoision());
	lstrcpy(m_szString3, szBuff);

	if (m_pStatInfo != nullptr && bStatBarOn)
		m_pStatInfo->RenderGameObject();

	if (m_pEffect && m_pEffect->GetIsActive())
		m_pEffect->RenderGameObject();

}

_bool CCreature::IsAttacking()
{
	for (int i = 0; i < 7; i++)
	{
		if (m_bAttacking[i])
		{
			return true;
		}
	}
	return false;
}

//shared_ptr<CSkill> CCreature::GetSkill(tstring _strSkillName)
//{
//	for (auto iter : *m_pVecSkill)
//	{
//		// ���� �ʿ� // �̷��� ���ص� �Ǵ��� Ȯ���غ��ߵ�
//		if (iter->GetSkillName() == _strSkillName)
//		{
//			return iter;
//		}
//	}
//
//	return nullptr;
//}

//HRESULT CCreature::SetSkill(shared_ptr<vector<shared_ptr<CSkill>>> _pSkill)
//{
//	m_pVecSkill = _pSkill;
//
//	return S_OK;
//}

void CCreature::StartCalculate()
{
	// �����̳� �ߵ� ���¶��
	if (m_bState[0] || m_bState[1])
	{
		// ��Ʈ�� ���
		m_tCommonStat.iHp -= (m_iBlightDot[0] + m_iBleedDot[0]);
		// ������� �ʾ������� �ǰ� 0�Ʒ��� �ȴ�� �ϱ�
		if (!m_bDeath)
		{
			if (m_tCommonStat.iHp < 0)
			{
				m_tCommonStat.iHp = 0;
			}
		}
		for (int i = 1; i < 3; i++)
		{
			m_iBlightDot[i - 1] = m_iBlightDot[i];
			m_iBleedDot[i - 1] = m_iBleedDot[i];
		}
		m_iBlightDot[3] = 0;
		m_iBleedDot[3] = 0;
		
		if (!m_iBlightDot[0]) m_bState[0] = false;
		if (!m_iBleedDot[0]) m_bState[1] = false;
	}

	// ����
	if (m_bState[2])
	{
		m_bState[2] = false;
		m_bMyTurn = false;
	}

	// ����
	if (m_bState3[0])
	{
		for (int i = 1; i < 3; i++)
		{
			m_iBuff1Dot[i - 1] = m_iBuff1Dot[i];
			m_iBuff2Dot[i - 1] = m_iBuff2Dot[i];
		}
		m_iBuff1Dot[3] = 0;
		m_iBuff2Dot[3] = 0;

		if (!m_iBuff1Dot[0] && !m_iBuff2Dot[0]) m_bState3[0] = false;
	}

	// �����
	if (m_bState3[1])
	{
		for (int i = 1; i < 3; i++)
		{
			m_iDeBuff1Dot[i - 1] = m_iDeBuff1Dot[i];
			m_iDeBuff1Dot[i - 1] = m_iDeBuff1Dot[i];
		}
		m_iDeBuff1Dot[3] = 0;

		if (!m_iDeBuff1Dot[0]) m_bState3[1] = false;
	}

	if (!m_bIsHero)
	{
		// ��ü ����	
		if (m_tCommonStat.iHp <= 0 && !m_bCorpse && !m_bDeath)
		{
			BleedCure();
			BlightCure();
			m_bCorpse = true;
			m_tCommonStat.iHp = 10;
			m_tCommonStat.iMaxHp = 10;
			// ���Ȱ���
			m_pStatInfo->SetMaxHp(m_tCommonStat.iMaxHp);
			m_pStatInfo->SetHp(m_tCommonStat.iHp);
			m_pStatInfo->SetCorpse(true);
		}

		// ��� ����
		if (m_tCommonStat.iHp <= 0 && m_bCorpse)
		{
			m_bCorpse = false;
			m_bDeath = true;
			m_tCommonStat.iHp = -100;
			
			bStatBarOn = false;
		}
	}
	else
	{
		// ������ �ϰ� ����
		if (m_tCommonStat.iHp <= 0 && m_bBeforeDeath)
		{
			int iNum = rand() % 100;
			// ���
			if (iNum < DEATHRATE)
			{
				m_bDeath = true;
				m_tCommonStat.iHp = -100;

				bStatBarOn = false;
			}
		}
		else if (m_tCommonStat.iHp <= 0 && !m_bBeforeDeath)
		{
			m_bBeforeDeath = true;
		}
	}
}

void CCreature::AttackCreature(shared_ptr<CCreature> _pCreature, shared_ptr<CCreature> _pCreature2, shared_ptr<CSkill> _pSkill)
{
	if (!_pCreature) return;
	if (!_pSkill) return;

	// ���� ����
	int iDodgeNum = rand() % 100;

	// ġ��Ÿ ����
	int iCritical = rand() % 100;

	_bool* arrAttack = _pSkill->GetArrAttack();
	
	
	// �ܼ� ����
	if (arrAttack[0])
	{
		if (iDodgeNum >= (_pCreature->GetDodge() + m_iBuff2Dot[0] + m_iDeBuff1Dot[0]))
		{
			if (iCritical < CRIRATE)
			{
				// ��ܿ��϶�
				if (!_pSkill->GetArrToEnemy()[2])
				{
					_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetCriticalRatio()
						* (m_iBuff1Dot[0]/100.f + 1.f)));
				}
				else
				{
					_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetCriticalRatio()));
				}

				if (dynamic_cast<CHero*>(this))
				{
					dynamic_cast<CHero*>(this)->DecreaseStress(5);
				}
			}
			else
			{
				// ��ܿ��϶�
				if (!_pSkill->GetArrToEnemy()[2])
				{
					_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetDamageRatio()
						* (m_iBuff1Dot[0] / 100.f + 1.f)));
				}
				else
				{
					_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetDamageRatio()));
				}
			}
		}
		// �ִϸ��̼�, ����Ʈ �ٲٴ� �ڵ� �־���ҵ�
	}

	// �ߵ�
	if (arrAttack[1])
	{
		if (iDodgeNum >= (_pCreature->GetDodge() + m_iBuff2Dot[0] + m_iDeBuff1Dot[0]))
		{
			if (iCritical < CRIRATE)
			{
				_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetCriticalRatio()));
				if (dynamic_cast<CHero*>(this))
				{
					dynamic_cast<CHero*>(this)->DecreaseStress(5);
				}
			}
			else
				_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetDamageRatio()));
			_pCreature->BlightAttack(_pSkill->GetDotDamage());
			_pCreature->SetBlight(true);
		}
	}

	// ����
	if (arrAttack[2])
	{
		if (iDodgeNum >= (_pCreature->GetDodge() + m_iBuff2Dot[0] + m_iDeBuff1Dot[0]))
		{
			if (iCritical < CRIRATE)
			{
				_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetCriticalRatio()));
				if (dynamic_cast<CHero*>(this))
				{
					dynamic_cast<CHero*>(this)->DecreaseStress(5);
				}
			}
			else
				_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetDamageRatio()));
			_pCreature->BleedAttack(_pSkill->GetDotDamage());
			_pCreature->SetBleed(true);
		}
	}

	// ����
	if (arrAttack[3])
	{
		if (iDodgeNum >= (_pCreature->GetDodge() + m_iBuff2Dot[0] + m_iDeBuff1Dot[0]))
		{
			if (iCritical < CRIRATE)
			{
				_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetCriticalRatio()));
				if (dynamic_cast<CHero*>(this))
				{
					dynamic_cast<CHero*>(this)->DecreaseStress(5);
				}
			}
			else
				_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetDamageRatio()));
			_pCreature->SetStun(true);
		}
	}

	// �̵�
	if (arrAttack[4])
	{
		if (iDodgeNum >= (_pCreature->GetDodge() + m_iBuff2Dot[0] + m_iDeBuff1Dot[0]))
		{
			if (iCritical < CRIRATE)
			{
				_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetCriticalRatio()));
				if (dynamic_cast<CHero*>(this))
				{
					dynamic_cast<CHero*>(this)->DecreaseStress(5);
				}
			}
			else
				_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetDamageRatio()));
		}
	}

	// ��
	if (arrAttack[5])
	{
		if (iCritical < CRIRATE)
		{
			_pCreature->IncreaseHP(_pSkill->GetHeal() * _pSkill->GetCriticalRatio());
			dynamic_pointer_cast<CHero>(_pCreature)->DecreaseStress(5);
		}
		else
			_pCreature->IncreaseHP(_pSkill->GetHeal()* _pSkill->GetDamageRatio());

		// ������ ���� ����
		if (_pCreature->GetIsBeforeDeath()) _pCreature->SetBeforeDeath(false);
	}

	// ����
	if (arrAttack[6])
	{
		// ��ܿ� ����
		if (!_pSkill->GetArrToEnemy()[0])
		{
			_int  DotBuff[2] = { 30, 3 };
			_pCreature2->Buff1Skill(DotBuff);
			_pCreature2->SetBuff(true);
		}
		// ��ܿ� �ع���
		if (!_pSkill->GetArrToEnemy()[1])
		{
			_int  DotBuff[2] = { 75, 3 };
			_pCreature2->Buff1Skill(DotBuff);
			_pCreature2->SetBuff(true);
		}
		// ��ܿ� ���� �ʱ�ȭ
		if (!_pSkill->GetArrToEnemy()[2])
		{
			_pCreature2->Buff1Reset();
			if (!m_iBuff1Dot[0] && !m_iBuff2Dot[0]) m_bState3[0] = false;
		}
		// ȸ�� ����
		if (!_pSkill->GetArrToEnemy()[3])
		{
			_int  DotBuff[2] = { 10, 3 };
			_pCreature->Buff2Skill(DotBuff);
			_pCreature->SetBuff(true);
		}
	}

	// �����
	if (arrAttack[7])
	{
		// ȸ�� �����
		if (iDodgeNum >= (_pCreature->GetDodge() + m_iBuff2Dot[0] + m_iDeBuff1Dot[0]))
		{
			if (!_pSkill->GetArrToEnemy()[4])
			{
				_int  DotDeBuff[2] = { -10, 3 };
				_pCreature->DeBuffSkill(DotDeBuff);
				_pCreature->SetDeBuff(true);
			}
		}
	}

	// ��Ʈ����
	if (dynamic_pointer_cast<CHero>(_pCreature))
	{
		if (iDodgeNum >= (_pCreature->GetDodge() + m_iBuff2Dot[0] + m_iDeBuff1Dot[0]))
		{
			dynamic_pointer_cast<CHero>(_pCreature)->IncreaseStress(_pSkill->GetStress());

		}
	}

	// ���϶�, ���������϶��� �ǰ� �ִϸ��̼� x
	if (arrAttack[5])
	{
		dynamic_pointer_cast<CCreature>(_pCreature)->SetHitted(false);
	}
	else if (m_bIsHero && !_pSkill->GetArrToEnemy()[3])
	{
		dynamic_pointer_cast<CCreature>(_pCreature)->SetHitted(false);
	}
	else
	{
		dynamic_pointer_cast<CCreature>(_pCreature)->SetHitted(true);
	}

	// �¾�����
	if (iDodgeNum >= (_pCreature->GetDodge() + m_iBuff2Dot[0] + m_iDeBuff1Dot[0]))
	{
		// ���
		dynamic_pointer_cast<CCreature>(_pCreature)->SetEffectOn(true);

		// ���� �ְ� �����̸�
		if (dynamic_pointer_cast<CCreature>(_pCreature)->m_bIsHero)
		{
			// ������ �ϰ� ����
			if (dynamic_pointer_cast<CCreature>(_pCreature)->GetHp() <= 0
				&& dynamic_pointer_cast<CCreature>(_pCreature)->GetIsBeforeDeath())
			{
				int iNum = rand() % 100;
				// ���
				if (iNum < DEATHRATE)
				{
					dynamic_pointer_cast<CCreature>(_pCreature)->SetDeath(true);
					dynamic_pointer_cast<CCreature>(_pCreature)->SetHp(-100);
					dynamic_pointer_cast<CCreature>(_pCreature)->SetStartBarOn(false);
				}
			}
			else if (dynamic_pointer_cast<CCreature>(_pCreature)->GetHp() <= 0
				&& !dynamic_pointer_cast<CCreature>(_pCreature)->GetIsBeforeDeath())
			{
				dynamic_pointer_cast<CCreature>(_pCreature)->SetBeforeDeath(true);
			}
		}
		
	}
	// ȸ��������
	else
	{

	}

	// ��
	m_bEffectOn = true;

	// Ÿ�� ����Ʈ
	{
		// �ִϸ��̼�, ����Ʈ �ٲٴ� �ڵ� �־���ҵ�
		if (_pSkill->GetTargetEffectAnimKey() != L"")
		{
			_pCreature->SetEffectInfo(_pSkill, true);
		}
	}

}

void CCreature::MovePos(_vec3 _vPos, const _float& fTimeDelta, _float _fSpeed)
{
	_vec3		vDir;
	vDir = _vPos - m_vPos;

	D3DXVec3Normalize(&vDir, &vDir);
	m_pTransformCom->MoveForward(&vDir, fTimeDelta, _fSpeed);
}

_float CCreature::MovingSpeed(_vec3 _vPos, _float _fMovingTime)
{
	_vec3		vDir;
	vDir = _vPos - m_vPos;

	D3DXVec3Normalize(&vDir, &vDir);
	_float m_fSpeed = 0.f;
	if (vDir.x != 0)
		m_fSpeed = (_vPos - m_vPos).x / vDir.x / _fMovingTime;
	else if (vDir.y != 0)
		m_fSpeed = (_vPos - m_vPos).y / vDir.y / _fMovingTime;
	else if (vDir.z != 0)
		m_fSpeed = (_vPos - m_vPos).z / vDir.z / _fMovingTime;

	return m_fSpeed;
}

void CCreature::OffTurnCursor()
{
	m_pStatInfo->SetIsTurn(false);
}

void CCreature::OnTurnCursor()
{
	m_pStatInfo->SetIsTurn(true);
}

void CCreature::OnTargetTeamCursor()
{
	m_pStatInfo->SetIsTeamTarget(true);
}

void CCreature::OffTargetTeamCursor()
{
	m_pStatInfo->SetIsTeamTarget(false);
}

void CCreature::OnTargetTeamsCursor()
{
	m_pStatInfo->SetIsTeamsTarget(true);
}

void CCreature::OffTargetTeamsCursor()
{
	m_pStatInfo->SetIsTeamsTarget(false);
}

void CCreature::OnTargetEnemyCursor()
{
	m_pStatInfo->SetIsEnemyTarget(true);
}

void CCreature::OffTargetEnemyCursor()
{
	m_pStatInfo->SetIsEnemyTarget(false);
}

void CCreature::OnTargetEnemiesCursor()
{
	m_pStatInfo->SetIsEnemiesTarget(true);
}

void CCreature::OffTargetEnemiesCursor()
{
	m_pStatInfo->SetIsEnemiesTarget(false);
}

void CCreature::OnTurnUi()
{
	m_pStatInfo->SetIsTurnOff(false);
}

void CCreature::OffTurnUi()
{
	m_pStatInfo->SetIsTurnOff(true);
}

void CCreature::OnVirtue()
{
	m_pStatInfo->SetVirtue(true);
}

void CCreature::OffVirtue()
{
	m_pStatInfo->SetVirtue(false);
}

void CCreature::OnAffliction()
{
	m_pStatInfo->SetAffliction(true);
}

void CCreature::OffAffliction()
{
	m_pStatInfo->SetAffliction(false);
}

void CCreature::EndAttack(shared_ptr<CGameObject> _pCreature)
{
	if (!dynamic_pointer_cast<CCreature>(_pCreature)) return;

	// ���
	dynamic_pointer_cast<CCreature>(_pCreature)->SetHitted(false);
	dynamic_pointer_cast<CCreature>(_pCreature)->SetEffectOn(false);

	// ��
	m_bEffectOn = false;

	// ������
	m_bMyTurn = false;
}

void CCreature::Buff1Skill(_int* _iDotBuff)
{
	for (int i = 0; i < _iDotBuff[1]; i++)
	{
		m_iBuff1Dot[i] += _iDotBuff[0];
	}
}

void CCreature::Buff2Skill(_int* _iDotBuff)
{
	for (int i = 0; i < _iDotBuff[1]; i++)
	{
		m_iBuff2Dot[i] += _iDotBuff[0];
	}
}

void CCreature::DeBuffSkill(_int* _iDotDeBuff)
{
	for (int i = 0; i < _iDotDeBuff[1]; i++)
	{
		m_iDeBuff1Dot[i] += _iDotDeBuff[0];
	}
}

void CCreature::Buff1Reset()
{
	m_bState3[0] = false;
	for (int i = 0; i < 4; i++) m_iBuff1Dot[i] = 0;
}

void CCreature::BlightAttack(_int* _iDotDam)
{
	for (int i = 0; i < _iDotDam[1]; i++)
	{
		m_iBlightDot[i] += _iDotDam[0];
	}
}

void CCreature::BleedAttack(_int* _iDotDam)
{
	for (int i = 0; i < _iDotDam[1]; i++)
	{
		m_iBleedDot[i] += _iDotDam[0];
	}
}

void CCreature::BlightCure()
{
	m_bState[0] = false;
	for (int i = 0; i < 4; i++) m_iBlightDot[i] = 0;
}

void CCreature::BleedCure()
{
	m_bState[1] = false;
	for (int i = 0; i < 4; i++) m_iBleedDot[i] = 0;
}



void CCreature::SetEffectInfo(shared_ptr<CSkill> _pSkill, _bool _bTarget)
{
	if (_bTarget)
	{
		if (_pSkill->GetTargetEffectAnimKey() != L"")
		{
			m_pEffect = CEffectMgr::GetInstance()->GetEffect();

			shared_ptr< tagTextureInfo> textureInfo = *CResourceMgr::GetInstance()->GetTexture(_pSkill->GetAnimKey(), TEX_NORMAL)->begin();

			m_pEffect->SetInfo(_pSkill->GetTargetEffectAnimKey(), textureInfo->vImgSize, m_pTransformCom->GetPos(), m_pTransformCom->GetScale(), ATTACKTIME, false);
			m_pEffect->SetActive(true);
		}
	}

	else
	{
		if (_pSkill->GetEffectAnimKey() != L"")
		{
			m_pEffect = CEffectMgr::GetInstance()->GetEffect();

			shared_ptr< tagTextureInfo> textureInfo = *CResourceMgr::GetInstance()->GetTexture(_pSkill->GetAnimKey(), TEX_NORMAL)->begin();

			m_pEffect->SetInfo(_pSkill->GetEffectAnimKey(), textureInfo->vImgSize, m_pTransformCom->GetPos(), m_pTransformCom->GetScale(), ATTACKTIME, false);
			m_pEffect->SetActive(true);
		}
	}
}

void CCreature::AddComponent()
{
	shared_ptr<CComponent> pComponent;

	pComponent = m_pBufCom = make_shared<CRcTex>(m_pGraphicDev);
	m_pBufCom->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_RCTex",pComponent });

	pComponent = m_pEffectBufCom = make_shared<CRcTex>(m_pGraphicDev);
	m_pEffectBufCom->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_EffectRCTex",pComponent });
}

void CCreature::ClimbingTerrain()
{
	//_vec3		vPos;
	//m_pTransformCom->GetInfo(INFO_POS, &vPos);
	//
	//CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"OBJ_Terrain", L"Proto_TerrainTex", ID_STATIC));
	//NULL_CHECK(pTerrainBufferCom);
	//
	//_float		fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos,
	//	pTerrainBufferCom->Get_VtxPos(),
	//	pTerrainBufferCom->Get_VtxCntX(),
	//	pTerrainBufferCom->Get_VtxCntZ());
	//
	//m_pTransformCom->Set_Pos(vPos.x, fHeight + 0.5f, vPos.z);
}

void CCreature::SettingStatBar()
{
	m_pStatInfo = make_shared<CStatView>(m_pGraphicDev);
	m_pStatInfo->AwakeGameObject();
	m_pStatInfo->ReadyGameObject();

	//m_pStatInfo->SettingInit(*(m_pTransformCom->GetPos()),100, 100, true);

}

void CCreature::Free()
{
}
