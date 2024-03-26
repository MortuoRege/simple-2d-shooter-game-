#pragma once
#include "Components.h"
#include <memory>
#include <String>

class Entity
{
	friend class EntityManager;

	bool			m_active	= true;
	size_t			m_id		= 0;
	std::string		m_tag		= "default"; 
	// this is the default tag for the entity later is will be changed to the tag that is passed in the constructor
	//for exaple if the tag is "player" then the tag will be changed to "player"

	Entity(const size_t id, const std::string& tag);

  public:
	  //there are the components for the entity
	  std::shared_ptr<CTransform> cTransform;
	  std::shared_ptr<CShape>	  cShape;
	  std::shared_ptr<CCollision> cCollision;
	  std::shared_ptr<CInput>	  cInput;
	  std::shared_ptr<CScore>	  cSore;
	  std::shared_ptr<CLifeSpan>  cLifespan;

	  bool isActive() const;
	  const std::string& tag() const;
	  const size_t id() const;
	  void destroy();
	  
};