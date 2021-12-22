#include "Entity.h"

Entity::Entity(const std::string& tag, size_t id) 
	: m_tag(tag), m_id(id)
{

}

const std::string& Entity::getTag()
{
	return m_tag;
}

const size_t Entity::getId()
{
	return m_id;
}

bool Entity::isActive()
{
	return m_active;
}

void Entity::destroy()
{
	m_active = false;
}
