#include "Game.h"

Game::Game(const std::string& config)
{
	init(config);
}

void Game::run()
{
	// TODO: add pause functionality
	// Some systems should function while paused (rendering)
	// some should not (movement / input)
	while (m_running)
	{
		m_entityManager.update();

		if (!m_paused)
		{

		}

		sEnemySpawner();
		sMovement();
		sCollision();
		sUserInput();
		sRender();

		// may need to be moved when pause implemented
		m_currentFrame++;
	}
}

void Game::init(const std::string& config)
{
	//TODO read in config file here
	// Use the premade PlayerConfig... variables

	//std::ifstream fin(config);

	// set up default window parameters
	m_window.create(sf::VideoMode(1280, 720), "Shape game");
	m_window.setFramerateLimit(60);

	spawnPlayer();
}

void Game::setPaused(bool paused)
{
}

void Game::sMovement()
{
	// Read m_player->cInput component to determine if player is moving

	Vec2 playerVelocity;
	if (m_player->cInput->left)
	{
		playerVelocity.x -= m_playerConfig.S;
	}
	if (m_player->cInput->right)
	{
		playerVelocity.x += m_playerConfig.S;
	}
	// up,down

	m_player->cTransform->velocity = playerVelocity;

	m_player->cTransform->pos += m_player->cTransform->velocity;

	//m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
	//m_player->cTransform->pos.y += m_player->cTransform->velocity.y;

	for (auto e : m_entityManager.getEntities())
	{
		e->cTransform->pos += e->cTransform->velocity;
	}
}

void Game::sUserInput()
{
	// set the player's input component variables here
	// not implement player's movement logic here
	// Movement system will read variables set here

	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				std::cout << "W press" << std::endl;
				break;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			// Set input components to false
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				// Set input component up to false
				break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				std::cout << event.mouseButton.x << "," << event.mouseButton.y << std::endl;
				// call spawnBullet
			}

			if (event.mouseButton.button == sf::Mouse::Right)
			{
				// call spawnSpecialWeapon
			}
		}
	}
}

void Game::sRender()
{
	m_window.clear();

	m_player->cShape->circle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);

	m_player->cTransform->angle += 1.0f;
	m_player->cShape->circle.setRotation(m_player->cTransform->angle);

	m_window.draw(m_player->cShape->circle);

	m_window.display();
}

void Game::sEnemySpawner()
{
	// (m_currentFrame - m_lastEnemySpawnTime) to determine how long has been
}

void Game::sCollision()
{
	for (auto p : m_entityManager.getEntities("player"))
	{
		for (auto e : m_entityManager.getEntities("enemyOrsmtgh"))
		{
			// distance between entities < sum of the radiases = collision
			float dist = p->cTransform->pos.dist(e->cTransform->pos);
			//p->cCollision
		}
	}

	for (auto p : m_entityManager.getEntities("bullet"))
	{
		for (auto e : m_entityManager.getEntities("enemyOrsmtgh"))
		{
			// distance between entities < sum of the radiases = collision
			float dist = p->cTransform->pos.dist(e->cTransform->pos);
			//p->cCollision
		}
	}

	// enemies bounce off falls and player cant leave bounds

	// all collisions between entities
	// collision radius, not the shape radius
}

void Game::sLifespan()
{
	for (auto e : m_entityManager.getEntities())
	{
		if (!e->cLifespan) { continue; }
	}

	// for all entities, if has lifespan and alive, scale its alpha channel properly
	// if entity has no lifespan component, skip it
	// if entity has remaining > 0 current lifespan, subtract 1
	// if has lifespan and its time is up, destroy it
	// has been alive for m_currentFrame - e->cLifespan->frameCreated frames

	/*
	auto color = m_player->cShape->circle.getFillColor();
	int newAlhpa = 100;
	sf::Color newColor(color.r, color.g, color.b, newAlhpa);
	m_player->cShape->circle.setFillColor(newColor);
	*/
}

void Game::spawnPlayer()
{
	// TODO: finish adding all properties of the player with the correct values from config

	// We create every entity by calling EntityManager.addEntity(tag)
	auto entity = m_entityManager.addEntity("player");

	// Spawn at (200,200) with velocity (1,1) and angle 0
	entity->cTransform = std::make_shared<CTransform>(Vec2(200.0f, 200.0f), Vec2(1.0f, 1.0f), 0.0f);

	// Entity shape: radius 32, 8 sides, dark grey fill and red outline of thickness 4
	entity->cShape = std::make_shared<CShape>(32, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);

	// Add input component to the player
	entity->cInput = std::make_shared<CInput>();

	// Set entity as player
	m_player = entity;

}

void Game::spawnEnemy()
{
	// TODO: enemy spawned with m_enemyConfig variables
	// spawn within bounds of the window

	// record when last enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	// Todo: spawn at location of input enemy entity

	int vertices = entity->cShape->circle.getPointCount();

	for (int i = 0; i < vertices; i++)
	{
		auto smallEnemy = m_entityManager.addEntity("smallEnemy");
		// ...
		smallEnemy->cScore = std::make_shared<CScore>(entity->cScore->score * 2);
	}

	// When we create smaller enemy, we have to read the values of the original enemy
	// - spawn a numebr of small enemies equal to the vertices of the original enemy
	// - set the color to the same as original, half the size
	// - small enemies are worth double points
}

// 1. position shooting from
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos)
{
	// bullet speed is given as a scalar speed, pixels per frame added to the bullet
	// set the velocity by using formula, traveling x-axel vs traveling 45 degree angle
	// x = cos(45)*scalar
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	// TODO
}
