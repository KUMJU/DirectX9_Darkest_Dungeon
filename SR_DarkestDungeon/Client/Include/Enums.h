#pragma once

enum class EAttackType {
	ATTACK,			// 단순 공격
	BLIGHT,			// 중독
	BLEED,			// 출혈
	STUN,			// 기절
	MOVE,			// 이동
	HEAL,			// 회복
	STRESS,			// 스트레스
	ENUM_END
};

enum class EFacilityType
{
	TERVARN,
	GUILD,
	STAGECOACH,
	STORE,

	ENUM_END
};

enum class ETerrainType
{
	VILLAGE,
	VILLAGE_INSIDE,
	DUNGEON,

	ENUM_END
};

enum class EHeroType
{
	VESTAL,
	HIGHWAYMAN,
	SHILEDBREAKER,
	JESTER,

	ENUM_END
};

enum class EWealdDungeonDoorType
{
	ROOM1,
	ROOM2,
	ROOM3,
	ROOM4,
	ROOM5,
	ROOM6,

	ENUM_END
};

enum class EState
{
	IDLE,
	ACTIVE,
	FINISH
};

enum class EHandItem {
	SHOVEL,			//삽
	ANTI_VENOM,		//해독제
	KEYS,			//열쇠
	BANDAGE,		//붕대
	ENUM_END	
};