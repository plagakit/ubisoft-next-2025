#pragma once

#include <engine.h>
#include "game/course/course_template.h"

/**
Manages the loading and generation of a course. This class
holds a CourseTemplate and uses that (along with a procedural)
generation algorithm to generate a course and all of its entities
when asked to. It doesn't handle the behaviour of how the course
interacts with the player, it only generates then keeps track of 
every course object.

Also this finally finds a use for IVec2's which I am really happy
for because I made Vector2 a generic template very early into the
project and I thought "I'm gonna need that in the future........" :D
*/
class CourseManager
{
public:
	CourseManager(EntityManager& registry, ResourceManager& resourceMgr);
	~CourseManager();

	void LoadTemplate(const std::string& path);

	void InstantiatePart(const CourseTemplate::Part& part, IVec2 entrance);
	void GenerateCourse(int parts);
	void ClearCourse();

	void InstantiateWall(IVec2 pos, const Color& color);
	void InstantiateWallBody(IVec2 min, IVec2 max, int wallIdx);

	void InstantiateHole(IVec2 pos);

	const std::vector<Entity>& GetPhysicsBodies() const;
	Vec2 GetGolfStartPos() const;

	void SetOrigin(const Vec2& origin);
	void SetGridSize(float gridSize);

	Vec2 GridPosToWorldPos(IVec2 gridPos);
	void RenderGrid(Renderer& renderer, Camera& camera);

private:
	EntityManager& m_registry;
	ResourceManager& m_resourceMgr;
	
	RID m_meshWall;
	RID m_texHole, m_colHole;
	RID m_colLip; // see obstacle.h

	Vec2 m_origin;
	float m_gridSize = 4.0f;

	RID m_curTemplate;
	Vec2 m_golfStartPos;
	
	std::vector<Entity> m_physicsBodies;
	std::vector<RID> m_wallColliders;	// the generated collider resources
	//std::vector<Entity> m_wallBodies;	// grouped walls that serve as physics bodies
	std::vector<Entity> m_courseEntities; // each individual wall

	IVec2 FindFirstTilePos(const CourseTemplate::Part& part, Tile::Type tileType) const;

};