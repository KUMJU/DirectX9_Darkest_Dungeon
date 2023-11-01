#pragma once

#include"UIObj.h"

BEGIN(Engine)

class CTexture;
END

class CSkill;


class CBattleHeroUI : public CUIObj
{
public:
	explicit CBattleHeroUI(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CBattleHeroUI();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

public:
	void			SettingHeroInfo(_int _iMaxHP, _int _iHP, _int _iStress, tstring _strHeroName, vector<shared_ptr<CSkill>>* _vecSkill);
	virtual			void PickingUI(LONG _fX, LONG _fY) override;

protected:
	void			AddComponent();
	void			SetIconPos();

private:
	shared_ptr<CTexture> m_pTextureCom;

private:
	//체력(현재, max), 스트레스(현재/max), 직업군

	tstring m_strHeroName = L"";
	
	_int m_iMaxHP = 0;
	_int m_iHP = 0;

	_int m_iStress = 0;

	vector<shared_ptr<CSkill>>* m_pSkillVec= nullptr;
	
	shared_ptr<CRcTex> m_arrSkillRcTex[6];
	shared_ptr<CTransform> m_arrSkillTransform[6];
	shared_ptr<CTexture> m_arrSkillTexture[6];

};

