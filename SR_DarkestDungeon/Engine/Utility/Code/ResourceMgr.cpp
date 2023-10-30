#include "ResourceMgr.h"

IMPLEMENT_SINGLETON(CResourceMgr)

CResourceMgr::CResourceMgr()
{
	
}

CResourceMgr::~CResourceMgr()
{
}

void CResourceMgr::ReadyResource(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	m_pGraphicDev = _pGraphicDev;

}

HRESULT CResourceMgr::CreateNewTexture(const tstring& _KeyName, TEXTUREID _eType, wstring _pPath, const _uint& _iCnt, _bool _bDelete)
{
	if (_eType >= TEXTUREID::TEX_END)
		return E_FAIL;
	
	IDirect3DBaseTexture9* pTexture = nullptr;

	vector<shared_ptr<tagTextureInfo>> vTempVec;
	vTempVec.reserve(_iCnt);
	
	for (_uint i = 0; i < _iCnt; ++i)
	{
		shared_ptr<tagTextureInfo> pInfo = make_shared<tagTextureInfo>();
		pInfo->bDelete = _bDelete;

		TCHAR		szFileName[128] = L"";
		wstring strPathCopy = _pPath;

		if (i < 10 && 1 != _iCnt) {
			size_t iSize = strPathCopy.find(L"%");
			strPathCopy.insert(iSize, L"0");
		}

		wsprintf(szFileName, strPathCopy.c_str(), i);

		switch (_eType)
		{
		case TEX_NORMAL:

			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
				return E_FAIL;

			break;

		case TEX_CUBE:
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		}

		pInfo->pTexture = pTexture;

		vTempVec.push_back(pInfo);
	}

	if (TEXTUREID::TEX_CUBE == _eType) {

		m_CubeMap.insert({ _KeyName, vTempVec });
	}
	else if (TEXTUREID::TEX_NORMAL == _eType)
		m_TextureMap.insert({ _KeyName,vTempVec });

	return S_OK;
}

vector<shared_ptr<tagTextureInfo>>* CResourceMgr::GetTexture(const tstring& _keyName, TEXTUREID _eType)
{

	if (_eType == TEXTUREID::TEX_NORMAL) {
		auto iter = m_TextureMap.find(_keyName);

		if (iter == m_TextureMap.end())
			return nullptr;

		return &(iter->second);
	}
	else if (_eType == TEXTUREID::TEX_CUBE) {
		auto iter = m_CubeMap.find(_keyName);

		if (iter == m_CubeMap.end())
			return nullptr;

		return &(iter->second);
	}

	return nullptr;
}

void CResourceMgr::RemoveAllTexture()
{
	for (auto& iter : m_TextureMap) {

		for (auto& it : iter.second) {
			it->pTexture->Release();
		}
		//for_each(iter.second.begin(), iter.second.end(), CDeleteObj());
	}

	m_TextureMap.clear();

	//for (auto& iter : m_CubeMap) {
	//	for_each(iter.second.begin(), iter.second.end(), CDeleteObj());
	//	iter.second.clear();
	//	m_CubeMap.erase(iter.first);
	//}

}

void CResourceMgr::RemoveSceneTexture()
{
	for (auto& iter : m_TextureMap) {
		//bDelete가 true인 요소만 골라서 삭제
		if (iter.second.front()->bDelete == true) {
			for (auto& it : iter.second) {
				it->pTexture->Release();
			}
			m_TextureMap.erase(iter.first);
		}
	}

}

void CResourceMgr::Free()
{
	RemoveAllTexture();
}

void CResourceMgr::BaseTextureLoad()
{
	_bool bDefault = false;

	//PlayerHandItem
	CreateNewTexture(L"Player_Item_Shovel", TEX_NORMAL,
		L"../Bin/Resource/Image/PlayerItem/supply_shovel.png", 1, bDefault);
	CreateNewTexture(L"Player_Item_Antivenom", TEX_NORMAL,
		L"../Bin/Resource/Image/PlayerItem/supply_antivenom.png", 1, bDefault);
	CreateNewTexture(L"Player_Item_Bandage", TEX_NORMAL,
		L"../Bin/Resource/Image/PlayerItem/supply_bandage.png", 1, bDefault);


	//Hero
	CreateNewTexture(L"Hero_Death", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/5.png", 1, bDefault);

	//방패파괴자
	{
		// 영웅 애니메이션

		CreateNewTexture(L"ShieldBreaker_Combat", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Combat/armatureName_combat_%d.png", 13, bDefault);
		CreateNewTexture(L"ShieldBreaker_Idle", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Idle/armatureName_idle_%d.png", 15, bDefault);
		CreateNewTexture(L"ShieldBreaker_Walk", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Walk/armatureName_walk_%d.png", 16, bDefault);

		CreateNewTexture(L"ShieldBreaker_Pierce", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Pierce.png", 1, bDefault);
		CreateNewTexture(L"ShieldBreaker_Puncture", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Puncture.png", 1, bDefault);
		CreateNewTexture(L"ShieldBreaker_AddersKiss", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/AddersKiss.png", 1, bDefault);
		CreateNewTexture(L"ShieldBreaker_Impale", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Impale.png", 1, bDefault);

		CreateNewTexture(L"ShieldBreaker_Defend", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Defend.png", 1, bDefault);
		CreateNewTexture(L"ShieldBreaker_Affliction", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Afflicted.png", 1, bDefault);
		CreateNewTexture(L"ShieldBreaker_Virtue", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Virtue.png", 1, bDefault);


		// 스킬 이미지

		CreateNewTexture(L"Pierce_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Skill/Pierce_Img.png", 1, bDefault);
		CreateNewTexture(L"Puncture_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Skill/Puncture_Img.png", 1, bDefault);
		CreateNewTexture(L"AddersKiss_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Skill/AddersKiss_Img.png", 1, bDefault);
		CreateNewTexture(L"Impale_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Skill/Impale_Img.png", 1, bDefault);


	// 스킬 이펙트 (본인에게)

		CreateNewTexture(L"AddersKiss_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/fx/adders_kiss/armatureName_adders_kiss_%d.png", 20, bDefault);
		CreateNewTexture(L"Pierce_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/fx/pierce/armatureName_pierce_%d.png", 15, bDefault);
		CreateNewTexture(L"Puncture_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/fx/break_guard/armatureName_break_guard_%d.png", 20, bDefault);
		CreateNewTexture(L"Impale_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/fx/spearing/armatureName_spearing_%d.png", 16, bDefault);
	
	
	
		// 스킬 이펙트 (타겟)
	
		CreateNewTexture(L"AddersKiss_Target_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/fx/adders_kiss_target/armatureName_adders_kiss_target_%d.png", 20, bDefault);
		CreateNewTexture(L"Impale_Target_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/fx/spearing_target/armatureName_spearing_target_%d.png", 20, bDefault);
	}

	//노상강도
	{
		CreateNewTexture(L"Highwayman_Combat", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Combat/armatureName_combat_%d.png", 14, bDefault);
		CreateNewTexture(L"Highwayman_Idle", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Idle/armatureName_idle_%d.png", 15, bDefault);
		CreateNewTexture(L"Highwayman_Walk", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Walk/armatureName_walk_%d.png", 16, bDefault);

		CreateNewTexture(L"Highwayman_WickedSlice", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/WickedSlice.png", 1, bDefault);
		CreateNewTexture(L"Highwayman_PistolShot", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/PistolShot.png", 1, bDefault);
		CreateNewTexture(L"Highwayman_PointBlankShot", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/PointBlankShot.png", 1, bDefault);
		CreateNewTexture(L"Highwayman_DuelistsAdvance", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/DuelistsAdvance.png", 1, bDefault);

		CreateNewTexture(L"Highwayman_Defend", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Defend.png", 1, bDefault);
		CreateNewTexture(L"Highwayman_Affliction", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Afflicted.png", 1, bDefault);
		CreateNewTexture(L"Highwayman_Virtue", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Virtue.png", 1, bDefault);
	

	// 스킬 이미지
	
		CreateNewTexture(L"WickedSlice_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Skill/WickedSlice_Img.png", 1, bDefault);
		CreateNewTexture(L"PistolShot_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Skill/PistolShot_Img.png", 1, bDefault);
		CreateNewTexture(L"PointBlankShot_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Skill/PointBlankShot_Img.png", 1, bDefault);
		CreateNewTexture(L"DuelistsAdvance_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Skill/DuelistsAdvance_Img.png", 1, bDefault);
	

	// 스킬 이펙트 (본인에게)
	
		CreateNewTexture(L"PointBlankShot_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/fx/point_blank_shot/armatureName_point_blank_shot_%d.png", 19, bDefault);
	

	// 스킬 이펙트 (타겟)
	
		CreateNewTexture(L"PistolShot_Target_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/fx/pistol_shot_target/armatureName_pistol_shot_target_%d.png", 15, bDefault);
		CreateNewTexture(L"PointBlankShot_Target_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/fx/point_blank_shot_target/armatureName_point_blank_shot_target_%d.png", 15, bDefault);
	}

	//성녀
	{
		CreateNewTexture(L"Vestal_Combat", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Vestal/Combat/armatureName_combat_%d.png", 16, bDefault);
		CreateNewTexture(L"Vestal_Idle", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Vestal/Idle/armatureName_idle_%d.png", 18, bDefault);
		CreateNewTexture(L"Vestal_Walk", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Vestal/Walk/armatureName_walk_%d.png", 16, bDefault);

		CreateNewTexture(L"Vestal_MaceBash", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Vestal/Attack.png", 1, bDefault);
		CreateNewTexture(L"Vestal_DazzlingLight", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Vestal/Stun.png", 1, bDefault);
		CreateNewTexture(L"Vestal_DivineGrace", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Vestal/Heal.png", 1, bDefault);
		CreateNewTexture(L"Vestal_DivineComfort", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Vestal/Heal.png", 1, bDefault);

		CreateNewTexture(L"Vestal_Defend", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Vestal/Defend.png", 1, bDefault);
		CreateNewTexture(L"Vestal_Affliction", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Vestal/Afflicted.png", 1, bDefault);
		CreateNewTexture(L"Vestal_Virtue", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Vestal/Virtue.png", 1, bDefault);

	// 스킬 이미지
	
		CreateNewTexture(L"MaceBash_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Vestal/Skill/MaceBashImg.png", 1, bDefault);
		CreateNewTexture(L"DazzlingLight_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Vestal/Skill/DazzlingLightImg.png", 1, bDefault);
		CreateNewTexture(L"DivineGrace_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Vestal/Skill/DivineGraceImg.png", 1, bDefault);
		CreateNewTexture(L"DivineComfort_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Vestal/Skill/DivineComfortImg.png", 1, bDefault);
	

	// 스킬 이펙트 (본인에게)
	
		CreateNewTexture(L"DazzlingLight_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Vestal/fx/dazzling_light/armatureName_dazzling_light_%d.png", 20, bDefault);
		CreateNewTexture(L"DivineGrace_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Vestal/fx/divine_grace/armatureName_divine_grace_%d.png", 20, bDefault);
		CreateNewTexture(L"DivineComfort_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Vestal/fx/gods_comfort/armatureName_gods_comfort_%d.png", 20, bDefault);
		//CreateNewTexture(L"Test_Effect", TEX_NORMAL,
		//	L"../Bin/Resource/Image/Creatures/Heroes/Vestal/fx/gods_illumination/armatureName_gods_illumination_%d.png", 40);
	

	// 스킬 이펙트 (타겟)
	
		CreateNewTexture(L"DazzlingLight_Target_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Vestal/fx/dazzling_light_target/armatureName_dazzling_light_target_%d.png", 20, bDefault);
		CreateNewTexture(L"DivineComfort_Target_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Vestal/fx/gods_comfort_target/armatureName_gods_comfort_target_%d.png", 20, bDefault);
		//CreateNewTexture(L"Test_Target_Effect", TEX_NORMAL,
		//	L"../Bin/Resource/Image/Creatures/Heroes/Vestal/fx/gods_illumination_target/armatureName_gods_illumination_target_%d.png", 40);
	}

	//광대
	{
		CreateNewTexture(L"Jester_Combat", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/Combat/armatureName_combat_%d.png", 14, bDefault);
		CreateNewTexture(L"Jester_Idle", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/Idle/armatureName_idle_%d.png", 14, bDefault);
		CreateNewTexture(L"Jester_Walk", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/Walk/armatureName_walk_%d.png", 16, bDefault);

		CreateNewTexture(L"Jester_DirkStab", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/DirkStab.png", 1, bDefault);
		CreateNewTexture(L"Jester_Harvest", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/Harvest.png", 1, bDefault);
		CreateNewTexture(L"Jester_Finale", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/Finale.png", 1, bDefault);
		CreateNewTexture(L"Jester_Solo", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/Solo.png", 1, bDefault);
		CreateNewTexture(L"Jester_SliceOff", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/SliceOff.png", 1, bDefault);
		CreateNewTexture(L"Jester_BattleBallad", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/BattleBallad.png", 1, bDefault);

		CreateNewTexture(L"Jester_Defend", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/Defend.png", 1, bDefault);
		CreateNewTexture(L"Jester_Affliction", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/Afflicted.png", 1, bDefault);
		CreateNewTexture(L"Jester_Virtue", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/Virtue.png", 1, bDefault);


		// 스킬 이미지

		CreateNewTexture(L"BattleBallad_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/Skill/BattleBallad_Img.png", 1, bDefault);
		CreateNewTexture(L"DirkStab_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/Skill/DirkStab_Img.png", 1, bDefault);
		CreateNewTexture(L"Finale_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/Skill/Finale_Img.png", 1, bDefault);
		CreateNewTexture(L"Harvest_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/Skill/Harvest_Img.png", 1, bDefault);
		CreateNewTexture(L"SliceOff_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/Skill/SliceOff_Img.png", 1, bDefault);
		CreateNewTexture(L"Solo_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/Skill/Solo_Img.png", 1, bDefault);


		// 스킬 이펙트 (본인에게)

		CreateNewTexture(L"BattleBallad_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/fx/battle_ballad/armatureName_battle_ballad_%d.png", 28, bDefault);
		CreateNewTexture(L"Finale_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/fx/finale/armatureName_finale_%d.png", 20, bDefault);
		CreateNewTexture(L"Solo_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/fx/solo/armatureName_solo_%d.png", 28, bDefault);


		// 스킬 이펙트 (타겟)

		CreateNewTexture(L"BattleBallad_Target_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/fx/battle_ballad_target/armatureName_battle_ballad_target_%d.png", 20, bDefault);
		CreateNewTexture(L"Finale_Target_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/fx/finale_target/armatureName_finale_target_%d.png", 20, bDefault);
		CreateNewTexture(L"Solo_Target_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Jester/fx/solo_target/armatureName_solo_target_%d.png", 20, bDefault);

	}

}

void CResourceMgr::UITextureLoad()
{
	_bool bDefault = false;

	//UIResource
	CreateNewTexture(L"UI_Inventory", TEX_NORMAL,
		L"../Bin/Resource/Image/UI/Dungeon/panel_inventory.png", 1, bDefault);

	//InventoryUI
	CreateNewTexture(L"Item_UI_Antivenom", TEX_NORMAL,
		L"../Bin/Resource/Image/UI/Item/supply/inv_supply+antivenom.png", 1, bDefault);
	CreateNewTexture(L"Item_UI_Shovel", TEX_NORMAL,
		L"../Bin/Resource/Image/UI/Item/supply/inv_supply+shovel.png", 1, bDefault);
	CreateNewTexture(L"Item_UI_Bandage", TEX_NORMAL,
		L"../Bin/Resource/Image/UI/Item/supply/inv_supply+bandage.png", 1, bDefault);

}

void CResourceMgr::WealdDungeonTextureLoad()
{
	_bool bDefault = true;

	// Environment
	CreateNewTexture(L"Com_Weald_FloorTexture", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/BackGround/Weald/forest_Tiles.png", 1 , bDefault);

	CreateNewTexture(L"Com_Weald_WallTexture", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/BackGround/Weald/weald.corridor_wall.%d.png", 9, bDefault);
	CreateNewTexture(L"Com_Weald_BackWallTexture", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/BackGround/Weald/weald.corridor_mid.png", 1, bDefault);


	// 문
	CreateNewTexture(L"Weald_Door_Closed", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/BackGround/Weald/weald.corridor_door.basic.png", 1, bDefault);
	CreateNewTexture(L"Weald_Door_Opened", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/BackGround/Weald/weald.corridor_door.Opened.png", 1, bDefault);

	// 골동품
	CreateNewTexture(L"Weald_Spiderweb", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/Curios/Weald/cosmic_spiderweb/idle/armatureName_idle_0.png", 1, bDefault);

	CreateNewTexture(L"Weald_Spiderweb_Finish", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/Curios/Weald/cosmic_spiderweb/investigate/armatureName_investigate_0.png", 1, bDefault);

	CreateNewTexture(L"Weald_heirloom_chest", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/Curios/Weald/heirloom_chest/idle/armatureName_idle_0.png", 1, bDefault);

	CreateNewTexture(L"Weald_heirloom_chest_Finish", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/Curios/Weald/heirloom_chest/investigate/armatureName_investigate_0.png", 1, bDefault);

	CreateNewTexture(L"Weald_lost_luggage", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/Curios/Weald/lost_luggage/idle/armatureName_idle_0.png", 1, bDefault);

	CreateNewTexture(L"Weald_lost_luggage_Finish", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/Curios/Weald/lost_luggage/investigate/armatureName_investigate_0.png", 1, bDefault);

	CreateNewTexture(L"Weald_mummified_remains", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/Curios/Weald/mummified_remains/idle/armatureName_idle_0.png", 1, bDefault);

	CreateNewTexture(L"Weald_mummified_remains_Finish", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/Curios/Weald/mummified_remains/investigate/armatureName_investigate_0.png", 1, bDefault);
	
	CreateNewTexture(L"Weald_travellers_tent_tutorial", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/Curios/Weald/travellers_tent_tutorial/idle/armatureName_idle_0.png", 1, bDefault);

	CreateNewTexture(L"Weald_travellers_tent_tutorial_Finish", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/Curios/Weald/travellers_tent_tutorial/investigate/armatureName_investigate_0.png", 1, bDefault);

	// 함정
	CreateNewTexture(L"Weald_Traps_Remains", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/Traps/Weald/idle/armatureName_idle_0.png", 1, bDefault);

	CreateNewTexture(L"Weald_Traps_Sprung", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/Traps/Weald/sprung/armatureName_sprung_%d.png", 66, bDefault);

	// 장애물
	CreateNewTexture(L"Weald_Obstacles_Remains", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/Obstacles/Weald/armatureName_clear_00.png", 1, bDefault);

	CreateNewTexture(L"Weald_Obstacles", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/Obstacles/Weald/armatureName_clear_%d.png", 60, bDefault);

	// 배경이미지
	CreateNewTexture(L"Weald_Image_Wagon", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/BackGround/Weald/weald.tutorial.room_wall.effigy_0.png", 1, bDefault);

	CreateNewTexture(L"Weald_Image_PoisonRiver", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/BackGround/Weald/weald.room_wall.poisonriver.png", 1, bDefault);
	
	// SkyBox
	CreateNewTexture(L"Com_Weald_SkyBoxTexture", TEX_CUBE,
		L"../Bin/Resource/Image/SkyBox/WealdSkyBox.dds", 1, bDefault);

	//Brigand Bloodletter
	CreateNewTexture(L"Brigand Bloodletter_Combat", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Bloodletter/Combat/armatureName_combat_%d.png", 36, bDefault);
	CreateNewTexture(L"Brigand Bloodletter_Attack1", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Bloodletter/1.png", 1, bDefault);
	CreateNewTexture(L"Brigand Bloodletter_Attack2", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Bloodletter/2.png", 1, bDefault);
	CreateNewTexture(L"Brigand Bloodletter_Dead", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Bloodletter/3.png", 1, bDefault);
	CreateNewTexture(L"Brigand Bloodletter_Hitted", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Bloodletter/4.png", 1, bDefault);
	CreateNewTexture(L"Brigand Bloodletter_Death", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/5.png", 1, bDefault);

	//Bone Soldier
	CreateNewTexture(L"Bone Soldier_Combat", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Ruin/Bone Soldier/Bone Soldier/combat/armatureName_combat_%d.png", 28, bDefault);
	CreateNewTexture(L"Bone Soldier_Attack1", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Ruin/Bone Soldier/1.png", 1, bDefault);
	CreateNewTexture(L"Bone Soldier_Dead", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Ruin/Bone Soldier/2.png", 1, bDefault);
	CreateNewTexture(L"Bone Soldier_Hitted", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Ruin/Bone Soldier/3.png", 1, bDefault);
	CreateNewTexture(L"Bone Soldier_Death", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/5.png", 1, bDefault);


	CreateNewTexture(L"Bone Defender_Combat", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Ruin/Bone Defender/Combat/armatureName_combat_%d.png", 30, bDefault);
	CreateNewTexture(L"Bone Defender_Attack1", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Ruin/Bone Defender/1.png", 1, bDefault);
	CreateNewTexture(L"Bone Defender_Attack2", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Ruin/Bone Defender/2.png", 1, bDefault);
	CreateNewTexture(L"Bone Defender_Dead", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Ruin/Bone Defender/3.png", 1, bDefault);
	CreateNewTexture(L"Bone Defender_Hitted", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Ruin/Bone Defender/4.png", 1, bDefault);
	CreateNewTexture(L"Bone Defender_Death", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/5.png", 1, bDefault);

	CreateNewTexture(L"Bone Courtier_Combat", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Ruin/Bone Courtier/Combat/armatureName_combat_%d.png", 26, bDefault);
	CreateNewTexture(L"Bone Courtier_Attack1", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Ruin/Bone Courtier/1.png", 1, bDefault);
	CreateNewTexture(L"Bone Courtier_Attack2", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Ruin/Bone Courtier/2.png", 1, bDefault);
	CreateNewTexture(L"Bone Courtier_Dead", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Ruin/Bone Courtier/3.png", 1, bDefault);
	CreateNewTexture(L"Bone Courtier_Hitted", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Ruin/Bone Courtier/4.png", 1, bDefault);
	CreateNewTexture(L"Bone Courtier_Death", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/5.png", 1, bDefault);



	CreateNewTexture(L"Brigand Cutthroat_Combat", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/Combat/armatureName_combat_%d.png", 33, bDefault);
	CreateNewTexture(L"Brigand Cutthroat_Attack1", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/1.png", 1, bDefault);
	CreateNewTexture(L"Brigand Cutthroat_Attack2", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/2.png", 1, bDefault);
	CreateNewTexture(L"Brigand Cutthroat_Dead", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/3.png", 1, bDefault);
	CreateNewTexture(L"Brigand Cutthroat_Hitted", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/4.png", 1, bDefault);
	CreateNewTexture(L"Brigand Cutthroat_Death", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/5.png", 1, bDefault);


	CreateNewTexture(L"Brigand Fusilier_Combat", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Fusilier/Combat/armatureName_combat_%d.png", 34, bDefault);
	CreateNewTexture(L"Brigand Fusilier_Attack1", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Fusilier/1.png", 1, bDefault);
	CreateNewTexture(L"Brigand Fusilier_Dead", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Fusilier/2.png", 1, bDefault);
	CreateNewTexture(L"Brigand Fusilier_Hitted", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Fusilier/3.png", 1, bDefault);
	CreateNewTexture(L"Brigand Fusilier_Death", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/5.png", 1, bDefault);


}

void CResourceMgr::VillageTextureLoad()
{
	_bool bDefault = true;

	CreateNewTexture(L"Village_Door_Open", TEX_NORMAL,
		L"../Bin/Resource/Image/Village/Interaction/Door/open.png", 1, bDefault);

	CreateNewTexture(L"Village_Door_Close", TEX_NORMAL,
		L"../Bin/Resource/Image/Village/Interaction/Door/closed.png", 1, bDefault);

	// 마을
	{
		//바닥
		CreateNewTexture(L"Village_FloorTexture", TEX_NORMAL,
			L"../Bin/Resource/Image/Village/Floor/WholeTerrain2.jpg", 1, bDefault);

		//외곽 나무 그림자
		CreateNewTexture(L"Com_Village_Wall_Texture", TEX_NORMAL,
			L"../Bin/Resource/Image/Village/BackGround/forest_skirt_%d.png", 3, bDefault);

		//외곽 건물 그림자
		CreateNewTexture(L"Com_Village_Back_Texture", TEX_NORMAL,
			L"../Bin/Resource/Image/Village/BackGround/city_back_%d.png", 3, bDefault);

		//플레이어 양옆 건물
		CreateNewTexture(L"Com_Village_Left_Buildings_Texture", TEX_NORMAL,
			L"../Bin/Resource/Image/Village/Wall/BG_Buildings_Left.png", 1, bDefault);
		CreateNewTexture(L"Com_Village_Right_Buildings_Texture", TEX_NORMAL,
			L"../Bin/Resource/Image/Village/Wall/BG_Buildings_Right.png", 1, bDefault);

		// SkyBox
		CreateNewTexture(L"Village_SkyBoxTexture", TEX_CUBE,
			L"../Bin/Resource/Image/SkyBox/VillageSkyBox.dds", 1, bDefault);
	}

	// 상호작용 건물 외부
	{
		// 여관
		CreateNewTexture(L"Tervarn_Outside", TEX_NORMAL,
			L"../Bin/Resource/Image/Village/Facilities/OutSide/Tavern/town_tavern_level03/armatureName_active_000.png", 1, bDefault);

		// 역마차
		CreateNewTexture(L"StageCoach_Outside", TEX_NORMAL,
			L"../Bin/Resource/Image/Village/Facilities/OutSide/Stagecoach/town_stage_coach_level01/idle.png", 1, bDefault);

		// 훈련소
		CreateNewTexture(L"Guild_Outside", TEX_NORMAL,
			L"../Bin/Resource/Image/Village/Facilities/OutSide/Guild/town_guild_level03/idle.png", 1, bDefault);

		// 상점
		CreateNewTexture(L"Store_Outside", TEX_NORMAL,
			L"../Bin/Resource/Image/Village/Facilities/OutSide/Nomad Wagon/town_nomad_wagon_level03/idle.png", 1, bDefault);

	}

	// 상호작용 건물 내부
	{
		CreateNewTexture(L"Inside_FloorTexture", TEX_NORMAL,
			L"../Bin/Resource/Image/Village/Floor/city_gate_floor_tile.png", 1, bDefault);

		CreateNewTexture(L"Wood_Wall_Texture", TEX_NORMAL,
			L"../Bin/Resource/Image/Village/Wall/Wood_Wall.png", 1, bDefault);

		CreateNewTexture(L"Gambling_Table_Texture", TEX_NORMAL,
			L"../Bin/Resource/Image/Village/Facilities/tavern/Gambling.png", 1, bDefault);
	}
}
