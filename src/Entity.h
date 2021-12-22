#pragma once

#include "Common.h"
#include "Components.h"

class Entity
{
public:
	// Only EntityManager can construct entities
	friend class EntityManager;

	const std::string& getTag();
	const size_t getId();
	bool isActive();

	// Flags entity to be destroyed at the start of next frame
	void destroy();

	// Component pointers
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CCollision> cCollision;
	std::shared_ptr<CInput> cInput;
	std::shared_ptr<CScore> cScore;
	std::shared_ptr<CLifespan> cLifespan;

private:
	// Private constructor to only allow entity creation in EntityManager
	Entity(const std::string& tag, size_t id);

	const size_t m_id = 0;
	const std::string m_tag = "Default";
	bool m_active = true;
};