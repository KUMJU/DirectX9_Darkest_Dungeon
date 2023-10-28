#include "pch.h"
#include "Creature.h"
#include "Hero.h"
#include "Export_System.h"
#include "Export_Utility.h"

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

	return E_NOTIMPL;
}

_int CCreature::UpdateGameObject(const _float& fTimeDelta)
{
	_int	iExit = CGameObject::UpdateGameObject(fTimeDelta);

	m_vPos = *m_pTransformCom->GetPos();

	//빌보드 시작
	_matrix matWorld;

	matWorld = *m_pTransformCom->GetWorld();
	SetBillBoard(matWorld);
	m_pTransformCom->SetWorld(&matWorld);

	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());

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
//		// 수정 필요 // 이렇게 비교해도 되는지 확인해봐야됨
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
	// 출혈이나 중독 상태라면
	if (m_bState[0] || m_bState[1])
	{
		// 도트뎀 계산
		m_tCommonStat.iHp -= (m_bBlightDot[0] + m_bBleedDot[0]);
		for (int i = 1; i < 3; i++)
		{
			m_bBlightDot[i - 1] = m_bBlightDot[i];
			m_bBleedDot[i - 1] = m_bBleedDot[i];
		}
		m_bBlightDot[3] = 0;
		m_bBleedDot[3] = 0;
	}

	// 기절
	if (m_bState[2])
	{
		m_bState[2] = false;
		m_bMyTurn = false;
	}

	if (!m_bIsHero)
	{
		// 시체 여부	
		if (m_tCommonStat.iHp <= 0 && !m_bCorpse && !m_bDeath)
		{
			BleedCure();
			BlightCure();
			m_bCorpse = true;
			m_tCommonStat.iHp = 10;
		}

		// 사망 여부
		if (m_tCommonStat.iHp <= 0 && m_bCorpse)
		{
			m_bCorpse = false;
			m_bDeath = true;
			m_tCommonStat.iHp = -100;
		}
	}
	else
	{
		if (m_tCommonStat.iHp <= 0)
		{
			m_bDeath = true;
			m_tCommonStat.iHp = -100;
		}
	}
}

void CCreature::AttackCreature(shared_ptr<CCreature> _pCreature, shared_ptr<CSkill> _pSkill)
{
	if (!_pCreature) return;
	if (!_pSkill) return;

	// 닷지 계산용
	int iDodgeNum = rand() % 100;

	_bool* arrAttack = _pSkill->GetArrAttack();


	// 단순 공격
	if (arrAttack[0])
	{
		if (iDodgeNum >= _pCreature->GetDodge())
		{
			_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetDamageRatio()));
		}
		// 애니메이션, 이펙트 바꾸는 코드 넣어야할듯
	}

	// 중독
	if (arrAttack[1])
	{
		if (iDodgeNum >= _pCreature->GetDodge())
		{
			_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetDamageRatio()));
			_pCreature->BlightAttack(_pSkill->GetDotDamage());
			_pCreature->SetBlight(true);
		}

		// 애니메이션, 이펙트 바꾸는 코드 넣어야할듯
	}

	// 출혈
	if (arrAttack[2])
	{
		if (iDodgeNum >= _pCreature->GetDodge())
		{
			_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetDamageRatio()));
			_pCreature->BleedAttack(_pSkill->GetDotDamage());
			_pCreature->SetBleed(true);
		}

		// 애니메이션, 이펙트 바꾸는 코드 넣어야할듯
	}

	// 기절
	if (arrAttack[3])
	{
		if (iDodgeNum >= _pCreature->GetDodge())
		{
			_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetDamageRatio()));
			_pCreature->SetStun(true);
		}

		// 애니메이션, 이펙트 바꾸는 코드 넣어야할듯
	}

	// 이동
	if (arrAttack[4])
	{
		if (iDodgeNum >= _pCreature->GetDodge())
		{
			_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetDamageRatio()));
			//_pCreature->SetPosition(_pSkill->GetMoveCnt());
		}
	}

	// 힐
	if (arrAttack[5])
	{
		_pCreature->IncreaseHP(_pSkill->GetHeal());
	}

	// 스트레스 처리
	if (dynamic_pointer_cast<CHero>(_pCreature))
	{
		if (iDodgeNum >= _pCreature->GetDodge())
		{
			dynamic_pointer_cast<CHero>(_pCreature)->IncreaseStress(_pSkill->GetStress());
		}
	}

	if (iDodgeNum >= _pCreature->GetDodge())
	{
		// 상대
		dynamic_pointer_cast<CCreature>(_pCreature)->SetHitted(true);
		dynamic_pointer_cast<CCreature>(_pCreature)->SetEffectOn(true);
	}

	// 나
	m_bEffectOn = true;
}

void CCreature::MovePos(_vec3 _vPos, const _float& fTimeDelta, _float _fSpeed)
{
	_vec3		vDir;
	vDir = _vPos - m_vPos;

	D3DXVec3Normalize(&vDir, &vDir);
	m_pTransformCom->MoveForward(&vDir, fTimeDelta, _fSpeed);
	int a = 3;
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

void CCreature::EndAttack(shared_ptr<CGameObject> _pCreature)
{
	if (!dynamic_pointer_cast<CCreature>(_pCreature)) return;

	// 상대
	dynamic_pointer_cast<CCreature>(_pCreature)->SetHitted(false);
	dynamic_pointer_cast<CCreature>(_pCreature)->SetEffectOn(false);

	// 나
	m_bEffectOn = false;

	// 턴종료
	m_bMyTurn = false;
}

void CCreature::BlightAttack(_int* _iDotDam)
{
	for (int i = 0; i < _iDotDam[1]; i++)
	{
		m_bBlightDot[i] += _iDotDam[0];
	}
}

void CCreature::BleedAttack(_int* _iDotDam)
{
	for (int i = 0; i < _iDotDam[1]; i++)
	{
		m_bBleedDot[i] += _iDotDam[0];
	}
}

void CCreature::BlightCure()
{
	m_bState[0] = false;
	for (int i = 0; i < 4; i++) m_bBlightDot[i] = 0;
}

void CCreature::BleedCure()
{
	m_bState[1] = false;
	for (int i = 0; i < 4; i++) m_bBleedDot[i] = 0;
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

void CCreature::Free()
{
}
