#pragma once

struct Timer
{
	float elapsedTime	= 0.0f;
	float duration		= 1.0f;
	bool isRunning		= false;
	bool isDone			= false;
	bool firesOnce		= true;
	
	float Progress() const
	{
		return elapsedTime / duration;
	}
};