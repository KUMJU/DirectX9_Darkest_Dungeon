#pragma once
#include "UIObj.h"
#include "Skill.h"

BEGIN(Engine)

class CTexture;

END

class CHero;

class CHeroStat : public CUIObj
{
public:
	explicit CHeroStat(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CHeroStat();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

public:
	virtual void PickingUI(LONG _fX, LONG _fY) override;
	
public:
	void	SetHero(shared_ptr<CHero> _pHero) { m_pHero = _pHero; }

protected:
	void	AddComponent();
	void	HoverUI(LONG _fX, LONG _fY);

private:
	weak_ptr<CHero>	m_pHero;
	_int m_iCurrentCursor = 0;

	tstring				m_strBackKey;

	vector<shared_ptr<CSkill>>* m_pVecSkill;

	// 0~3 : ��õ ��ġ / 4~7 : ��õ ��ǥ / 8~13 : ��ų �̹��� / 14 ~ 17 : Ȱ�� ��ų ǥ��
	shared_ptr<CRcTex> m_pRCTexCom[20];
	shared_ptr<CTransform> m_pTransformCom[20];

	// 0: Ʋ
	// 1~4 : ��õ ��ġ1~4
	// 5~8 : ��õ ��ǥ1~4
	// 9~14: ��ų �̹���
	// 15 : ��ų Ȱ�� ǥ��
	// 16 : ��ų ��� ǥ��
	shared_ptr<CTexture> m_pTextureCom[17];

	//_int	m_iRecommend
};

