#include "pch.h"
#include "Ruin_Curio_Sconce.h"
#include "Player.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Creature.h"
#include "Hero.h"

CRuin_Curio_Sconce::CRuin_Curio_Sconce(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteractionObj(pGraphicDev)
{
}

CRuin_Curio_Sconce::CRuin_Curio_Sconce(const CRuin_Curio_Sconce& rhs)
	: CInteractionObj(rhs)
{
}

CRuin_Curio_Sconce::~CRuin_Curio_Sconce()
{
}

HRESULT CRuin_Curio_Sconce::ReadyGameObject()
{
	__super::ReadyGameObject();

	m_bInteractionKey = L"C";
	m_bInteractionInfo = L"�� ���̱�";

	D3DLIGHT9 _pLightInfo1;

	_pLightInfo1.Diffuse = { 1.f , 0.2f , 0.2f , 1.f };
	_pLightInfo1.Specular = { 1.f , 1.f, 1.f , 1.f };
	_pLightInfo1.Ambient = { 1.f ,1.f , 1.f , 1.f };
	_pLightInfo1.Position = { m_vPos.x, m_vPos.y + 1.5f  , m_vPos.z };
	_pLightInfo1.Range = 30.f;


	_pLightInfo1.Attenuation0 = 0.f;
	_pLightInfo1.Attenuation1 = 0.5f;
	_pLightInfo1.Attenuation2 = 0.f;

	shared_ptr<CLight> m_pLight = CLightMgr::GetInstance()->InitPointLight(m_pGraphicDev, _pLightInfo1, 4);

	return S_OK;
}

_int CRuin_Curio_Sconce::UpdateGameObject(const _float& fTimeDelta)
{
	_int	iExit = __super::UpdateGameObject(fTimeDelta);

	if (m_bActive)
	{
		m_fActiveTime -= fTimeDelta;
		if (m_fActiveTime < 0)
		{
			// ��ȣ�ۿ�
			if (!dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->GetEventTrigger())
			{
				dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->SetEventTrigger(true);
			}
			else if (!dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->GetEvent2Trigger())
			{
				dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->SetEvent2Trigger(true);
			}
			else if (!dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->GetEvent3Trigger())
			{
				dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->SetEvent3Trigger(true);
			}

			m_fActiveTime = CURIOACTIVETIME;
			m_bActive = false;
			m_ePrevAnimState = m_eCurAnimState;
			m_eCurAnimState = EState::FINISH;
		}
	}

	// Animation
	if (m_eCurAnimState != m_ePrevAnimState)
	{
		switch (m_eCurAnimState)
		{
		case EState::IDLE:
			m_pAnimatorCom->SetAnimKey(L"Ruin_Sconce", 0.05f);
			//m_pTransformCom->SetScale(WEALD_PATHSIZEX / 3.f, WEALD_PATHSIZEX / 3.f, 1.f);
			break;
		case EState::ACTIVE:
			m_pAnimatorCom->SetAnimKey(L"Ruin_Sconce", 0.02f);
			//m_pTransformCom->SetScale(WEALD_PATHSIZEX / 3.f, WEALD_PATHSIZEX / 3.f, 1.f);
			break;
		case EState::FINISH:
			m_pAnimatorCom->SetAnimKey(L"Ruin_Sconce_Finish", 0.02f);
			//m_pTransformCom->SetScale(WEALD_PATHSIZEX / 3.f, WEALD_PATHSIZEX / 3.f, 1.f);
			break;
		}
	}

	//������ ����
	_matrix matWorld;

	matWorld = *m_pTransformCom->GetWorld();
	SetBillBoard(matWorld);
	m_pTransformCom->SetWorld(&matWorld);

	return iExit;
}

void CRuin_Curio_Sconce::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CRuin_Curio_Sconce::RenderGameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorld());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	SetMaterial();

	m_pAnimatorCom->SetAnimTexture();

	m_pBufferCom->RenderBuffer();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CRuin_Curio_Sconce::AddComponent()
{

	shared_ptr<CComponent> pComponent;

	pComponent = m_pBufferCom = make_shared<CRcTex>(m_pGraphicDev);
	dynamic_pointer_cast<CRcTex>(m_pBufferCom)->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_RcTex", pComponent });

	//pComponent = m_pTextureCom = make_shared<CTexture>(m_pGraphicDev);
	//m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = make_shared<CTransform>(_vec3(0.f, 0.f, 0.f), _vec3(1.f, 1.f, 1.f), _vec3(0.f, 0.f, 0.f));
	m_pTransformCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	m_pTransformCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
	m_pTransformCom->SetScale(m_vScale.x, m_vScale.y, m_vScale.z);

	//pComponent = m_pTextureCom = make_shared<CTexture>(m_pGraphicDev);
	//m_pTextureCom->SetTextureKey(L"Weald_Traps_Remains", TEX_NORMAL);
	//m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture",pComponent });

	pComponent = m_pAnimatorCom = make_shared<CAnimator>(m_pGraphicDev);
	m_pAnimatorCom->SetAnimKey(L"Ruin_Sconce", 0.05f);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animator",pComponent });
}

void CRuin_Curio_Sconce::GetInteractionKey(const _float& fTimeDelta)
{
	// ���ͷ��� Ű UI �����ֱ�
	// ----------���� �ʿ�------------

	// Ű �Է¹ޱ�
	if (GetAsyncKeyState('C') & 0x8000)
	{
		//m_bInteracting = true;
		m_bActive = true;

		// �÷��̾� �ൿ ����
		//CGameMgr::GetInstance()->SetGameState(EGameState::LOCK);

		// �ؽ�ó �Ǵ� �ִϸ��̼� ����
		ChangeTexture();

		CSoundMgr::GetInstance()->PlaySound(L"prop_curio_sconce.wav", CHANNELID::CURIO, 1.f);


		Interaction();
	}
}

void CRuin_Curio_Sconce::Interaction()
{
}

void CRuin_Curio_Sconce::FinishInteraction()
{
	//m_bInteracting = false;

	//m_pTextureCom->SetTextureKey(L"Weald_Traps_Remains", TEX_NORMAL);
}

void CRuin_Curio_Sconce::ChangeTexture()
{
	m_ePrevAnimState = m_eCurAnimState;
	m_eCurAnimState = EState::ACTIVE;
}

_bool CRuin_Curio_Sconce::IsFinish()
{
	//// Ű �Է¹ޱ�
	//if (GetAsyncKeyState('X') & 0x8000)
	//{
	//	m_bInteracting = false;
	//	m_ePrevAnimState = m_eCurAnimState;
	//	m_eCurAnimState = EState::FINISH;
	//
	//	// �÷��̾� �ൿ Ǯ��
	//	//CGameMgr::GetInstance()->SetGameState(EGameState::PRCESS);
	//
	//	return true;
	//}

	return false;
}