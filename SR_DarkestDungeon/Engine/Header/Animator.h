#pragma once


#include"Component.h"


BEGIN(Engine)

struct tagFrame {
	_int iCurrentFrame;
	_int iMaxFrame;
	_float fTime;
};

class ENGINE_DLL CAnimator : public CComponent
{
public:
	explicit CAnimator(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAnimator();

public:

	//Animation Key ���� Ȥ�� �ʱ� ���� �� ���
	HRESULT	SetAnimKey(const tstring& _keyName, float _fAnimTime);

	//�� �����Ӹ��� �̹��� �������� �ѱ��� Ȯ���ؾ��ϱ� ������ Texture�� �ٸ��� Update�� ���� ����
	virtual		_int	UpdateComponent(const _float& fTimeDelta);
	virtual		void	LateUpdateComponent();

	void		SetAnimTexture();

public:
	//�ִϸ��̼��� loop�� ������ �ʰ� �ѹ��� ������ 
	_bool CheckFinish() { return m_bFinishCount; }

private:
	vector<IDirect3DBaseTexture9*>* m_vecTexture;

private:

	tagFrame m_tAnimFrame;

	_float m_fActTime= 0.f;
	_bool m_bFinishCount = false;

};

END