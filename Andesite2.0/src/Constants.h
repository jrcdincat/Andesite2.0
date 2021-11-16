#pragma once

namespace constants {
	constexpr int SCREEN_WIDTH = 1080;
	constexpr int SCREEN_HEIGHT = 720;
	constexpr int TARGET_FPS = 60;
	constexpr float TARGET_DELTATIME = 1.5f;
	constexpr float PIXEL_PER_METER = 32.0f;

	// Physics
	constexpr float PHYSICS_WORLD_GRAVITY = 3.8f;
	constexpr int VELOCITY_ITERATIONS = 8;
	constexpr int POSITION_ITERATIONS = 3;

	// Player constants
	constexpr float PLAYER_X_START_POS = 5.0f;
	constexpr float PLAYER_Y_START_POS = 21.5f;
	constexpr float PLAYER_X_OFFSET_ANIMATION = 150.0f;
	constexpr float PLAYER_Y_OFFSET_ANIMATION = 105.0f;
	constexpr float X_AXIS_WIN_LOCATION = 214.0f;
	constexpr float X_AXIS_END_OF_LEVEL_LOCATION = 227.0f;
	constexpr float LAVA_Y_AXIS = 30.0f;
	constexpr float PLAYER_SPEED = 1.0f;

	// Golem constants
	constexpr float GOLEM_X_OFFSET_ANIMATION = 40.0f;
	constexpr float GOLEM_Y_OFFSET_ANIMATION = 5.0f;
	constexpr float GOLEM_DEFAULT_SPEED = 0.3f;
	constexpr float GOLEM_CHARGE_SPEED = 0.6f;
	constexpr float GOLEM_DETECT_RANGE = 10.0f;

	// Rock constants
	constexpr float ROCK1_X_OFFSET_ANIMATION = 10.0f;
	constexpr float ROCK1_Y_OFFSET_ANIMATION = 5.0f;
	constexpr float EXPLOSION1_X_OFFSET_ANIMATION = 70.0f;
	constexpr float EXPLOSION1_Y_OFFSET_ANIMATION = 60.0f;

	// Bat constants
	constexpr float BAT_X_OFFSET_ANIMATION = 60.0f;
	constexpr float BAT_Y_OFFSET_ANIMATION = 18.0f;
	constexpr float BAT_DEFAULT_SPEED = 0.5f;
	constexpr float BAT_CHARGE_SPEED = 0.8f;
	constexpr float BAT_DETECT_RANGE = 12.0f;


	enum UserDataTypes {
		USER_TYPE_PLAYER_FEET,
		USER_TYPE_BOUDNARY,
		USER_TYPE_PLAYER,
		USER_TYPE_GOLEM,
		USER_TYPE_BAT,
		USER_TYPE_ROCK 
	};

	enum CategoryBits {
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
		IDLE,
		RUN,
		JUMP,
		FALL,
		DIE,
		WIN
	};

	namespace EnemyStates
	{
		enum EnemyState {
			IDLE,
			MOVE,
			DIE
		};
	}

	namespace HazardStates
	{
		enum RockState {
			FALL,
			EXPLODE
		};
	}
}
