#include "pch.h"
#include "gui_object.h"

#include "core/debug/assert.h"

UIDGenerator GUIObject::m_uidGenerator{};

GUIObject::GUIObject() :
	m_parent(nullptr)
{
	m_uid = m_uidGenerator.Next();
}

void GUIObject::Update(float dt)
{
	UpdateInternal(dt);
	for (auto& child : m_children)
		child->Update(dt);
}

void GUIObject::Render(Renderer& renderer, Vec2 parentOrigin, Vec2 parentScale)
{
	m_origin = parentOrigin + parentScale * m_position.relative + m_position.offset;
	m_scale = parentScale * m_size.relative + m_size.offset;
	m_origin -= m_anchor * m_scale;

	RenderInternal(renderer, m_origin, m_scale);
	for (auto& child : m_children)
		child->Render(renderer, m_origin, m_scale);
}

std::vector<std::unique_ptr<GUIObject>>& GUIObject::GetChildren()
{
	return m_children;
}

GUIObject* GUIObject::GetParent()
{
	return m_parent;
}

void GUIObject::AddChild(std::unique_ptr<GUIObject> child)
{
	ASSERT_ERROR(child, "Trying to add null GUI child!");

	child->m_parent = this;
	m_children.push_back(std::move(child));
}

void GUIObject::RemoveChild(UID child)
{
	auto it = std::find_if(m_children.begin(), m_children.end(),
		[child](const auto& ptr) 
		{
			return ptr->GetUID() == child;
		}
	);

	if (it != m_children.end()) 
		m_children.erase(it);
}

GUIObject& GUIObject::GetChild(UID child)
{
	auto it = std::find_if(m_children.begin(), m_children.end(),
		[child](const auto& ptr)
		{
			return ptr->GetUID() == child;
		}
	);

	ASSERT_ERROR(it != m_children.end(), "Can't find GUI child %d!", child);
	return *it->get();
}

UID GUIObject::GetUID() const
{
	return m_uid;
}

Dim2& GUIObject::GetPosition()
{
	return m_position;
}

Dim2& GUIObject::GetSize()
{
	return m_size;
}

// GETTERS/SETTERS

Vec2 GUIObject::GetAbsolutePosition() const
{
	return m_origin;
}

Vec2 GUIObject::GetAbsoluteSize() const
{
	return m_scale;
}

//Dim2 GUIObject::GetPosition() const
//{
//	return m_position;
//}
//
//Dim2 GUIObject::GetSize() const
//{
//	return m_size;
//}
//
//void GUIObject::SetPosition(const Dim2& position)
//{
//	m_position = position;
//}
//
//void GUIObject::SetSize(const Dim2& size)
//{
//	m_size = size;
//}

