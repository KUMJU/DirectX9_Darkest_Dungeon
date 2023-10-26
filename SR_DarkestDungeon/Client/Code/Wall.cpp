#include "pch.h"
#include "Wall.h"
#include"Export_Utility.h"

CWall::CWall(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CWall::CWall(LPDIRECT3DDEVICE9 pGraphicDev, const tstring& _KeyName)
	: Engine::CGameObject(pGraphicDev), m_strKeyName(_KeyName)
{
}

CWall::CWall(const CWall& rhs)
	: Engine::CGameObject(rhs)
{
}

CWall::~CWall()
{
}

HRESULT CWall::ReadyGameObject()
{
	m_pTransformCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
	m_pTransformCom->SetScale(6.f, 6.f, 6.f);
	
	m_pTransformCom->SetAngle(m_vAngle);
	if (PI / 2.f == m_vAngle.y)
	{
		m_bHorizontal = true;
		m_pColliderCom->SetScale({ 12.f, 12.f, 1.f });
	}
	else
		m_bHorizontal = false;
	m_pTransformCom->Rotation(ROT_Y, PI /2.f);
	m_iNum = rand() % 9;

	m_eCollideID = ECollideID::WALL;
	m_bColliding = true;
	return E_NOTIMPL;
}

_int CWall::UpdateGameObject(const _float& fTimeDelta)
{
	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());
	_int	iExit = CGameObject::UpdateGameObject(fTimeDelta);

	return iExit;
}

void CWall::LateUpdateGameObject()
{
}

void CWall::RenderGameObject()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorld());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	// m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	if (m_strKeyName != L"")
	{
		m_pTextureCom->SetTextureKey(m_strKeyName, TEX_NORMAL);
		m_pTextureCom->SetTexture(m_iNum / 3);
	}

	else
	{
		m_pTextureCom2->SetTextureKey(L"Com_Weald_BackWallTexture", TEX_NORMAL);
		m_pTextureCom2->SetTexture(0);
		m_pBufferCom2->RenderBuffer();

		m_pTextureCom->SetTextureKey(L"Com_Weald_WallTexture", TEX_NORMAL);
		m_pTextureCom->SetTexture(m_iNum);
	}

	m_pBufferCom->RenderBuffer();
	m_pColliderCom->RenderCollider();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CWall::OnCollide(shared_ptr<CGameObject> _pObj)
{
}

void CWall::AddComponent()
{
	shared_ptr<CComponent> pComponent;

	pComponent = m_pBufferCom = (make_shared<CRcTex>(m_pGraphicDev));
	NULL_CHECK_MSG(pComponent, L"Wall AddComponent Failed");
	dynamic_pointer_cast<CRcTex>(m_pBufferCom)->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_Weald_WallTexture", pComponent });

	pComponent = m_pBufferCom2 = (make_shared<CRcTex>(m_pGraphicDev));
	NULL_CHECK_MSG(pComponent, L"Wall AddComponent Failed");
	dynamic_pointer_cast<CRcTex>(m_pBufferCom2)->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_Weald_BackWallTexture", pComponent });

	pComponent = m_pTextureCom = make_shared<CTexture>(m_pGraphicDev);
	NULL_CHECK_MSG(pComponent, L"Texture AddComponent Failed");
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTextureCom2 = make_shared<CTexture>(m_pGraphicDev);
	NULL_CHECK_MSG(pComponent, L"Texture AddComponent Failed");
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = make_shared<CTransform>(_vec3(0.f, 0.f, 0.f), _vec3(1.f, 1.f, 1.f), _vec3(0.f, 0.f, 0.f));
	NULL_CHECK_MSG(pComponent, L"Transform AddComponent Failed");
	//m_pTransformCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pColliderCom = make_shared<CCollider>(m_pGraphicDev);
	NULL_CHECK_MSG(pComponent, L"Make Player Item ColliderCom Failed");
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider",pComponent });
	m_pColliderCom->SetScale({ 1.f, 12.f, 12.f });
	m_pColliderCom->SetPos(m_pTransformCom->GetPos());
}
