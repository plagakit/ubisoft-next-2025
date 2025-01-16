#pragma once

#include "components/2d/transform_2d.h"
#include "components/3d/transform_3d.h"

struct CollisionData2D
{
	const Transform2D& tf1;
	const Transform2D& tf2;
};

struct CollisionData3D
{
	const Transform3D& tf1;
	const Transform3D& tf2;
};