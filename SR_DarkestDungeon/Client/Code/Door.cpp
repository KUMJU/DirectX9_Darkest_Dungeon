#include "pch.h"
#include "Door.h"
#include "Export_System.h"
#include "Export_Utility.h"

#include "Player.h"
#include "Hero.h"

CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphicDev, EFacilityType _eDoorType)
	: CInteractionObj(pGraphicDev), m_eDoorType(_eDoorType)
{
}

CDoor::CDoor(const CDoor& rhs)
	: CInteractionObj(rhs)
{
}

CDoor::~CDoor()
{
}

HRESULT CDoor::ReadyGameObject()
{
	__super::ReadyGameObject();

	return S_OK;
}

_int CDoor::UpdateGameObject(const _float& fTimeDelta)
{
	_int	iExit = __super::UpdateGameObject(fTimeDelta);

	return iExit;
}

void CDoor::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CDoor::RenderGameObject()
{
	__super::RenderGameObject();
}

void CDoor::AddComponent()
{

	shared_ptr<CComponent> pComponent;

	pComponent = m_pBufferCom = make_shared<CRcTex>(m_pGraphicDev);
	dynamic_pointer_cast<CRcTex>(m_pBufferCom)->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_RcTex", pComponent });

	pComponent = m_pTransformCom = make_shared<CTransform>(_vec3(0.f, 0.f, 0.f), _vec3(1.f, 1.f, 1.f), _vec3(0.f, 0.f, 0.f));
	m_pTransformCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	m_pTransformCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
	m_pTransformCom->SetScale(m_vScale.x, m_vScale.y, m_vScale.z);

	pComponent = m_pTextureCom = make_shared<CTexture>(m_pGraphicDev);
	m_pTextureCom->SetTextureKey(L"Village_Door_Close", TEX_NORMAL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture",pComponent });
}

void CDoor::GetInteractionKey(const _float& fTimeDelta)
{
	// ���ͷ��� Ű UI �����ֱ�
	// ----------���� �ʿ�------------

	// Ű �Է¹ޱ�
	if (GetAsyncKeyState('C') & 0x8000)
	{
		//m_bInteracting = true;

		// �ؽ�ó �Ǵ� �ִϸ��̼� ����
		//ChangeTexture();

		if (EFacilityType::TERVARN == m_eDoorType)
		{

			// ��¥ �׽�Ʈ
			{
				//shared_ptr<CPlayer> pPlayer = dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer());
				//
				//vector<shared_ptr<CGameObject>>* pHeroVec = pPlayer->GetHeroVec();

				//shared_ptr<CHero> pHero = dynamic_pointer_cast<CHero>((*pHeroVec)[0]);

				//pHero->SetStress(50);
				//pHero->SetHp(10);
			}

			// ���� ���η� �̵�
			dynamic_pointer_cast<CTransform>(CGameMgr::GetInstance()->GetPlayer()->GetComponent(L"Com_Transform", ID_DYNAMIC))->SetPosition(50.f, -97.f, 4.f);

		}
			
		else if (EFacilityType::GUILD == m_eDoorType)
		{
			// �Ʒü� ���η� �̵�
			dynamic_pointer_cast<CTransform>(CGameMgr::GetInstance()->GetPlayer()->GetComponent(L"Com_Transform", ID_DYNAMIC))->SetPosition(0.f, -197.f, 104.f);

		}

		Interaction();
		}
}

void CDoor::Interaction()
{
}

void CDoor::FinishInteraction()
{
}

void CDoor::ChangeTexture()
{
	m_pTextureCom->SetTextureKey(L"Village_Door_Open", TEX_NORMAL);
}

_bool CDoor::IsFinish()
{
	// Ű �Է¹ޱ�
	//if (GetAsyncKeyState('X') & 0x8000)
	//{
	//	m_bInteracting = false;
	//	m_pTextureCom->SetTextureKey(L"Village_Door_Close", TEX_NORMAL);

	//	// �÷��̾� �ൿ Ǯ��
	//	CGameMgr::GetInstance()->SetGameState(EGameState::PRCESS);

	//	return true;
	//}

	return false;
}