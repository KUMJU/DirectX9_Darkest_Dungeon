#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE { MODE_FULL, MODE_WIN };

	// Dynamic 컴포넌트 경우 매 프레임마다 갱신해야하는 컴포넌트 집단
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum class ECollideID { PLAYER, WALL, ITEM, ENVIRONMENT, HERO, BOSS, PLAYER_PROJECTILE, BOSS_PROJECTILE, BOSS_SUNKEN, MOB, TRIGGER, SCENE_CHANGE_TRIGGER, ETC, ENUM_END };
	enum class EDirection { TOP, RIGHT, BOTTOM, LEFT, ENUM_END };



	enum INFO {	INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_ALPHA, TEX_END };

	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_FRONT, RENDER_UI, RENDER_LAST, RENDER_MOUSE, RENDER_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE {	DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

}
#endif // Engine_Enum_h__
