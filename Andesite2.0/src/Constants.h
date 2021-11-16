#pragma once

namespace constants {
	constexpr int SCREEN_WIDTH = 1080;
	constexpr int SCREEN_HEIGHT = 720;
	constexpr float JUMP_FORCE = 20.0f;
	constexpr float JUMP_TIME = 15.0f;
	constexpr float PIXEL_PER_METER = 32.0f;
	constexpr float GOLEM_X_OFFSET_ANIMATION = 40.0f;
	constexpr float GOLEM_Y_OFFSET_ANIMATION = 5.0f;
	constexpr float PLAYER_X_OFFSET_ANIMATION = 150.0f;
	constexpr float PLAYER_Y_OFFSET_ANIMATION = 105.0f;
	constexpr float ROCK1_X_OFFSET_ANIMATION = 10.0f;
	constexpr float ROCK1_Y_OFFSET_ANIMATION = 5.0f;
	constexpr float EXPLOSION1_X_OFFSET_ANIMATION = 70.0f;
	constexpr float EXPLOSION1_Y_OFFSET_ANIMATION = 60.0f;
	constexpr float LAVA_Y_AXIS = 30.0f;
	constexpr float X_AXIS_WIN_LOCATION = 214.0f;
	constexpr float X_AXIS_END_OF_LEVEL_LOCATION = 227.0f;

	enum userDataTypes {
		USER_TYPE_PLAYER_FEET = 1,
		USER_TYPE_BOUDNARY = 2,
		USER_TYPE_PLAYER = 3,
		USER_TYPE_ENEMY = 4,
		USER_TYPE_ROCK = 5
	};

	enum categoryBits {
		NOTHING = 0x0000,
		BOUNDARY = 0x0001,
		PLAYER = 0x0002,
		ENEMY = 0x0004,
		ENEMY_HEAD = 0x0008,
		PLAYER_FEET = 0x0016,
		ROCK = 0x0032,
		GEM = 0x0064
	};

	enum MenuState {
		IN_MAIN_MENU,
		IN_GAME,
		IN_YOU_WON_MENU
	};

	enum PlayerState {
		Idle,
		Run,
		Jump,
		Fall,
		Die,
		Win
	};

	namespace EnemyStates
	{
		enum EnemyState {
			Idle,
			Walk,
			Die
		};
	}

	namespace HazardStates
	{
		enum RockState {
			Fall,
			Explode
		};
	}

}