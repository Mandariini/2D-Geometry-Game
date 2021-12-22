#include "EntityManager.h"

EntityManager::EntityManager()
{

}

void EntityManager::update()
{
	// Add entities from m_entitiesToAdd the proper location(s)

	for (auto e : m_entitiesToAdd)
	{
		m_entities.push_back(e);
		m_entityMap[e->getTag()].push_back(e);
	}

	// Remove from all entities
	removeDeadEntities(m_entities);

	// Remove dead entities from each vector in the entity map
	// C++17
	for (auto& [tag, entityVec] : m_entityMap)
	{
		removeDeadEntities(entityVec);
	}

	//m_entitiesToAdd.clear();
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto e = std::shared_ptr<Entity>(new Entity(tag, m_totalEntities++));
	m_entitiesToAdd.push_back(e);

	return e;
}

EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

EntityVec& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	// Called by update
	// Remove entities set to m_alive false

	// Avoid iterator invalidation
	// G: how to remove from vector while iterating over it

	for (auto e : vec)
	{
		// if e is dead remove from m_entities and map
		// ITERATOR INVALIDATION
	}
}
