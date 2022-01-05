#include "Game.h"

Game::Game(const std::string& config)
{
	init(config);
}

void Game::run()
{
	while (m_running)
	{
		m_entityManager.update();

		sUserInput();
		if (!m_paused)
		{
			sCollision();
			sMovement();
			sLifespan();
			sEnemySpawner();
			m_currentFrame++;
		}
		sRender();
	}
}

void Game::init(const std::string& config)
{
	// Random seed for rand
	srand(time(NULL));

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
	int firerate = 15; // player

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
				m_textScore.setFont(m_font);
				m_textScore.setString("Points: 99999");
				m_textScore.setCharacterSize(textWidth);
				m_textScore.setFillColor(sf::Color(textR, textG, textB, 255));
				m_textScore.setPosition(wWidth - m_textScore.getLocalBounds().width, 0);

				m_textPause.setFont(m_font);
				m_textPause.setString("ESC to pause");
				m_textPause.setCharacterSize(textWidth);
				m_textPause.setFillColor(sf::Color(textR, textG, textB, 255));
				m_textPause.setPosition(0, 0);
			}
		}
		else if (type == "Player")
		{
			fin >> shapeRadius >> collisionRadius >> speed >> fillR >> fillG >> fillB 
				>> outlineR >> outlineG >> outlineB >> outlineThickness 
				>> vertices >> firerate;

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
			m_playerConfig.Firerate = firerate;
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
		else if (type == "HomingMissile")
		{
			fin >> shapeRadius >> collisionRadius >> speed >> fillR >> fillG >> fillB >> outlineR >> outlineG >> outlineB >> outlineThickness >> vertices >> lifespan;
		
			m_homingMissileConfig.SR = shapeRadius;
			m_homingMissileConfig.CR = collisionRadius;
			m_homingMissileConfig.S = speed;
			m_homingMissileConfig.FR = fillR;
			m_homingMissileConfig.FG = fillG;
			m_homingMissileConfig.FB = fillB;
			m_homingMissileConfig.OR = outlineR;
			m_homingMissileConfig.OG = outlineG;
			m_homingMissileConfig.OB = outlineB;
			m_homingMissileConfig.OT = outlineThickness;
			m_homingMissileConfig.V = vertices;
			m_homingMissileConfig.L = lifespan;
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


	for (auto& homingMissile : m_entityManager.getEntities("bullet"))
	{
		if (homingMissile->getType() != "homingMissile") { continue; }

		std::shared_ptr<Entity> closestEnemy;
		float smallestDist = std::numeric_limits<float>::max();
		for (const auto& enemy : m_entityManager.getEntities("enemy"))
		{
			float dist = homingMissile->cTransform->pos.dist(enemy->cTransform->pos);
			if (dist < smallestDist)
			{
				smallestDist = dist;
				closestEnemy = enemy;
			}
		}

		if (!closestEnemy) { continue; }

		int pDot = Vec2::perpDot(closestEnemy->cTransform->pos - homingMissile->cTransform->pos, homingMissile->cTransform->velocity);
		if (pDot < 0)
		{
			// Rotate missile counter-clockwise
			homingMissile->cTransform->velocity = Vec2::rotate(homingMissile->cTransform->velocity, -0.05);
		}
		else if (pDot > 0)
		{
			// Rotate missile clockwise
			homingMissile->cTransform->velocity = Vec2::rotate(homingMissile->cTransform->velocity, 0.05);
		}
		else
		{
			// Either towards or away from target
			int dot = Vec2::dot(closestEnemy->cTransform->pos - homingMissile->cTransform->pos, homingMissile->cTransform->velocity);
			if (dot < 0)
			{
				// moving directly away target
				homingMissile->cTransform->velocity = Vec2::rotate(homingMissile->cTransform->velocity, -0.05);
			}
			else
			{
				// moving directly towards target
			}
		}
	}

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
			case sf::Keyboard::Escape:
				m_paused = !m_paused;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
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
				spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
			}

			if (event.mouseButton.button == sf::Mouse::Right)
			{
				spawnSpecialWeapon(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
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

	if (m_paused)
	{
		m_textPause.setString("ESC to unpause");
	}
	else if (!m_paused)
	{
		m_textPause.setString("ESC to pause");
	}

	m_textScore.setString("Points: " + std::to_string(m_score));
	m_window.draw(m_textScore);
	m_window.draw(m_textPause);

	m_window.display();
}

void Game::sEnemySpawner()
{
	if (m_currentFrame - m_lastEnemySpawnTime >= m_enemyConfig.SI)
	{
		spawnEnemy();
	}
}

void Game::sCollision()
{
	// TODO: implement enemy hierarchy to simplify loops

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
				restart();
				break;
			}
		}
	}

	// Bullet/missile and enemy collision
	for (auto& b : m_entityManager.getEntities("bullet"))
	{
		checkBulletEnemyCollision(b);
	}

	// Enemies and small enemies bounce off falls
	for (auto& e : m_entityManager.getEntities("enemy"))
	{
		checkWindowCollision(e);
	}

	// Player cant leave bounds
	for (auto& p : m_entityManager.getEntities("player"))
	{
		checkWindowCollision(p);
	}
}

void Game::sLifespan()
{
	for (auto& e : m_entityManager.getEntities())
	{
		if (!e->cLifespan) { continue; }
		else if (e->cLifespan->remaining > 0)
		{
			e->cLifespan->remaining -= 1;

			auto fillColor = e->cShape->circle.getFillColor();
			auto outlineColor = e->cShape->circle.getOutlineColor();

			int newAlpha = ((float)e->cLifespan->remaining / (float)e->cLifespan->total) * 255;
			sf::Color newFillColor(fillColor.r, fillColor.g, fillColor.b, newAlpha);
			sf::Color newOutlineColor(outlineColor.r, outlineColor.g, outlineColor.b, newAlpha);

			e->cShape->circle.setFillColor(newFillColor);
			e->cShape->circle.setOutlineColor(newOutlineColor);
		}
		else if (e->cLifespan->remaining == 0)
		{
			e->destroy();
		}
	}
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
	auto enemy = m_entityManager.addEntity("enemy", "regular");

	int distFromPlayerSquared = std::numeric_limits<int>::min();
	float randX = 50, randY = 50;
	// Minimum enemy spawn location from player based on collision radius
	while (distFromPlayerSquared < m_player->cCollision->radius * m_player->cCollision->radius * 3)
	{
		// Random between min and max: rand()%(max-min+1)+min
		randX = rand() % ((m_window.getSize().x - m_enemyConfig.CR) - m_enemyConfig.CR + 1) + m_enemyConfig.CR;
		randY = rand() % ((m_window.getSize().y - m_enemyConfig.CR) - m_enemyConfig.CR + 1) + m_enemyConfig.CR;
		distFromPlayerSquared = (randX + m_player->cTransform->pos.x) * (randX + m_player->cTransform->pos.x) + 
			(randY + m_player->cTransform->pos.y) * (randY + m_player->cTransform->pos.y);
	}

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
	int vertices = entity->cShape->circle.getPointCount();
	float vx;
	float vy;

	for (int i = 0; i < vertices; i++)
	{
		auto smallEnemy = m_entityManager.addEntity("enemy", "smallEnemy");
		smallEnemy->cShape = std::make_shared<CShape>(
			m_enemyConfig.SR / 2, // Shape radius
			vertices, // Sides
			entity->cShape->circle.getFillColor(), // Fill color
			entity->cShape->circle.getOutlineColor(), // Outline color
			m_playerConfig.OT / 2 // Outline thickness
			);
		smallEnemy->cScore = std::make_shared<CScore>(entity->cScore->score * 2);
		smallEnemy->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR / 2);
		smallEnemy->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);

		// Rotate each smallEnemy evenly from destroyed regular enemy
		Vec2 rotatedVelocity = Vec2::rotate(entity->cTransform->velocity, 360 / vertices * i * M_PI / 180);
		smallEnemy->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, rotatedVelocity, entity->cTransform->angle);
	}
}

// 1. position shooting from
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos)
{
	if (m_currentFrame - m_lastBulletTime >= m_playerConfig.Firerate)
	{
		float dist = entity->cTransform->pos.dist(mousePos);
		Vec2 normalizedVec = (mousePos - entity->cTransform->pos) / dist;
		float vx = cosf(45 * M_PI / 180) * normalizedVec.x * m_bulletConfig.S;
		float vy = sinf(45 * M_PI / 180) * normalizedVec.y * m_bulletConfig.S;
		auto bullet = m_entityManager.addEntity("bullet");

		bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, Vec2(vx, vy), 0.0f);
		bullet->cShape = std::make_shared<CShape>(
			m_bulletConfig.SR,
			m_bulletConfig.V,
			sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB, 255),
			sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB),
			m_bulletConfig.OT);
		bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
		bullet->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
		m_lastBulletTime = m_currentFrame;
	}
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity, const Vec2& mousePos)
{
	float dist = entity->cTransform->pos.dist(mousePos);
	Vec2 normalizedVec = (mousePos - entity->cTransform->pos) / dist;
	float vx = cosf(45 * M_PI / 180) * normalizedVec.x * m_homingMissileConfig.S;
	float vy = sinf(45 * M_PI / 180) * normalizedVec.y * m_homingMissileConfig.S;
	auto bullet = m_entityManager.addEntity("bullet", "homingMissile");

	bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, Vec2(vx, vy), 0.0f);
	bullet->cShape = std::make_shared<CShape>(
		m_homingMissileConfig.SR,
		m_homingMissileConfig.V,
		sf::Color(m_homingMissileConfig.FR, m_homingMissileConfig.FG, m_homingMissileConfig.FB, 255),
		sf::Color(m_homingMissileConfig.OR, m_homingMissileConfig.OG, m_homingMissileConfig.OB),
		m_homingMissileConfig.OT);
	bullet->cCollision = std::make_shared<CCollision>(m_homingMissileConfig.CR);
	bullet->cLifespan = std::make_shared<CLifespan>(m_homingMissileConfig.L);
}

void Game::checkWindowCollision(std::shared_ptr<Entity> entity)
{
	if (entity->cTransform->pos.x - entity->cCollision->radius <= 0)
	{
		if (!entity->cInput)
		{
			entity->cTransform->velocity.x *= -1;
		}
		else
		{
			entity->cInput->right = false;
		}
	}
	if (entity->cTransform->pos.x + entity->cCollision->radius >= m_window.getSize().x)
	{
		if (!entity->cInput) 
		{
			entity->cTransform->velocity.x *= -1;
		}
		else 
		{
			entity->cInput->left = false;
		}
	}
	if (entity->cTransform->pos.y - entity->cCollision->radius <= 0)
	{
		if (!entity->cInput)
		{
			entity->cTransform->velocity.y *= -1;
		}
		else
		{
			entity->cInput->up = false;
		}
	}
	if (entity->cTransform->pos.y + entity->cCollision->radius >= m_window.getSize().y)
	{
		if (!entity->cInput)
		{
			entity->cTransform->velocity.y *= -1;
		}
		else
		{
			entity->cInput->down = false;
		}
	}
}

void Game::checkBulletEnemyCollision(std::shared_ptr<Entity> b)
{
	for (auto& e : m_entityManager.getEntities("enemy"))
	{
		// distance between entities < sum of the radiases = collision
		float dist = b->cTransform->pos.dist(e->cTransform->pos);

		if (dist < (b->cCollision->radius + e->cCollision->radius))
		{
			b->destroy();
			//m_player->cInput->shoot = false;
			e->destroy();
			m_score += e->cScore->score;
			if (e->getType() == "regular") 
			{ 
				spawnSmallEnemies(e); 
			}
			break;
		}
	}
	if (b->cTransform->pos.x < 0 ||
		b->cTransform->pos.x > m_window.getSize().x ||
		b->cTransform->pos.y < 0 ||
		b->cTransform->pos.y > m_window.getSize().y)
	{
		b->destroy();
		//m_player->cInput->shoot = false;
	}
}

void Game::restart()
{
	m_score = 0;
	m_entityManager.clearEntities();
	spawnPlayer();
}
