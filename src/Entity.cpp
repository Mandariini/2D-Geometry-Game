#include "Entity.h"

Entity::Entity(const std::string& tag, const std::string& type, size_t id)
	: m_tag(tag), m_type(type), m_id(id)
{

}

const std::string& Entity::getTag()
{
	return m_tag;
}

const std::string& Entity::getType()
{
	return m_type;
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
