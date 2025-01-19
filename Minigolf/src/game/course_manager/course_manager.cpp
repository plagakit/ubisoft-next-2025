#include "course_manager.h"

#include "game/components/fake_3d.h"
#include "game/components/obstacle.h"
#include "game/course_manager/course_file.h"

CourseManager::CourseManager(EntityManager& registry, ResourceManager& resourceMgr) :
	m_registry(registry),
	m_resourceMgr(resourceMgr),
	m_courseCurrent(INVALID_RID)
{
	m_registry.RegisterComponentType<Obstacle>(500);

	auto [wcRID, wc] = m_resourceMgr.LoadAndGet<AABB2DCollider>("wall_collider");
	m_colliderWall = wcRID;
	wc.width = 4.0f;
	wc.height = 4.0f;

	m_meshWall = m_resourceMgr.Load<Mesh>("res/models/wall.obj");
}

CourseManager::~CourseManager()
{
	m_resourceMgr.Unload(m_colliderWall);
	m_resourceMgr.Unload(m_meshWall);
}

void CourseManager::LoadCourse(const std::string& path)
{
	if (m_courseCurrent != INVALID_RID)
		m_resourceMgr.Unload(m_courseCurrent);

	m_courseCurrent = m_resourceMgr.Load<Course>(path.c_str());
}

void CourseManager::Update(float dt)
{
	
}

void CourseManager::CreateWall(int row, int col)
{
	Entity w = m_registry.CreateEntity();

	Transform2D tf2D;
	tf2D.position = GridToPos(row, col);

	Transform3D tf3D;
	tf3D.scale = Vec3(2.0f, 2.0f, 2.0f);

	Physics2D ph;
	ph.colliderHandle = m_colliderWall;
	ph.isImmovable = true;
	
	MeshInstance m;
	m.meshHandle = m_meshWall;
	m.mode = ShadingMode::SHADED;
	m.color = Color::MAGENTA;

	m_registry.Add<Transform2D>(w, tf2D);
	m_registry.Add<Transform3D>(w, tf3D);
	m_registry.Add<Fake3D>(w, { 3.0f });
	m_registry.Add<Physics2D>(w, ph);
	m_registry.Add<MeshInstance>(w, m);
	m_registry.Add<Obstacle>(w, { Obstacle::Type::WALL });

	m_walls.push_back(w);
}

const std::vector<Entity>& CourseManager::GetWalls() const
{
	return m_walls;
}

Vec2 CourseManager::GridToPos(int row, int col)
{
	return Vec2();
}
