#pragma once

#include "core/uid/uid.h"
#include "graphics/renderer/renderer.h"
#include "gui/dim2.h"

#include <vector>
#include <memory>

class GUIObject
{
public:
	GUIObject();

	GUIObject* GetParent();
	void AddChild(std::unique_ptr<GUIObject> child);
	void RemoveChild(UID child);
	GUIObject& GetChild(UID child);
	std::vector<std::unique_ptr<GUIObject>>& GetChildren();

	UID GetUID() const;

	Dim2& GetPosition();
	Dim2& GetSize();
	Vec2 GetAbsolutePosition() const;
	Vec2 GetAbsoluteSize() const;

	Vec2 m_anchor = Vec2::ZERO;
	Dim2 m_position = Dim2(0, 0, 0, 0);
	Dim2 m_size = Dim2(1, 1, 0, 0);

	//Dim2 GetPosition() const;
	//Dim2 GetSize() const;
	//void SetPosition(const Dim2& position);
	//void SetSize(const Dim2& size);

protected:
	static UIDGenerator m_uidGenerator;
	UID m_uid;
	GUIObject* m_parent;
	std::vector<std::unique_ptr<GUIObject>> m_children;



	virtual void UpdateInternal(float dt) = 0;
	virtual void RenderInternal(Renderer& renderer, Vec2 absPos, Vec2 absSize) = 0;

private:
	friend class GUIRoot;

	Vec2 m_origin; // Absolute position
	Vec2 m_scale; // Absolute size

	void Update(float dt);
	void Render(Renderer& renderer, Vec2 origin, Vec2 scale);

};