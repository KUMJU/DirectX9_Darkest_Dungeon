#include "pch.h"
#include "Outside.h"

#include "Export_Utility.h"
#include "Export_System.h"

#include "Door.h"

COutside::COutside(LPDIRECT3DDEVICE9 pGraphicDev, EFacilityType _eFacilityType)
	: Engine::CGameObject(pGraphicDev), m_eFacilityType(_eFacilityType)
{
}

COutside::COutside(const COutside& rhs)
    : Engine::CGameObject(rhs)
{
}

COutside::~COutside()
{
}

HRESULT COutside::ReadyGameObject()
{	
	if (m_eFacilityType == EFacilityType::TERVARN)
	{
		shared_ptr<CGameObject> m_pDoor = make_shared<CDoor>(m_pGraphicDev, EFacilityType::TERVARN);
		m_pDoor->SetPos({ m_vPos.x + 4.5f, m_vPos.y + 4.f, m_vPos.z - 0.1f });
		m_pDoor->SetScale({ 1.7f, 4.f, 1.f });
		m_vecGameObject.push_back(m_pDoor);
	}

	else if (m_eFacilityType == EFacilityType::GUILD)
	{
		shared_ptr<CGameObject> m_pDoor = make_shared<CDoor>(m_pGraphicDev, EFacilityType::GUILD);
		m_pDoor->SetPos({ m_vPos.x - 5.5f, m_vPos.y + 5.7f, m_vPos.z - 0.1f });
		m_pDoor->SetScale({ 3.f, 5.5f, 1.f });
		m_vecGameObject.push_back(m_pDoor);
	}

	for (auto& iter : m_vecGameObject) {
		iter->AwakeGameObject();
		iter->ReadyGameObject();
	}

    return S_OK;
}

_int COutside::UpdateGameObject(const _float& fTimeDelta)
{
    Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());

	for (auto& iter : m_vecGameObject) {
		iter->UpdateGameObject(fTimeDelta);
	}

	_int iExit = __super::UpdateGameObject(fTimeDelta);

	return iExit;
}

void COutside::LateUpdateGameObject()
{
	for (auto& iter : m_vecGameObject) {
		iter->LateUpdateGameObject();
	}
}

void COutside::RenderGameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorld());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->SetTexture(0);

	m_pBufferCom->RenderBuffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	for (auto& iter : m_vecGameObject) {
		iter->RenderGameObject();
	}
}

void COutside::AddComponent()
{
	shared_ptr<CComponent> pComponent;

	pComponent = m_pBufferCom = make_shared<CRcTex>(m_pGraphicDev);
	dynamic_pointer_cast<CRcTex>(m_pBufferCom)->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_RcTex", pComponent });

	pComponent = m_pTextureCom = make_shared<CTexture>(m_pGraphicDev);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = make_shared<CTransform>(_vec3(0.f, 0.f, 0.f), _vec3(1.f, 1.f, 1.f), _vec3(0.f, 0.f, 0.f));
	m_pTransformCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	m_pTransformCom->SetPosition(m_vPos.x, m_vPos.y + m_vScale.y / 2 + m_vScale.y / 3, m_vPos.z);
	m_pTransformCom->SetScale(m_vScale.x, m_vScale.y, m_vScale.z);

	pComponent = m_pTextureCom = make_shared<CTexture>(m_pGraphicDev);
	switch (m_eFacilityType)
	{
	case EFacilityType::TERVARN:
		m_strKey = L"Tervarn_Outside";
		break;
	case EFacilityType::GUILD:
		m_strKey = L"Guild_Outside";
		break;
	case EFacilityType::STAGECOACH:
		m_strKey = L"StageCoach_Outside";
		break;
	case EFacilityType::STORE:
		m_strKey = L"Store_Outside";
		break;
	case EFacilityType::ENUM_END:
		break;
	}
	m_pTextureCom->SetTextureKey(m_strKey, TEX_NORMAL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture",pComponent });
}
