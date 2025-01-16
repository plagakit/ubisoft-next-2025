#pragma once

#include "core/resource/resource.h"
#include "graphics/renderer/renderer.h"
#include "physics/collision/collision_result.h"
#include "physics/collision/collision_data.h"
#include "physics/collision/collision_dispatcher.h"

struct Collider2D : public Resource
{
	void Load(const char* path) override {};
	void Unload() override {};
	
	virtual CollisionResult2D Collide(const Collider2D&, const CollisionData2D&) const = 0;
	virtual CollisionResult2D Collide(const CollisionDispatcher2DBase&, const CollisionData2D&) const = 0;
	virtual void DebugDraw(Renderer& renderer, Vec2 position) = 0;
};

#define DEFINE_COLLIDER2D_VISITOR() \
	CollisionResult2D Collide(const Collider2D&, const CollisionData2D&) const override; \
	CollisionResult2D Collide(const CollisionDispatcher2DBase&, const CollisionData2D&) const override;

#define IMPL_COLLIDER2D_VISITOR(type) \
	CollisionResult2D type::Collide(const Collider2D& other, const CollisionData2D& data) const \
	{ \
		auto dispatch = CollisionDispatcher2D<CircleCollider>(*this); \
		return other.Collide(dispatch, data); \
	} \
	\
	CollisionResult2D type::Collide(const CollisionDispatcher2DBase& dispatch, const CollisionData2D& data) const \
	{ \
		return dispatch.Collide(*this, data); \
	}