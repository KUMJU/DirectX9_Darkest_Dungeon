#include "pch.h"
#include "Creature.h"
#include "Export_System.h"
#include "Export_Utility.h"

CCreature::CCreature(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	memset(&m_tCommonStat, NULL, sizeof(STAT));
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

	//������ ����
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
		m_tCommonStat.iHp -= (m_bBlightDot[0] + m_bBleedDot[0]);
		for (int i = 1; i < 3; i++)
		{
			m_bBlightDot[i - 1] = m_bBlightDot[i];
			m_bBleedDot[i - 1] = m_bBleedDot[i];
		}
		m_bBlightDot[3] = 0;
		m_bBleedDot[3] = 0;
	}

	// ����
	if (m_bState[2])
	{
		m_bState[2] = false;
		m_bMyTurn = false;
		m_bDone = true;
	}

	// �׾�����
	if (m_tCommonStat.iHp <= 0)
	{
		m_bState[3] = true;
		m_bMyTurn = false;
		m_bDone = true;
	}
}

void CCreature::AttackCreature(shared_ptr<CCreature> _pCreature, shared_ptr<CSkill> _pSkill)
{
	if (!_pCreature) return;
	if (!_pSkill) return;

	_bool* arrAttack = _pSkill->GetArrAttack();

	// �ܼ� ����
	if (arrAttack[0])
	{
		_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetDamageRatio()));

		// �ִϸ��̼�, ����Ʈ �ٲٴ� �ڵ� �־���ҵ�
	}

	// �ߵ�
	if (arrAttack[1])
	{
		_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetDamageRatio()));
		_pCreature->BlightAttack(_pSkill->GetDotDamage());
		_pCreature->SetBlight(true);

		// �ִϸ��̼�, ����Ʈ �ٲٴ� �ڵ� �־���ҵ�
	}

	// ����
	if (arrAttack[2])
	{
		_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetDamageRatio()));
		_pCreature->BleedAttack(_pSkill->GetDotDamage());
		_pCreature->SetBleed(true);

		// �ִϸ��̼�, ����Ʈ �ٲٴ� �ڵ� �־���ҵ�
	}

	// ����
	if (arrAttack[3])
	{
		_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetDamageRatio()));
		_pCreature->SetStun(true);

		// �ִϸ��̼�, ����Ʈ �ٲٴ� �ڵ� �־���ҵ�
	}

	// �̵�
	if (arrAttack[4])
	{
		_pCreature->DecreaseHP((_int)((_float)m_tCommonStat.iAttackPower * _pSkill->GetDamageRatio()));
		_pCreature->SetPosition(_pSkill->GetMoveCnt());

		// ���� ũ��ó�鵵 �������ֱ�
		// ���� �ʿ�
		// �̰� ������ ���� ��,,,,
	}

	// ��
	if (arrAttack[5])
	{
		_pCreature->IncreaseHP(_pSkill->GetHeal());

		// �ִϸ��̼�, ����Ʈ �ٲٴ� �ڵ� �־���ҵ�
	}

	// ���
	dynamic_pointer_cast<CCreature>(_pCreature)->SetHitted(true);
	dynamic_pointer_cast<CCreature>(_pCreature)->SetEffectOn(true);

	// ��
	m_bEffectOn = true;
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
