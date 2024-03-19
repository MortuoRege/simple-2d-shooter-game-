#include "EntityManager.h"

EntityManager::EntityManager()
{

}

void EntityManager::update()
{
	for (auto& e : m_entetiesToAdd)
	{
		m_entities.push_back(e);
		m_entityMap[e->tag()].push_back(e); 
		//this line pushe the entity into the map with the tag as the key
	}

	m_entetiesToAdd.clear();
	removeDeadEntities(m_entities);

	for (auto& element : m_entityMap)
	{
		auto& tag = element.first;
		auto& entityVector = element.second;

		removeDeadEntities(entityVector);
	}
}

void EntityManager::removeDeadEntities(EntityVector& vec)
{
	vec.erase(std::remove_if(vec.begin(), vec.end(), [](const std::shared_ptr<Entity>& e) {
		return !e->isActive();
		}), vec.end());

	//this function is used to remove all the elements from the vector that are not active using 
	//the combination of .erase and std::remove_if functions remove_if is used to remove the elements
	//so that the elements that satisfy the removal condition are moved to the end of the vector
	//in this case the elements that are not active are moved to the end of the vector
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{

	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntites++, tag));
	
	m_entetiesToAdd.push_back(entity);

	return entity;
	//this function is used to make a new entity and add it to the vector of entities
}

const EntityVector& EntityManager::getEntities()
{
	return m_entities;
}

const EntityVector& EntityManager::getEntities(const std::string& tag)
{
	return  m_entityMap[tag];
}