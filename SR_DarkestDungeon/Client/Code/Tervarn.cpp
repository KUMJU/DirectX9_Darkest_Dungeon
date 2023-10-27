#include "pch.h"
#include "Tervarn.h"

#include "Export_Utility.h"
#include "Export_System.h"

#include "Terrain.h"

CTervarn::CTervarn(LPDIRECT3DDEVICE9 pGraphicDev)
    : CInside(pGraphicDev)
{
}

CTervarn::CTervarn(const CTervarn& rhs)
    : CInside(rhs)
{
}

CTervarn::~CTervarn()
{
}

HRESULT CTervarn::ReadyGameObject()
{


    return __super::ReadyGameObject();
}

_int CTervarn::UpdateGameObject(const _float& fTimeDelta)
{

    _int iExit = __super::UpdateGameObject(fTimeDelta);

    return iExit;
}

void CTervarn::LateUpdateGameObject()
{
    __super::LateUpdateGameObject();
}

void CTervarn::RenderGameObject()
{
    __super::RenderGameObject();
}