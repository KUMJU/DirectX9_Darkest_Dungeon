#include"pch.h"
#include "PlayerProj.h"
#include"Export_Utility.h"

#include"EffectMgr.h"
#include"Effect.h"

CPlayerProj::CPlayerProj(LPDIRECT3DDEVICE9 _pGraphicDev, tstring _strKeyName, _vec3 _vInitPos, _matrix _matPlrWorld)
	:CGameObject(_pGraphicDev), m_strAnimKeyName(_strKeyName)
{
	memcpy(&m_vStartPos, &_vInitPos, sizeof(_vInitPos));
	memcpy(&m_matWorld, &_matPlrWorld, sizeof(_matPlrWorld));
	ZeroMemory(&m_tProjInfo, sizeof(m_tProjInfo));
}

CPlayerProj::~CPlayerProj()
{
}

HRESULT CPlayerProj::ReadyGameObject()
{
	//초기 위치 setting
	m_pTransmCom->SetWorld(&m_matWorld);
	memcpy(&m_vLook, &m_matWorld.m[2][0], sizeof(_vec3));
	m_pTransmCom->SetPosition(m_vStartPos.x, m_vStartPos.y + 1.f, m_vStartPos.z);
	
	m_tProjInfo.iSpeed = 20.f;
	m_pColliderCom->SetPos(m_pTransmCom->GetPos());

	m_strEffectAnimKey = L"SpellHand_Proj_Effect";

	m_bColliding = true;
	m_eCollideID = ECollideID::PLAYER_PROJECTILE;
 
	return S_OK;
}

_int CPlayerProj::UpdateGameObject(const _float& fTimeDelta)
{
	if (m_pEffect) {
		m_pEffect->UpdateGameObject(fTimeDelta);
	}


	if (!m_bTestBool)
		return 0;

	_int iExit(0);
	iExit = __super::UpdateGameObject(fTimeDelta);
	

	//MoveForward
	_vec3 vLook, vPos;
	m_pTransmCom->GetInfo(INFO::INFO_LOOK, &vLook);
	m_pTransmCom->GetInfo(INFO::INFO_POS, &vPos);

	vPos += fTimeDelta * m_tProjInfo.iSpeed * m_vLook;
	
	m_pTransmCom->SetPosition(vPos.x, vPos.y, vPos.z);

	_matrix matWorld;

	//빌보드
	matWorld = *m_pTransmCom->GetWorld();
	SetBillBoard(matWorld);
	m_pTransmCom->SetWorld(&matWorld);

	AddRenderGroup(RENDERID::RENDER_ALPHA, shared_from_this());

	return iExit;
}

void CPlayerProj::LateUpdateGameObject()
{

}

void CPlayerProj::RenderGameObject()
{
	if (m_pEffect)
		m_pEffect->RenderGameObject();

	if (!m_bTestBool)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransmCom->GetWorld());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	m_pTextureCom->SetTexture();
	m_pRcTexCom->RenderBuffer();


	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


}

void CPlayerProj::AddComponent()
{
	shared_ptr<CComponent> pComponent;

	pComponent = m_pRcTexCom = make_shared<CRcTex>(m_pGraphicDev);
	m_pRcTexCom->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_RcTex",pComponent });

	pComponent = m_pTextureCom = make_shared<CTexture>(m_pGraphicDev);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });
	m_pTextureCom->SetTextureKey(m_strAnimKeyName, TEXTUREID::TEX_NORMAL);

	pComponent = m_pTransmCom = make_shared<CTransform>();
	m_pTransmCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });
	m_pTransmCom->SetAngle({ 0.f, 0.f, 0.f });
	m_pTransmCom->SetScale(0.5f, 0.5f, 1.f);

	pComponent = m_pColliderCom = make_shared<CCollider>(m_pGraphicDev);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider",pComponent });
	m_pColliderCom->SetScale({ 1.f, 1.f, 1.f });

}

void CPlayerProj::OnCollide(shared_ptr<CGameObject> _pObj)
{
	if (!m_bTestBool)
		return;

	if (ECollideID::WALL == _pObj->GetColType() || ECollideID::ENVIRONMENT == _pObj->GetColType())
	{
		//Effect Setting
		
		m_pEffect = CEffectMgr::GetInstance()->GetEffect();
		
		if (m_pEffect) {

			_vec3 testPos = *m_pTransmCom->GetPos();
			m_pEffect->SetProjEffect(m_strEffectAnimKey, testPos, m_pTransmCom->GetScale(), 10.f);
			m_pEffect->SetActive(true);

		}

		m_bTestBool = false;
		//m_bEnable = false;

	}

}
