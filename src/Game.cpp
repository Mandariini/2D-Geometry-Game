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

		sMovement();
		sUserInput();
		sLifespan();
		sEnemySpawner();
		sCollision();
		sRender();

		m_currentFrame++;
	}
}

void Game::init(const std::string& config)
{
	int wWidth = 1280;
	int wHeight = 720;
	int framerate = 60;
	bool fullscreen = false;

	// Common to all entities
	int shapeRadius, collisionRadius;
	int outlineR, outlineG, outlineB;
	int outlineThickness;

	// Enemy specific configs
	int vmin, vmax;
	int lifespan = 300, spawnInterval = 120;
	float smin, smax;

	// Others
	float speed = 1; // player and bullet
	int vertices; // player and bullet
	int fillR, fillG, fillB; // player and bullet

	int textWidth, textR, textG, textB;

	std::ifstream fin(config, std::ifstream::in);
	std::string type;
	while (fin >> type)
	{
		if (type == "Window")
		{
			fin >> wWidth >> wHeight >> framerate >> fullscreen;
		}
		else if (type == "Font")
		{
			std::string fileName;
			fin >> fileName >> textWidth >> textR >> textG >> textB;
			if (!m_font.loadFromFile(fileName))
			{
				std::cerr << "Could not load font." << std::endl;
			}
			else
			{
				m_text.setFont(m_font);
				m_text.setString("Points: 99999");
				m_text.setCharacterSize(textWidth);
				m_text.setFillColor(sf::Color(textR, textG, textB, 255));
				m_text.setPosition(wWidth - m_text.getLocalBounds().width, 0);
			}
		}
		else if (type == "Player")
		{
			fin >> shapeRadius >> collisionRadius >> speed >> fillR >> fillG >> fillB >> outlineR >> outlineG >> outlineB >> outlineThickness >> vertices;

			m_playerConfig.SR = shapeRadius;
			m_playerConfig.CR = collisionRadius;
			m_playerConfig.S = speed;
			m_playerConfig.FR = fillR;
			m_playerConfig.FG = fillG;
			m_playerConfig.FB = fillB;
			m_playerConfig.OR = outlineR;
			m_playerConfig.OG = outlineG;
			m_playerConfig.OB = outlineB;
			m_playerConfig.OT = outlineThickness;
			m_playerConfig.V = vertices;
		}
		else if (type == "Enemy")
		{
			fin >> shapeRadius >> collisionRadius >> smin >> smax >> outlineR >> outlineG >> outlineB >> outlineThickness >> vmin >> vmax >> lifespan >> spawnInterval;

			m_enemyConfig.SR = shapeRadius;
			m_enemyConfig.CR = collisionRadius;
			m_enemyConfig.SMIN = smin;
			m_enemyConfig.SMAX = smax;
			m_enemyConfig.OR = outlineR;
			m_enemyConfig.OG = outlineG;
			m_enemyConfig.OB = outlineB;
			m_enemyConfig.OT = outlineThickness;
			m_enemyConfig.VMIN = vmin;
			m_enemyConfig.VMAX = vmax;
			m_enemyConfig.L = lifespan;
			m_enemyConfig.SI = spawnInterval;
		}
		else if (type == "Bullet")
		{
			fin >> shapeRadius >> collisionRadius >> speed >> fillR >> fillG >> fillB >> outlineR >> outlineG >> outlineB >> outlineThickness >> vertices >> lifespan;

			m_bulletConfig.SR = shapeRadius;
			m_bulletConfig.CR = collisionRadius;
			m_bulletConfig.S = speed;
			m_bulletConfig.FR = fillR;
			m_bulletConfig.FG = fillG;
			m_bulletConfig.FB = fillB;
			m_bulletConfig.OR = outlineR;
			m_bulletConfig.OG = outlineG;
			m_bulletConfig.OB = outlineB;
			m_bulletConfig.OT = outlineThickness;
			m_bulletConfig.V = vertices;
			m_bulletConfig.L = lifespan;
		}
	}

	if (!fullscreen)
	{
		m_window.create(sf::VideoMode(wWidth, wHeight), "Geometry game");
	}
	else if (fullscreen)
	{
		m_window.create(sf::VideoMode::getDesktopMode(), "Geometry game", sf::Style::Fullscreen);
	}
	m_window.setFramerateLimit(framerate);

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
		playerVelocity.x += m_playerConfig.S;
	}
	if (m_player->cInput->right)
	{
		playerVelocity.x -= m_playerConfig.S;
	}
	if (m_player->cInput->up)
	{
		playerVelocity.y -= m_playerConfig.S;
	}
	if (m_player->cInput->down)
	{
		playerVelocity.y += m_playerConfig.S;
	}

	m_player->cTransform->velocity = playerVelocity;
	m_player->cTransform->pos += m_player->cTransform->velocity;

	// Move all other entities
	for (auto& e : m_entityManager.getEntities())
	{
		e->cTransform->pos += e->cTransform->velocity;
	}
}

void Game::sUserInput()
{
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
			case sf::Keyboard::Up:
				std::cout << "W press" << std::endl;
				m_player->cInput->up = true;
				break;
			case sf::Keyboard::S:
			case sf::Keyboard::Down:
				m_player->cInput->down = true;
				break;
			case sf::Keyboard::D:
			case sf::Keyboard::Right:
				m_player->cInput->left = true;
				break;
			case sf::Keyboard::A:
			case sf::Keyboard::Left:
				m_player->cInput->right = true;
				break;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			// Set input components to false
			switch (event.key.code)
			{
			case sf::Keyboard::W:
			case sf::Keyboard::Up:
				m_player->cInput->up = false;
				break;
			case sf::Keyboard::S:
			case sf::Keyboard::Down:
				m_player->cInput->down = false;
				break;
			case sf::Keyboard::D:
			case sf::Keyboard::Right:
				m_player->cInput->left = false;
				break;
			case sf::Keyboard::A:
			case sf::Keyboard::Left:
				m_player->cInput->right = false;
				break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				std::cout << event.mouseButton.x << "," << event.mouseButton.y << std::endl;
				//m_player->cInput->shoot = true;
				spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
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

	for (auto& e : m_entityManager.getEntities())
	{
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

		e->cTransform->angle += 1.0f;
		e->cShape->circle.setRotation(e->cTransform->angle);

		m_window.draw(e->cShape->circle);
	}

	m_text.setString("Points: " + std::to_string(m_score));
	m_window.draw(m_text);

	m_window.display();
}

void Game::sEnemySpawner()
{
	// (m_currentFrame - m_lastEnemySpawnTime) to determine how long has been
	if (m_currentFrame - m_lastEnemySpawnTime >= m_enemyConfig.SI)
	{
		spawnEnemy();
	}
}

void Game::sCollision()
{
	// Player and enemy collision
	for (auto& p : m_entityManager.getEntities("player"))
	{
		for (auto& e : m_entityManager.getEntities("enemy"))
		{
			// distance between entities < sum of the radiases = collision
			float dist = p->cTransform->pos.dist(e->cTransform->pos);
			if (dist < (p->cCollision->radius + e->cCollision->radius))
			{
				p->destroy();
			}
		}
	}

	// Bullet and enemy collision
	for (auto& b : m_entityManager.getEntities("bullet"))
	{
		for (auto& e : m_entityManager.getEntities("enemy"))
		{
			// distance between entities < sum of the radiases = collision
			float dist = b->cTransform->pos.dist(e->cTransform->pos);

			if (dist < (b->cCollision->radius + e->cCollision->radius))
			{
				b->destroy();
				e->destroy();
				m_score += e->cScore->score;
				continue;
			}
		}
		if (b->cTransform->pos.x < 0 ||
			b->cTransform->pos.x > m_window.getSize().x ||
			b->cTransform->pos.y < 0 ||
			b->cTransform->pos.y > m_window.getSize().y)
		{
			b->destroy();
		}
	}

	// Enemies bounce off falls
	for (auto& e : m_entityManager.getEntities("enemy"))
	{
		if (e->cTransform->pos.x - e->cCollision->radius <= 0 ||
			e->cTransform->pos.x + e->cCollision->radius >= m_window.getSize().x)
		{
			e->cTransform->velocity.x *= -1;
		}
		if (e->cTransform->pos.y - e->cCollision->radius <= 0 ||
			e->cTransform->pos.y + e->cCollision->radius >= m_window.getSize().y)
		{
			e->cTransform->velocity.y *= -1;
		}
	}

	// Player cant leave bounds
	for (auto& p : m_entityManager.getEntities("player"))
	{
		if (p->cTransform->pos.x - p->cCollision->radius <= 0)
		{
			p->cInput->right = false;
		}
		if (p->cTransform->pos.y - p->cCollision->radius <= 0)
		{
			p->cInput->up = false;
		}
		if (p->cTransform->pos.x + p->cCollision->radius >= m_window.getSize().x)
		{
			p->cInput->left = false;
		}
		if (p->cTransform->pos.y + p->cCollision->radius >= m_window.getSize().y)
		{
			p->cInput->down = false;
		}
	}
}

void Game::sLifespan()
{
	for (auto& e : m_entityManager.getEntities())
	{
		if (!e->cLifespan) { continue; }
		else if (e->cLifespan->remaining > 0)
		{
			//e->cLifespan->remaining -= 1;

			//auto color = e->cShape->circle.getFillColor();
			//int newAlhpa = (e->cLifespan->total - e->cLifespan->remaining);
			//sf::Color newColor(color.r, color.g, color.b, newAlhpa);
			//e->cShape->circle.setFillColor(newColor);
		}
		else if (e->cLifespan->remaining == 0)
		{
			e->destroy();
		}
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
	auto entity = m_entityManager.addEntity("player");

	// Spawn at (200,200) with velocity (1,1) and angle 0
	Vec2 middlePos = Vec2(m_window.getSize().x / 2, m_window.getSize().y / 2);
	entity->cTransform = std::make_shared<CTransform>(middlePos, Vec2(0.0f, 0.0f), 0.0f);

	entity->cShape = std::make_shared<CShape>(
		m_playerConfig.SR, // Shape radius
		m_playerConfig.V, // Sides
		sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB, 255), // Fill color
		sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), // Outline color
		m_playerConfig.OT // Outline thickness
		);

	entity->cInput = std::make_shared<CInput>();
	entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR); // Collision radius

	m_player = entity;
}

void Game::spawnEnemy()
{
	// TODO: enemy spawned with m_enemyConfig variables
	// spawn within bounds of the window

	auto enemy = m_entityManager.addEntity("enemy");

	// Spawn at random position with random velocity and angle 0
	// Random between min and max: rand()%(max-min+1)+min
	float randX = rand() % ((m_window.getSize().x - m_enemyConfig.CR) - m_enemyConfig.CR + 1) + m_enemyConfig.CR;
	float randY = rand() % ((m_window.getSize().y - m_enemyConfig.CR) - m_enemyConfig.CR + 1) + m_enemyConfig.CR;

	float xVelocity = rand() % (int)(m_enemyConfig.SMAX - m_enemyConfig.SMIN + 1) + m_enemyConfig.SMIN;
	float yVelocity = rand() % (int)(m_enemyConfig.SMAX - m_enemyConfig.SMIN + 1) + m_enemyConfig.SMIN;

	enemy->cTransform = std::make_shared<CTransform>(Vec2(randX, randY), Vec2(xVelocity, yVelocity), 0.0f);
	
	int vertices = rand() % (m_enemyConfig.VMAX - m_enemyConfig.VMIN + 1) + m_enemyConfig.VMIN;
	enemy->cShape = std::make_shared<CShape>(
		m_enemyConfig.SR, // Shape radius
		vertices, // Sides
		sf::Color(rand() % 255, rand() % 255, rand() % 255, 255), // Fill color
		sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), // Outline color
		m_playerConfig.OT // Outline thickness
		);
	enemy->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
	enemy->cScore = std::make_shared<CScore>(vertices*100);
	enemy->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);

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

	float dist = entity->cTransform->pos.dist(mousePos);
	Vec2 normalizedVec = (mousePos - entity->cTransform->pos) / dist;
	float vx = cosf(45 * M_PI / 180) * normalizedVec.x * m_bulletConfig.S;
	float vy = sinf(45 * M_PI / 180) * normalizedVec.y * m_bulletConfig.S;

	auto bullet = m_entityManager.addEntity("bullet");

	bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, Vec2(vx, vy), 0.0f);
	bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR,
		m_bulletConfig.V, 
		sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB, 255),
		sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB),
		m_bulletConfig.OT);
	bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
	bullet->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	// TODO
}
