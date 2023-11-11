#pragma once

enum class EAttackType {
	ATTACK,			// �ܼ� ����
	BLIGHT,			// �ߵ�
	BLEED,			// ����
	STUN,			// ����
	MOVE,			// �̵�
	HEAL,			// ȸ��
	STRESS,			// ��Ʈ����
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
	DUNGEON2,

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

enum class ERuinDungeonDoorType
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
	SHOVEL,				// ��
	ANTI_VENOM,			// �ص���
	KEYS,				// ����
	BANDAGE,			// �ش�
	GOLD,				// ���
	HEIRLOOM,			// ����
	FOOD,				// ����
	TORCH,				// ȶ��
	SPELLHAND_FIRE,		//������ ����
	SPELLHAND_STORM ,	//������ ����
	ENUM_END	
};