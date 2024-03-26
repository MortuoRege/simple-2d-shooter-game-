#include "Game.h"

#include<iostream>
#include<cmath>

Game::Game()
{
	init();
	
}

void Game::init()
{
	if (!m_window.isOpen()) {
		std::cerr << "Failed to create window!" << std::endl;
		// Add any necessary cleanup or error handling here
	}
	m_window.create(sf::VideoMode(1280, 720), "Game");
	m_window.setFramerateLimit(60);

	spawnPlayer();
}

void Game::run()
{
	while (m_running)
	{
		// inside the main loop, between window.clear() and window.display()
		if (!m_paused) // Only update game entities and logic if the game is not paused
		{
			m_entities.update();
			sCollision();
			sEnemySpawner();
			sMovement();
			m_currentFrame++;
		}

		sLifeSpan();
		sUserInput();
		sRender();

	}
}

void Game::setPaused(bool paused)
{
	m_paused = paused;
}

void Game::spawnPlayer()
{
	auto entity = m_entities.addEntity("player");
	//spawn player at the center of the screen
	float centerX = m_window.getSize().x / 2.0f;
	float centerY = m_window.getSize().y / 2.0f;

	entity->cTransform = std::make_shared<CTransform>(Vec2(centerX, centerY), Vec2(0, 0), 0.0f);

	entity->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(225, 0, 0), 4.0f);
	
	//for now the only player has input component
	entity -> cInput = std::make_shared<CInput>();

	entity->cCollision = std::make_shared<CCollision>(32.0f);

	m_player = entity;
}

void Game::spawnEnemy()
{
	auto entity = m_entities.addEntity("enemy");

	float ex = (std::rand() % (m_window.getSize().x - 30)); // Add/subtract radius to ensure it's within screen
	float ey = (std::rand() % (m_window.getSize().y - 30)); // Add/subtract radius to ensure it's within screen
	int randomNumber = std::rand() % 6 + 3;//random number to determine the polign number of the enemy
	int numComponentsToSet = std::rand() % 3 + 1; //random number to determine the color of the enemy
	int plusOrMinus = ((std::rand() % 2 == 0) ? -1 : 1) * 4; // given a -3 or 3 randomly excluding 0

	sf::Color randomColor;

	//this swith statmeent is used to randomly set the color of the enemy
	switch (numComponentsToSet) {
	case 1:
		randomColor.r = 255;
		break;
	case 2:
		randomColor.g = 255;
		break;
	case 3:
		randomColor.b = 255;
		break;
	}


	entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(plusOrMinus, plusOrMinus), 0.0f);

	entity->cShape = std::make_shared<CShape>(30.0f, randomNumber, randomColor, sf::Color(225, 225, 225), 1.5);

	entity->cInput = std::make_shared<CInput>();

	entity->cCollision = std::make_shared<CCollision>(30.0f);

	entity->cSore = std::make_shared<CScore>(100);

	m_lastEnemyspawnTime = m_currentFrame;//set the last enemy spawn time to the current frame
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	int degree = 360 / entity->cShape->circle.getPointCount();//calculate the angle between the small enemies
	double angleinRadians = degree * (3.14/ 180);//convert the angle to radians
	int numberOfSmallEnemies = entity->cShape->circle.getPointCount();//this is the number of smallEnemies to spawn

	for (int i = 0; i < numberOfSmallEnemies; i++)
	{
		auto smallEnemies = m_entities.addEntity("smallEnemy");

		int ex = entity->cTransform->pos.x + entity->cCollision->radius * std::cos(angleinRadians * i);//calculate the change in x at an angle
		int ey = entity->cTransform->pos.y + entity->cCollision->radius * std::sin(angleinRadians * i);//calculate the change in y at an angle

		Vec2 velocity = Vec2(std::cos(angleinRadians * i), std::sin(angleinRadians * i)); // make the velocity  vector with the change in x and y

		smallEnemies->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), velocity, 0.0f);
		smallEnemies->cShape = std::make_shared<CShape>(entity->cCollision->radius-10, entity->cShape->circle.getPointCount(), entity->cShape->circle.getFillColor(), entity->cShape->circle.getOutlineColor(), 1.5);
		smallEnemies->cLifespan = std::make_shared<CLifeSpan>(225);
	}
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mouseposition)
{

	auto bullet = m_entities.addEntity("bullet");

	bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, Vec2(0, 0), 0);
	bullet->cShape = std::make_shared<CShape>(10, 8, sf::Color(255, 255, 255), sf::Color(0, 0, 0), 0);
	bullet->cLifespan = std::make_shared<CLifeSpan>(225);
	bullet->cCollision = std::make_shared<CCollision>(10);
	Vec2 distance = mouseposition - entity->cTransform->pos;
	distance.normalize();
	bullet->cTransform->velocity += distance * 10.0f;//set the velocity of the bullet to the direction of the mouse

}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{

	// I like to call this scpecial weapon the black hole weapon
	//basicallly it will pull all the enemies towards it and player will be able to shoot them easily
	
	//player will only be able to shoot the special weapon after 1000 frames
	if (m_currentFrame - m_lastSpecialWeaponSpawnTime < 1000)
	{
		return;
	}

	auto specialWeapon = m_entities.addEntity("specialWeapon");

	specialWeapon->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, Vec2(0, 0), 0);

	specialWeapon->cShape = std::make_shared<CShape>(40, 15, sf::Color(0, 0, 0), sf::Color(255, 255, 255), 5);
	specialWeapon->cLifespan = std::make_shared<CLifeSpan>(225);
	specialWeapon->cCollision = std::make_shared<CCollision>(40);

	float centerX = m_window.getSize().x / 2.0f;
	float centerY = m_window.getSize().y / 2.0f;

	Vec2 centerPos(centerX, centerY);
	Vec2 dist = centerPos - entity->cTransform->pos; // Vector pointing from entity to center

	dist.normalize();
	specialWeapon->cTransform->velocity = dist * 10.0f;

	m_lastSpecialWeaponSpawnTime = m_currentFrame;
	
}

void Game::sMovement()
{
	//implement all the entity movement here
	//red the m_player->cInput

	//*note do it for every entety


	//movement for player based on the keyboard input(w,a,s,d)
	for (auto& e : m_entities.getEntities("player"))
	{
		e->cTransform->velocity = Vec2(0, 0);
		if (e->cInput->up)
		{
			e->cTransform->velocity.y = -5;
			e->cTransform->pos.y += e->cTransform->velocity.y;
		}
		else if (e->cInput->down)
		{
			e->cTransform->velocity.y = 5;
			e->cTransform->pos.y += e->cTransform->velocity.y;
		}
		else if (e->cInput->right)
		{
			e->cTransform->velocity.x = 5;
			e->cTransform->pos.x += e->cTransform->velocity.x;
		}
		else if (e->cInput->left)
		{
			e->cTransform->velocity.x = -5;
			e->cTransform->pos.x += e->cTransform->velocity.x;
		}
	}


	for(auto & e : m_entities.getEntities("bullet"))
	{

		e->cTransform->pos.x += e->cTransform->velocity.x;
		e->cTransform->pos.y += e->cTransform->velocity.y;
 
	}

	for (auto& e : m_entities.getEntities("smallEnemy"))
	{
		e->cTransform->pos.x += e->cTransform->velocity.x;
		e->cTransform->pos.y += e->cTransform->velocity.y;
	}

	for (auto& e : m_entities.getEntities("specialWeapon"))
	{
		e->cTransform->pos.x += e->cTransform->velocity.x;
		e->cTransform->pos.y += e->cTransform->velocity.y;

		float centerX = m_window.getSize().x / 2.0f;
		float centerY = m_window.getSize().y / 2.0f;

		float distanceToCenter = std::sqrt(std::pow(e->cTransform->pos.x - centerX, 2) + std::pow(e->cTransform->pos.y - centerY, 2));

		if (distanceToCenter < 7.0f) // If the special weapon is close enough to the center
		{
			e->cTransform->velocity.x = 0;
			e->cTransform->velocity.y = 0;
			
			for (auto& e2 : m_entities.getEntities("enemy"))
			{
				Vec2 v1(e->cTransform->pos.x, e->cTransform->pos.y);
				Vec2 v2(e2->cTransform->pos.x, e2->cTransform->pos.y);
				Vec2 dist = v1 - v2;
				dist.normalize();
				e2->cTransform->velocity = dist * 5.0f;

				
			}
		}
		
	}

	

}

void Game::sLifeSpan()
{
	//implelnt lifespan here
	//if has no lifespan component skip it
	//if entity has > 0 remining lifespan skip it
	//if it has lifespan and is alive scale its alpha channel properly
	//if it's lifespan is up destroy the entity 

	//lifespan for the bullets
	for (auto& e : m_entities.getEntities("bullet"))
	{
		if (e->cLifespan)
		{
			sf::Color color = e->cShape->circle.getFillColor();
			e->cLifespan->remaining -= 5;
			color.a -= 5;
			if (color.a < 0)
			{
				color.a = 0;
				
			}
			e->cShape->circle.setFillColor(color);
			if (e->cLifespan->remaining <= 0)
			{
				e->destroy();
				continue;
			}

		}
	}

	//life span for the small enemies
	for (auto& e : m_entities.getEntities("smallEnemy"))
	{
		if (e->cLifespan)
		{
			sf::Color color = e->cShape->circle.getFillColor();
			sf::Color outlineColor = e->cShape->circle.getOutlineColor();
			e->cLifespan->remaining -= 5;
			color.a -= 5;
			outlineColor.a -= 5;
			if (color.a < 0 && outlineColor.a < 0)
			{
				color.a = 0;
				outlineColor.a = 0;
			}
			e->cShape->circle.setFillColor(color);
			e->cShape->circle.setOutlineColor(outlineColor);
			if (e->cLifespan->remaining <= 0)
			{
				e->destroy();
				continue;
			}

		}
	}

	for (auto& e : m_entities.getEntities("specialWeapon"))
	{
		if (e->cLifespan)
		{
			e->cLifespan->remaining -= 0.5;
			if (e->cLifespan->remaining <= 0)
			{
				e->destroy();
				m_activeWeapon = false;
				continue;
			}
		}
	}	

}

void Game::sCollision()
{
	//all the entities collisions should be here 

	//collisioon detection between a bullet and an enemy
	for (auto& e : m_entities.getEntities("bullet"))
	{
		for (auto& e2 : m_entities.getEntities("enemy"))
		{
			if (e->cCollision && e2->cCollision)
			{
				Vec2 v1(e->cTransform->pos.x, e->cTransform->pos.y);
				Vec2 v2(e2->cTransform->pos.x, e2->cTransform->pos.y);
				float length = v1.dist(v2);
				if (e->cCollision->radius + e2->cCollision->radius > length)
				{
					e->destroy();
					e2->destroy();
					spawnSmallEnemies(e2);
					m_score += e2->cSore->score;
				}
			}

		}
	}
	//collision deection for when the enemy hits the edges of the screen
	for (auto& e : m_entities.getEntities("enemy"))
	{
		if (e->cShape->circle.getPosition().y > m_window.getSize().y - e->cCollision->radius)
		{
			e->cTransform->velocity.y  = -e->cTransform->velocity.y;

		}
		if (e->cShape->circle.getPosition().x > m_window.getSize().x -  e->cCollision->radius)
		{
			e->cTransform->velocity.x = -e->cTransform->velocity.x;

		}
		if (e->cShape->circle.getPosition().y < e->cCollision->radius)
		{
			e->cTransform->velocity.y = -e->cTransform->velocity.y;

		}
		if (e->cShape->circle.getPosition().x < e->cCollision->radius)
		{
			e->cTransform->velocity.x = -e->cTransform->velocity.x;

		}
		e->cTransform->pos.x += e->cTransform->velocity.x;
		e->cTransform->pos.y += e->cTransform->velocity.y;
	}
	
	//collision detection for the player when it hits the edges of the screen
	for (auto& e : m_entities.getEntities("player"))
	{

		if (e->cTransform->pos.y > m_window.getSize().y-e->cCollision->radius)
		{
			
			e->cTransform->pos.y = m_window.getSize().y - e->cCollision->radius;
			
		}
		for (auto& e : m_entities.getEntities("player"))
		{

			if (e->cTransform->pos.y > m_window.getSize().y - e->cCollision->radius)
			{

				e->cTransform->pos.y = m_window.getSize().y - e->cCollision->radius;

			}
			if (e->cShape->circle.getPosition().x > m_window.getSize().x - e->cCollision->radius)
			{
				e->cTransform->pos.x = m_window.getSize().x - e->cCollision->radius;

			}
			if (e->cShape->circle.getPosition().y < 0 )
			{
				e->cTransform->pos.y = 0;

			}
			if (e->cShape->circle.getPosition().x <0)
			{
				e->cTransform->pos.x = 0;

			}


		}
	}
	
	//collsion detection when the enemy hits the player
	for (auto& e : m_entities.getEntities("enemy"))
	{
		for (auto& e2 : m_entities.getEntities("player"))
		{
			if (e->cCollision && e2->cCollision)
			{
				Vec2 v1(e->cTransform->pos.x, e->cTransform->pos.y);
				Vec2 v2(e2->cTransform->pos.x, e2->cTransform->pos.y);
				float length = v1.dist(v2);
				if (e->cCollision->radius + e2->cCollision->radius > length)
				{
					e->destroy();
					e2->destroy();
					spawnPlayer();
					m_score = 0;
				}
			}

		}
	}

	

}

void Game::sEnemySpawner()
{
	//*note use m_currentframe-m_lastEnemySpawn 
	// to determine how long it has been since the last enemy spawned

	float time_passed = m_currentFrame - m_lastEnemyspawnTime;

	if (time_passed > 100)
	{
		spawnEnemy();
	}
}

void Game::sRender()
{
	m_window.clear();

	// Render entities
	for (auto& e : m_entities.getEntities())
	{
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
		e->cTransform->angle += 1.0f;
		e->cShape->circle.setRotation(e->cTransform->angle);
		m_window.draw(e->cShape->circle);
	}

	if (!m_font.loadFromFile("Fonts/arial.ttf")) // replace with the path to your font file
	{
		// handle error
	}

	// Render text using loaded font
	m_text.setFont(m_font); // Set the font
	std::string scoreString = "The score is " + std::to_string(m_score);
	m_text.setString(scoreString); // Set the string to display
	m_text.setCharacterSize(24); // Set the character size
	m_text.setFillColor(sf::Color::White); // Set the fill color
	m_text.setPosition(10.f, 10.f); // Set the position (top-left corner)
	m_window.draw(m_text);

	if (m_currentFrame - m_lastSpecialWeaponSpawnTime >= 1000)
	{
		// If it has, the player is able to shoot the special weapon
		m_activeWeapon = true;
	}

	std::string weaponStatus;

	m_activeWeapon ? weaponStatus = "online\n" : weaponStatus = "offline\n";
	std::string weaponString = "The special weapon is: " + weaponStatus;
	m_text.setString(weaponString); // Set the string to display
	m_text.setCharacterSize(24); // Set the character size
	m_text.setFillColor(sf::Color::White); // Set the fill color
	m_text.setPosition(10.f, 40.f); // Set the position (top-left corner)
	m_window.draw(m_text);


	m_window.display();
	
}


void Game::sUserInput()
{

	sf::Event event;

	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}
		//this event is trigered when the W key is pressed 
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				//set player input component up to true 
				m_player->cInput->up = true;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = true;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = true;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = true;
				break;
			case sf::Keyboard::P:
				m_paused = !m_paused;
				m_paused ? std::cout << "Game Paused\n" : std::cout << "Game Unpaused\n";
				setPaused(m_paused);
				break;
			default:
				break;

			}
		}

		//this event is triggered when the w key is released
		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				//set player input component up to false 
				m_player->cInput->up = false;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = false;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = false;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = false;
				break;
			case sf::Keyboard::Space:
				spawnSpecialWeapon(m_player);
			default:
				break;

			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
			
				spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));

			}

			if (event.mouseButton.button == sf::Mouse::Right)
			{
				
			}
		}
	}
}