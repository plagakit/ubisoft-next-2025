#include "ecs_test.h"

#include <engine.h>

void ECSTest::RunTests()
{
	Logger::Info("Running ECS tests...");

	EntityManager registry;
	constexpr size_t amt = 100000;

	registry.ReserveEntities(150000);
	registry.RegisterComponentType<Transform3D>(150000);
	registry.RegisterComponentType<TestComponent>(150000);

	Entity e1 = registry.CreateEntity();
	Entity e2 = registry.CreateEntity();

	Transform3D c1 = { Vec3(1, 2, 3) };
	TestComponent c2 = { 23 };

	ASSERT_ERROR(registry.Exists(e1), "%d does not exist", e1);
	ASSERT_ERROR(registry.Exists(e2), "%d does not exist", e2);
	ASSERT_ERROR(registry.Count() == 2, "Wrong count: %d", registry.Count());

	registry.QueueDelete(e1);
	registry.QueueDelete(e2);
	registry.FlushDeleteQueue();

	for (int i = 0; i < amt; i++)
	{
		Entity e = registry.CreateEntity();
		TestComponent a = { e * 2 };
		registry.Add<TestComponent>(e, a);
	}

	for (int i = 300; i < 700; i++)
		registry.QueueDelete(i);
	registry.FlushDeleteQueue();

	ASSERT_ERROR(registry.Count() == (amt - 400), "Wrong count: %d, expected %d", registry.Count(), amt - 400);
	ASSERT_ERROR(registry.Has<TestComponent>(299), "Entity 299 doesn't have TestComponent");
	ASSERT_ERROR(!registry.Has<TestComponent>(300), "Entity 300 has TestComponent.");

	for (int i = 0; i < 700 - 300; i++)
	{
		Entity id = registry.CreateEntity();
		TestComponent a = { id * 100 };
		registry.Add<TestComponent>(id, a);
	}

	ASSERT_ERROR(registry.Count() == amt, "Wrong count: %d, expected %d", registry.Count(), amt);

	for (auto [id, tc, tf] : registry.AllWith<TestComponent, Transform3D>())
		Logger::Info("%d %d", id, tc.a);

	int count = 0;
	for (auto [id, tc] : registry.AllWith<TestComponent>())
		count++;

	ASSERT_ERROR(count == amt, "EntityView AllWith wrong count: %d, expected %d", count, amt);

	Logger::Info("ECS tests passed!");
}
