#pragma once

#include "Entity.h"
#include <vector>
#include <map>

typedef std::vector<std::shared_ptr<Entity>> EntityVector;
typedef std::map<std::string, EntityVector>  EntityMap;


class EntityManager
{
	EntityVector m_entities;
	EntityVector m_entetiesToAdd;
	EntityMap	 m_entityMap;
	size_t		 m_totalEntites = 0;

	void removeDeadEntities(EntityVector& vec);

public:
	EntityManager();

	void update();

	std::shared_ptr<Entity> addEntity(const std::string& tag);

	const EntityVector& getEntities();
	const EntityVector& getEntities(const std::string& tag);
	
};