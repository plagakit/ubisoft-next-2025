#pragma once

struct Golfball
{
	enum State
	{
		IN_PLAY,
		SINKING
	};

	State state = State::IN_PLAY;
};