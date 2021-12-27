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
	m_entitiesToAdd.clear();

	// Remove from all entities
	removeDeadEntities(m_entities);

	// Remove dead entities from each vector in the entity map
	// C++17
	for (auto& [tag, entityVec] : m_entityMap)
	{
		removeDeadEntities(entityVec);
	}
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

	EntityVec::iterator it;

	for (it = vec.begin(); it != vec.end(); )
	{
		if (!(*it)->isActive())
		{
			it = vec.erase(it);
		}
		else
		{
			it++;
		}
	}

}
