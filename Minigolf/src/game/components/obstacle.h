#pragma once

struct ObstacleLipOut
{
	float centripetalMultiplier = 1.8f;
};

struct ObstacleMoving
{
	bool toDest = true;
	Vec2 src	= Vec2(0.0f, 0.0f);
	Vec2 dest	= Vec2(10.0f, 0.0f);
	float speed = 10.0f;
};

//struct Obstacle
//{
//	enum Type
//	{
//		LIP_OUT //https://www.youtube.com/watch?v=Ni5MQmQ-0Rk
//	};
//
//	Type obstacle;
//};3