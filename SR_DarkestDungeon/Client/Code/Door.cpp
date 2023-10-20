#include "pch.h"
#include "Door.h"
#include "Export_System.h"
#include "Export_Utility.h"

CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteractionObj(pGraphicDev)
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

	m_pPlayerTransformCom = dynamic_pointer_cast<CTransform>(m_pPlayer->GetComponent(L"Com_Transform", ID_DYNAMIC));

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
	NULL_CHECK_MSG(pComponent, L"RcTex AddComponent Failed");
	dynamic_pointer_cast<CRcTex>(m_pBufferCom)->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_RcTex", pComponent });

	pComponent = m_pTextureCom = make_shared<CTexture>(m_pGraphicDev);
	NULL_CHECK_MSG(pComponent, L"Texture AddComponent Failed");
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = make_shared<CTransform>(_vec3(0.f, 0.f, 0.f), _vec3(1.f, 1.f, 1.f), _vec3(0.f, 0.f, 0.f));
	NULL_CHECK_MSG(pComponent, L"Transform AddComponent Failed");
	m_pTransformCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	m_pTransformCom->SetPosition(10.f, 1.f, 10.f);
	m_pTransformCom->SetScale(2.f, 2.f, 2.f);

	pComponent = m_pTextureCom = make_shared<CTexture>(m_pGraphicDev);
	m_pTextureCom->SetTextureKey(L"Village_Door_Close", TEX_NORMAL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture",pComponent });
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
	// 키 입력받기
	if (GetAsyncKeyState('X') & 0x8000)
	{
		m_bInteracting = false;
		m_pTextureCom->SetTextureKey(L"Village_Door_Close", TEX_NORMAL);

		// 플레이어 행동 풀기
		CGameMgr::GetInstance()->SetGameState(EGameState::PRCESS);

		return true;
	}

	return false;
}