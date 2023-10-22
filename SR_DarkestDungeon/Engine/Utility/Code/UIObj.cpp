#include "UIObj.h"

CUIObj::CUIObj(LPDIRECT3DDEVICE9 _pGraphicDev)
	:CGameObject(_pGraphicDev)
{
}

CUIObj::~CUIObj()
{
}

HRESULT CUIObj::ReadyGameObject()
{
	return S_OK;
}

_int CUIObj::UpdateGameObject(const _float& fTimeDelta)
{
	int iExit = 0;

	return iExit;
}

void CUIObj::LateUpdateGameObject()
{
}

void CUIObj::RenderGameObject()
{
}

void CUIObj::AddComponent()
{
	shared_ptr<CComponent> pComponent;

	pComponent = m_pRcTexCom = make_shared<CRcTex>(m_pGraphicDev);
	m_pRcTexCom->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_RcTex",pComponent });

	pComponent = m_pTransCom = make_shared<CTransform>();
	NULL_CHECK_MSG(pComponent, L"Make Player Item TransformCom Failed");
	m_pTransCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });
}
