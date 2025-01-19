#pragma once

#include <engine.h>

class CourseManager
{
public:
	CourseManager(EntityManager& registry, ResourceManager& resourceMgr);
	~CourseManager();

	void LoadCourse(const std::string& path);

	void Update(float dt);

	void CreateWall(int row, int col);

	const std::vector<Entity>& GetWalls() const;

private:
	EntityManager& m_registry;
	ResourceManager& m_resourceMgr;

	RID m_courseCurrent;
	RID m_colliderWall;
	RID m_meshWall;

	std::vector<Entity> m_walls;

	Vec2 GridToPos(int row, int col);

};