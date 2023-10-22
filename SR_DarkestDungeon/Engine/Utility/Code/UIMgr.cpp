#include "UIMgr.h"

IMPLEMENT_SINGLETON(CUIMgr)

CUIMgr::CUIMgr()
{
}

CUIMgr::~CUIMgr()
{
}

void CUIMgr::AddUIObject(EUIType _eType, shared_ptr<CGameObject>)
{
}

void CUIMgr::AllVisibleOn()
{
}

void CUIMgr::AllVisibleOff()
{
}

void CUIMgr::SelectUIVisibleOn(EUIType _eType)
{
}

void CUIMgr::SelectUIVisibleOn()
{
}
