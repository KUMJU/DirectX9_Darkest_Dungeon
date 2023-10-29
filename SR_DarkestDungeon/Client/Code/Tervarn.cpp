#include "pch.h"
#include "Tervarn.h"

#include "Export_Utility.h"
#include "Export_System.h"

#include "Terrain.h"
#include "Wall.h"
#include "Gambling.h"

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
    // Environment
    {
        // 바닥
        {
            shared_ptr<CGameObject> m_pTerrain = make_shared<CTerrain>(m_pGraphicDev, L"Inside_FloorTexture");
            m_pTerrain->SetPos(m_vPos);

            m_vecGameObject.push_back(m_pTerrain);
        }

        // 벽
        {
            shared_ptr<CGameObject> m_pWall;

            // 뒷쪽
            for (int i = 0; i < 5; ++i)
            {
                m_pWall = make_shared<CWall>(m_pGraphicDev, L"Wood_Wall_Texture", 1, false);
                m_pWall->SetScale(_vec3(VILLAGE_WALLSIZE * 3 / 5, VILLAGE_WALLSIZE * 3 / 2, 1));
                m_pWall->SetPos(_vec3(m_vPos.x + VILLAGE_TILESIZE * i / 5 * 3 + VILLAGE_TILESIZE / 10 * 3, m_vPos.y + m_pWall->GetScale().y - 1, m_vPos.z + VILLAGE_TILESIZE * 3 - 1));
                m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));

                m_vecGameObject.push_back(m_pWall);
            }

            // 왼쪽
            for (int i = 0; i < 5; ++i)
            {
                m_pWall = make_shared<CWall>(m_pGraphicDev, L"Wood_Wall_Texture", 1, false);
                m_pWall->SetScale(_vec3(VILLAGE_WALLSIZE * 3 / 5, VILLAGE_WALLSIZE * 3 / 2, 1));
                m_pWall->SetPos(_vec3(m_vPos.x, m_vPos.y + m_pWall->GetScale().y - 1, m_vPos.z + VILLAGE_TILESIZE * i / 5 * 3 + VILLAGE_TILESIZE / 10 * 3));
                m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));

                m_vecGameObject.push_back(m_pWall);
            }

            // 오른쪽
            for (int i = 0; i < 5; ++i)
            {
                m_pWall = make_shared<CWall>(m_pGraphicDev, L"Wood_Wall_Texture", 1, false);
                m_pWall->SetScale(_vec3(VILLAGE_WALLSIZE * 3 / 5, VILLAGE_WALLSIZE * 3 / 2, 1));
                m_pWall->SetPos(_vec3(m_vPos.x + VILLAGE_TILESIZE * 3 - 1, m_vPos.y + m_pWall->GetScale().y - 1, m_vPos.z + VILLAGE_TILESIZE * i / 5 * 3 + VILLAGE_TILESIZE / 10 * 3));
                m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));

                m_vecGameObject.push_back(m_pWall);
            }
        }

        // 천장
        {

        }
    }

    // GameObject
    {
        // 주점
        {

        }

        // 도박장
        shared_ptr<CGameObject> m_pGambling = make_shared<CGambling>(m_pGraphicDev);
        m_pGambling->SetScale({ 7.f, 5.f, 1.f });
        m_pGambling->SetPos({ m_vPos.x + VILLAGE_TILESIZE * 2, m_vPos.y + m_pGambling->GetScale().y / 2 + 1, m_vPos.z + VILLAGE_TILESIZE * 2 + 5.f });

        m_vecGameObject.push_back(m_pGambling);
    }

    return __super::ReadyGameObject();
}

_int CTervarn::UpdateGameObject(const _float& fTimeDelta)
{
    Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());

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