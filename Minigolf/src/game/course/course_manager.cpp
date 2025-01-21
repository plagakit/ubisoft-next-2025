#include "course_manager.h"

#include "game/components/fake_3d.h"
#include "game/components/obstacle.h"
#include "game/course/tile.h"
#include <iostream>
#include <sstream>

CourseManager::CourseManager(EntityManager& registry, ResourceManager& resourceMgr) :
	m_registry(registry),
	m_resourceMgr(resourceMgr),
	m_curTemplate(INVALID_RID)
{
	m_registry.RegisterComponentType<Tile>(500);
	m_registry.RegisterComponentType<ObstacleLipOut>(500);

	//auto [wcRID, wc] = m_resourceMgr.LoadAndGet<AABB2DCollider>("wall_collider");
	//m_colliderWall = wcRID;
	//wc.width = m_gridSize;
	//wc.height = m_gridSize;

	// Load wall resources
	m_meshWall = m_resourceMgr.Load<Mesh>("res/models/wall.obj");
	m_meshMovingObstacle = m_resourceMgr.Load<Mesh>("res/models/wall.obj");

	// Load hole resoruces
	m_texHole = m_resourceMgr.Load<Texture>("res/sprites/hole.png");
	auto [hrid, colHole] = m_resourceMgr.LoadAndGet<AABB2DCollider>("hole_collider");
	m_colHole = hrid;
	colHole.width = 0.8f;
	colHole.height = 0.1f;

	auto [lrid, colLip] = m_resourceMgr.LoadAndGet<AABB2DCollider>("lip_collider");
	m_colLip = lrid;
	colLip.width = 2.8f;
	colLip.height = 1.5f;

	auto [morid, colMovOb] = m_resourceMgr.LoadAndGet<AABB2DCollider>("moving_obstacle_collider");
	m_colMovingObstacle = morid;
	colMovOb.width = m_gridSize;
	colMovOb.height = m_gridSize;
}

CourseManager::~CourseManager()
{
	ClearCourse();

	for (RID col : m_wallColliders)
		m_resourceMgr.Unload(col);

	m_resourceMgr.Unload(m_meshWall);
}

void CourseManager::LoadTemplate(const std::string& path)
{
	if (m_curTemplate != INVALID_RID)
		m_resourceMgr.Unload(m_curTemplate);

	m_curTemplate = m_resourceMgr.Load<CourseTemplate>(path.c_str());
}

void CourseManager::ClearCourse()
{
	for (Entity body : m_physicsBodies)
		m_registry.QueueDelete(body);
	m_physicsBodies.clear();

	for (Entity obj : m_courseEntities)
		m_registry.QueueDelete(obj);
	m_courseEntities.clear();
}

const std::vector<Entity>& CourseManager::GetPhysicsBodies() const
{
	return m_physicsBodies;
}

Vec2 CourseManager::GetGolfStartPos() const
{
	return m_golfStartPos;
}


void CourseManager::SetOrigin(const Vec2& origin)
{
	m_origin = origin;
}

void CourseManager::SetGridSize(float gridSize)
{
	m_gridSize = gridSize;
}

Vec2 CourseManager::GridPosToWorldPos(IVec2 gridPos)
{
	Vec2 pos = Vec2(
		static_cast<float>(gridPos.x),
		static_cast<float>(gridPos.y)
	);
	return m_origin + pos * m_gridSize;
}

void CourseManager::RenderGrid(Renderer& renderer, Camera& camera)
{
	Color GRID_COLOR = Color(0.1f, 0.4f, 0.0f);
	constexpr int GRIDLINE_COUNT = 40;
	constexpr float GRIDLINE_SPACING = 4.0f;
	Vec3 camPos = camera.GetTransform().position;
	float xOffset = camPos.x - fmodf(camPos.x, GRIDLINE_SPACING);
	float zOffset = camPos.z - fmodf(camPos.z, GRIDLINE_SPACING);

	for (int i = 0; i < GRIDLINE_COUNT; i++)
	{
		for (int j = 0; j < GRIDLINE_COUNT; j++)
		{
			float x = (i - GRIDLINE_COUNT / 2) * GRIDLINE_SPACING + xOffset + GRIDLINE_SPACING * 0.5f;
			float z = (j - GRIDLINE_COUNT / 2) * GRIDLINE_SPACING + zOffset + GRIDLINE_SPACING * 0.5f;

			renderer.Draw3DLine(Vec3(x, 0.01f, zOffset), Vec3(x, 0.01f, z), GRID_COLOR);
			renderer.Draw3DLine(Vec3(xOffset, 0.01f, z), Vec3(x, 0.01f, z), GRID_COLOR);
		}
	}
}

IVec2 CourseManager::FindFirstTilePos(const CourseTemplate::Part& part, Tile::Type tileType) const
{
	int height = static_cast<int>(part.m_tilemap.size());
	for (int i = height - 1; i >= 0; i--)
		for (int j = 0; j < part.m_tilemap[i].size(); j++)
			if (part.m_tilemap[i][j].type == tileType)
				return IVec2(j, height - i - 1);

	return IVec2(0, 0);
}

// COURSE GENERATION

void CourseManager::GenerateCourse(int numParts)
{
	ClearCourse();

	const CourseTemplate& temp = m_resourceMgr.Get<CourseTemplate>(m_curTemplate);
	const auto& parts = temp.GetParts();

	//InstantiatePart(parts[1], IVec2::ZERO);
	//return;

	IVec2 curGridPos = IVec2(0, 0);
	
	// Collect all the parts that are either a start or end
	std::vector<size_t> startParts;
	std::vector<size_t> endParts;
	std::vector<size_t> generalParts;
	for (size_t i = 0; i < parts.size(); i++)
	{
		if (parts[i].m_isStart)
			startParts.push_back(i);
		else if (parts[i].m_isHole)
			endParts.push_back(i);
		else
			generalParts.push_back(i);
	}

	//Logger::Debug("Starts: %d Ends: %d", startParts.size(), endParts.size());
	ASSERT_ERROR(startParts.size() > 0, "Trying to generate a course w/ no start parts!");
	ASSERT_ERROR(endParts.size() > 0, "Trying to generate a course w/ no end parts!");
	ASSERT_ERROR(generalParts.size() > 0, "Trying to generate a course w/ no in between general parts!");

	const auto& p = parts[2];
	InstantiatePart(p, curGridPos);
	return;

	// Randomly choose a start part to start
	int startIdx = Math::RandInt(0, static_cast<int>(startParts.size()) - 1);
	const auto& startPart = parts[startParts[startIdx]];
	InstantiatePart(startPart, curGridPos);
	curGridPos = FindFirstTilePos(startPart, Tile::Type::EXIT);

	IVec2 startPos = FindFirstTilePos(startPart, Tile::Type::START);
	m_golfStartPos = GridPosToWorldPos(startPos);
	//Logger::Debug("Start Grid Pos: %s, Start World Pos: %s", startPos.ToString().c_str(), m_golfStartPos.ToString().c_str());
	
	// Generate parts/new rooms equal to argument
	for (int i = 0; i < numParts; i++)
	{
		int randIdx = Math::RandInt(0, static_cast<int>(generalParts.size()) - 1);
		const auto& part = parts[generalParts[randIdx]];
		InstantiatePart(part, curGridPos);
		IVec2 enter = FindFirstTilePos(part, Tile::Type::ENTER);
		IVec2 exit = FindFirstTilePos(part, Tile::Type::EXIT);
		Logger::Debug("Exit: %s", exit.ToString().c_str());
		curGridPos += exit - enter;
	}

	// Generate a random end part
	int endIdx = Math::RandInt(0, static_cast<int>(endParts.size()) - 1);
	InstantiatePart(parts[endParts[endIdx]], curGridPos);
}

void CourseManager::InstantiatePart(const CourseTemplate::Part& part, IVec2 pos)
{
	Logger::Debug("Instantiating %s @ (%d, %d)", part.m_name.c_str(), pos.x, pos.y);

	// Offset the position by the part's enter position so that exit and enter line up
	if (!part.m_isStart)
	{
		IVec2 enterPos = FindFirstTilePos(part, Tile::Type::ENTER);
		enterPos.y--;
		pos -= enterPos;
	}

	// INSTANTIATE TILES

	// Temporary struct that isn't needed outside of instantiation
	struct WallMinMax
	{
		bool isValid = false;
		IVec2 min;
		IVec2 max;
	};
	WallMinMax walls[CourseTemplate::PART_MAX_WALLS];

	constexpr int WALL_COLOR_COUNT = 5;
	Color wallColors[WALL_COLOR_COUNT] = { Color::RED, Color::BLUE, Color::MAGENTA, Color::YELLOW, Color::CYAN };
	int curWallColor = 0;

	// Start from top and work down
	int height = static_cast<int>(part.m_tilemap.size());
	for (int i = height - 1; i >= 0; i--)
	{
		for (int j = 0; j < part.m_tilemap[i].size(); j++)
		{
			const Tile& tile = part.m_tilemap[i][j];
			IVec2 gridPos = pos + IVec2(j, height - i - 1);
			switch (tile.type)
			{
			case Tile::Type::WALL:
			{
				// Update the collider coords
				WallMinMax& wall = walls[tile.wallIndex];
				if (!wall.isValid)
				{
					wall.min = gridPos;
					wall.max = gridPos;
					wall.isValid = true;
				}
				else
				{
					wall.min = Math::Min(gridPos, wall.min);
					wall.max = Math::Max(gridPos, wall.max);
				}

				// Create a wall mesh
				InstantiateWall(gridPos, wallColors[curWallColor]);
				curWallColor = (curWallColor + 1) % WALL_COLOR_COUNT;
				break;
			}
			case Tile::Type::HOLE:
				InstantiateHole(gridPos);
				break;

			break;
			default:
				break;
			}
		}
	}

	// Create wall colliders
	for (int i = 0; i < CourseTemplate::PART_MAX_WALLS; i++)
	{
		auto& [isValid, min, max] = walls[i];
		if (isValid)
			InstantiateWallBody(min, max, i);
	}

	// Create any obstacles
	for (const auto& [obstName, args] : part.m_rawObstacles)
	{
		if (obstName == "moving")
		{
			//Logger::Info("Making moving obstacle, strings %s/%s", obstName.c_str(), args.c_str());
			std::istringstream stream(args);
			IVec2 from, to;
			stream >> from.x >> from.y >> to.x >> to.y;
			InstantiateMovingObstacle(to, from);
		}
	}
}

void CourseManager::InstantiateWall(IVec2 gridPos, const Color& color)
{
	Entity w = m_registry.CreateEntity();

	Transform2D tf2D;
	tf2D.position = GridPosToWorldPos(gridPos);

	Transform3D tf3D;
	tf3D.scale = Vec3::ONE * m_gridSize / 2.0f;

	MeshInstance m;
	m.meshHandle = m_meshWall;
	m.mode = ShadingMode::SHADED;
	m.color = color;

	m_registry.Add<Transform2D>(w, tf2D);
	m_registry.Add<Transform3D>(w, tf3D);
	m_registry.Add<Fake3D>(w, { 3.0f });
	m_registry.Add<MeshInstance>(w, m);

	m_courseEntities.push_back(w);
}

void CourseManager::InstantiateWallBody(IVec2 min, IVec2 max, int wallIdx)
{
	// Create the resource
	int width = max.x - min.x + 1;
	int height = max.y - min.y + 1;

	// The nice thing about this is that we have 2 identical colliders
	// ex. 2x4_wall_collider
	// the resource manager will reuse the collider.
	std::string colliderName =
		std::to_string(width) + "x" + std::to_string(height)
		+ "_wall_collider";

	auto [colRID, wallCol] = m_resourceMgr.LoadAndGet<AABB2DCollider>(colliderName.c_str());
	wallCol.width = width * m_gridSize;
	wallCol.height = height * m_gridSize;

	// If not reused, we have to keep track of it
	if (std::find(m_wallColliders.begin(), m_wallColliders.end(), colRID) == m_wallColliders.end())
		m_wallColliders.push_back(colRID);

	// Create the physics body
	Transform2D tf;
	tf.position = (GridPosToWorldPos(min) + GridPosToWorldPos(max)) / 2.0f;

	Physics2D ph;
	ph.colliderHandle = colRID;
	ph.isImmovable = true;

	Entity body = m_registry.CreateEntity();
	m_registry.Add<Transform2D>(body, tf);
	m_registry.Add<Physics2D>(body, ph);
	m_registry.Add<Tile>(body, { Tile::Type::WALL, wallIdx });
	m_physicsBodies.push_back(body);
}

void CourseManager::InstantiateHole(IVec2 pos)
{
	// Create the hole entity
	{
		Entity hole = m_registry.CreateEntity();
		Transform2D tf;
		tf.position = GridPosToWorldPos(pos);

		Transform3D tf3;
		tf3.scale = Vec3::ONE * 0.75f;

		Physics2D ph;
		ph.colliderHandle = m_colHole;
		ph.isTrigger = true;

		m_registry.Add<Transform2D>(hole, tf);
		m_registry.Add<Transform3D>(hole, tf3);
		m_registry.Add<Fake3D>(hole, { 0.1f });
		m_registry.Add<Physics2D>(hole, ph);
		m_registry.Add<Sprite>(hole, { m_texHole });
		m_registry.Add<Tile>(hole, { Tile::Type::HOLE });

		m_courseEntities.push_back(hole);
		m_physicsBodies.push_back(hole);
	}

	// Create the lip of the hole
	{
		Entity lip = m_registry.CreateEntity();

		Transform2D tf;
		tf.position = GridPosToWorldPos(pos);

		Physics2D ph;
		ph.colliderHandle = m_colLip;
		ph.isTrigger = true;

		m_registry.Add<Transform2D>(lip, tf);
		m_registry.Add<Physics2D>(lip, ph);
		m_registry.Add<ObstacleLipOut>(lip, { });

		m_courseEntities.push_back(lip);
		m_physicsBodies.push_back(lip);
	}

}

void CourseManager::InstantiateMovingObstacle(IVec2 to, IVec2 from)
{
	Entity o = m_registry.CreateEntity();

	ObstacleMoving om;
	om.src = GridPosToWorldPos(from);
	om.dest = GridPosToWorldPos(to);
	om.toDest = true;

	Transform2D tf2D;
	tf2D.position = GridPosToWorldPos(to);
	tf2D.velocity = (om.dest - om.src).Normalized() * om.speed;

	Physics2D ph;
	ph.colliderHandle = m_colMovingObstacle;
	ph.isImmovable = true;

	Transform3D tf3D;
	tf3D.scale = Vec3::ONE * m_gridSize / 2.0f;

	MeshInstance m;
	m.meshHandle = m_meshMovingObstacle;
	m.mode = ShadingMode::SHADED;
	m.color = Color::WHITE;

	m_registry.Add<Transform2D>(o, tf2D);
	m_registry.Add<Transform3D>(o, tf3D);
	m_registry.Add<Physics2D>(o, ph);
	m_registry.Add<Fake3D>(o, { 3.2f });
	m_registry.Add<MeshInstance>(o, m);
	m_registry.Add<ObstacleMoving>(o, om);
	m_registry.Add<Tile>(o, { Tile::Type::WALL });

	m_physicsBodies.push_back(o);
	m_courseEntities.push_back(o);
}
