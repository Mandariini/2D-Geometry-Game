#pragma once

#include "Entity.h"
#include "Common.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

// Entity factory

class EntityManager
{
public:
	EntityManager();

	void update();

	std::shared_ptr<Entity> addEntity(const std::string& tag);

	EntityVec& getEntities();
	EntityVec& getEntities(const std::string& tag);

private:
	// Iterate all entities that are not active (= destroyed)
	void removeDeadEntities(EntityVec& vec);

	EntityVec m_entities;
	EntityVec m_entitiesToAdd;
	EntityMap m_entityMap;
	size_t m_totalEntities = 0;
};