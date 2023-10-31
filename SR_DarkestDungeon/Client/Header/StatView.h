#pragma once

#include"GameObject.h"


BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END


class CStatView : public CGameObject
{
public:
	explicit CStatView(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CStatView();
public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

public:
	//�ʱ�ȭ
	void	SettingInit(_vec3 _vCreaturePos, _int _iHp, _int _iMaxHp, _bool _isHero);
	void	SettingPos(_vec3 _vPos);

public:
	//���� Creature���� ����ؾ��ϴ� ��ġ ���� �Լ� -------------------------------------------------------
	void SetStress(int _iStress);
	void SetHp(int _iHp);

	// 0 : �ߵ� , 1: ���� , 2: ���� , 3: ���, 4: �ر�
	void SetAttribute(_int _iTypeNum) { m_bAttributeArr[_iTypeNum] = true;  }
	void SetAttributeOff(_int _iTypeNum) { m_bAttributeArr[_iTypeNum] = false;  }

	//------------------------------------------------------------------------------------------------------
private:
	void	AddComponent();
	//�� Stat������ ������ ũ������ Top Position�� ��� �׿� ���� �������ش�

private:

	//0~9 : ��Ʈ���� /10~11 : ü�¹� /12~16 :���°� 
	shared_ptr<CRcTex> m_pRCTexCom[17];
	shared_ptr<CTransform> m_pTransformCom[17];
	//0:��Ʈ����X 1:��Ʈ����o 2:�� HP�� 3:HP�� 4: �ߵ� 5:���� 6:���� 7: ��� 8: �ر�
	shared_ptr<CTexture> m_pTextureCom[9];

	_bool m_bAttributeArr[5] = {false, false, false, false, false};

	_vec3 m_vCenterPos; //���� ����â�� ���� ��ǥ 

	_int m_iStress = 0;
	_int m_iHp;
	_int m_iMaxHp;

	//���� ��Ʈ���� UI ON ����
	_int m_iStressNum = 0;
	//��ü ü�¿� ���� ���� ü�� ����
	_float m_fHpBarRange = 1.f;

	_bool m_bHero = true;

};

