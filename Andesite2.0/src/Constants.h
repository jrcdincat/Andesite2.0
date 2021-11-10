#pragma once

namespace constants {
	constexpr int SCREEN_WIDTH = 1080;
	constexpr int SCREEN_HEIGHT = 720;
	constexpr float JUMP_FORCE = 20.0f;
	constexpr float JUMP_TIME = 15.0f;
	constexpr float PIXEL_PER_METER = 32.0f;
	constexpr int GOLEM_X_OFFSET_ANIMATION = 40.0f;
	constexpr int GOLEM_Y_OFFSET_ANIMATION = 5.0f;
	constexpr int PLAYER_X_OFFSET_ANIMATION = 150.0f;
	constexpr int PLAYER_Y_OFFSET_ANIMATION = 105.0f;

	enum userDataTypes {
		USER_TYPE_PLAYER_FEET = 1,
		USER_TYPE_BOUDNARY = 2,
		USER_TYPE_PLAYER = 3,
		USER_TYPE_ENEMY = 4
	};

	enum categoryBits {
		NOTHING = 0x0000,
		BOUNDARY = 0x0001,
		PLAYER = 0x0002,
		ENEMY = 0x0004,
		ENEMY_HEAD = 0x0008,
		GEM = 0x0010,
		PLAYER_FEET = 0x0016
	};

	enum PlayerState {
		Idle,
		Run,
		Jump,
		Fall,
		Die
	};
}