#pragma once

#include"SceneMgr.h"
#include "Renderer.h"
#include"GameMgr.h"
#include"ResourceMgr.h"

#include"TerrainTex.h"
#include"CubeTex.h"
#include"Texture.h"
#include"RcTex.h"
#include "Transform.h"
#include"Animator.h"


BEGIN(Engine)

//SceneManager

inline shared_ptr<CComponent> Get_Component(const tstring* pLayerTag, const tstring* pObjTag, const tstring* pComponentTag, COMPONENTID eID);

inline void ChangeScene(shared_ptr<CScene> _pScene);
inline _int			UpdateScene(const _float& _fTimeDelta);
inline void			LateUpdateScene();
inline void			RenderScene(LPDIRECT3DDEVICE9 _pGraphicDev);

// Renderer
inline void		AddRenderGroup(RENDERID eType, shared_ptr<CGameObject> _pGameObject);
inline void		RenderGameObject(LPDIRECT3DDEVICE9& _pGraphicDev);
inline void		ClearRenderGroup();

inline void		Release_Utility();

//ResourceManager
inline void		ReadyResourceMgr(LPDIRECT3DDEVICE9 _pGraphicDev);
inline void		CreateNewTexture(const tstring& _KeyName, TEXTUREID _eType, wstring _pPath, const _uint& _iCnt);

#include"Export_Utility.inl"

END