#pragma once

#include "Common.h"
#include "EntityManager.h"
#include "Entity.h"

#include <fstream>

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BullectConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };

class Game
{
public:
	Game(const std::string& config);

	void run();

private:
	void init(const std::string& config); // init the GameState with a config file path
	void setPaused(bool paused); // pause/unpause the game

	// Systems
	void sMovement();		// System: Entity position / movement update
	void sUserInput();		// System: User Input
	void sRender();			// System: Render / Drawing
	void sEnemySpawner();	// System: Spawns Enemies
	void sCollision();		// System: Collision
	void sLifespan();		// System: Lifespan

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr <Entity> entity);
	void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

	sf::RenderWindow m_window;
	EntityManager m_entityManager;

	// All other entities in entity manager
	std::shared_ptr<Entity> m_player;

	sf::Font m_font;
	sf::Text m_text;

	PlayerConfig m_playerConfig;
	EnemyConfig m_enemyConfig;
	BullectConfig m_bulletConfig;

	int m_score = 0;
	int m_currentFrame = 0;
	int m_lastEnemySpawnTime = 0;

	bool m_paused = false; // update game logic
	bool m_running = true;
};

