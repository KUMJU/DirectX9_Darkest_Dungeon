#include "Renderer.h"

IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer()
{
}

CRenderer::~CRenderer()
{
}

void CRenderer::AddRenderGroup(RENDERID eType, shared_ptr<CGameObject> pGameObject)
{
	if(RENDER_END < eType || nullptr == pGameObject)
		return;

	m_RenderList[eType].push_back(pGameObject);
}

void CRenderer::RenderGameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	RenderPriority(pGraphicDev);
	RenderNonalpha(pGraphicDev);
	RenderAlpha(pGraphicDev);
	RenderUI(pGraphicDev);

	ClearRenderGroup();

}

void CRenderer::ClearRenderGroup()
{

	for (size_t i = 0; i < RENDER_END; ++i)
	{
		m_RenderList[i].clear();
	}

}

void CRenderer::RenderPriority(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (auto& iter : m_RenderList[RENDER_PRIORITY])
		iter->RenderGameObject();

	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CRenderer::RenderNonalpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for (auto& iter : m_RenderList[RENDER_NONALPHA]) {

		iter->RenderGameObject();
	}
}

void CRenderer::RenderAlpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//// 알파 테스트
	/*pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);*/

	//m_RenderList[RENDER_ALPHA].sort([](CGameObject* pDst, CGameObject* pSrc)->bool { return pDst->Get_ViewZ() > pSrc->Get_ViewZ();  });

	for (auto& iter : m_RenderList[RENDER_ALPHA])
		iter->RenderGameObject();

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


}

void CRenderer::RenderUI(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for (auto& iter : m_RenderList[RENDER_UI]) {
		


	}
		//iter->Render_GameObject();
}
