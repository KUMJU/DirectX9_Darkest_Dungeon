#include "pch.h"
#include "SceneEvent.h"

#include "UIMgr.h"

CSceneEvent::CSceneEvent(LPDIRECT3DDEVICE9 _pGraphicDev)
{
}

CSceneEvent::~CSceneEvent()
{
}

HRESULT CSceneEvent::ReadyGameObject()
{
    CUIMgr::GetInstance()->AllVisibleOff();

    return S_OK;
}

_int CSceneEvent::UpdateGameObject(const _float& fTimeDelta)
{
    m_fSceneActTime += fTimeDelta;

    //������ �̺�Ʈ �ð��� ������ ���� 
    if (m_fEventTime <= m_fSceneActTime)
        m_bEnable = false;


    return _int();
}

void CSceneEvent::LateUpdateGameObject()
{
}

void CSceneEvent::RenderGameObject()
{
}
