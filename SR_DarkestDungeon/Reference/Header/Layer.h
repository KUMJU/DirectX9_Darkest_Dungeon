#pragma once

#include"Engine_Define.h"
#include"GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer
{
public:
	explicit CLayer();
	virtual ~CLayer();

public:
	void				AwakeLayer();
	HRESULT				ReadyLayer();
	_int				UpdateLayer(const _float& fTimeDelta);
	void				LateUpdateLayer();

public:
	//�� GameObj ���� 
	HRESULT				CreateGameObject(tstring _strObjName, shared_ptr<CGameObject> _pObject);
	void				SetColliderVisible(_bool _bCollderVisible) { m_bColliderVisible = _bCollderVisible; }

public:
	shared_ptr<CComponent> GetComponent(const tstring& _strObjName, const tstring& _strComName, COMPONENTID _eID);
	HRESULT AddNewObject(const tstring& _strObjKey, shared_ptr<CGameObject> _pObj);

private:
	map<tstring, list<shared_ptr<CGameObject>>> m_objectMap;
	_bool	m_bColliderVisible = false;

};

END
