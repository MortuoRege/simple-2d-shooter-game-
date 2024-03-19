#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include <SFML/Graphics.hpp>


class Game
{
	sf::RenderWindow m_window;
	EntityManager m_entities;
	sf::Font m_font;
	sf::Text m_text;
	int m_score = 0;
	int m_currentFrame = 0;
	int m_lastEnemyspawnTime = 0;
	int m_lastSpecialWeaponSpawnTime = 0;
	bool m_paused = false;
	bool m_running = true;
	bool m_activeWeapon = false;

	std::shared_ptr<Entity> m_player;

	//there are the systems that will be used in the game logic 
	void init();
	void setPaused(bool paused);

	void sMovement();
	void sUserInput();
	void sLifeSpan();
	void sRender();
	void sEnemySpawner();
	void sCollision();

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);
	void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mouseposition);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

public:
	Game();
	void run();

};

