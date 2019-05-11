#ifndef Game_h
#define Game_h
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

#include <time.h>
#include "states.h"

struct Block
{
	sf::Vector2f position;
	sf::IntRect texture;
};

struct Game
{
	Game();
	States current_state;
	
	
	void Update();
	void HandleInput();
	void Render();
	void GameOverAnimation();
	void BoardAnimation();
	void LoadLevel();
	void Reset(bool load_new_level);
	
	sf::RenderWindow window;
	sf::Event event;
	sf::Clock clock;
	sf::Time elapsed;
	sf::Font font;
	
	//Buttons
	sf::RectangleShape restart_btn;
	// TEXT
	sf::Text pause_text;
	sf::Text game_over_text;
	sf::Text restart_text;
	
	
	bool was_released = true;
	
	float frametime = 1.0/30.0;
	float slow_down_time;
	
	// Contains the actual number of blocks on the level
	int level_size = 0;
	
	
	///////////////////////////////////////////////
	sf::Sprite board;
	sf::Sprite bg;
	
	sf::Sprite ball;
	sf::Sprite brick;

	sf::Texture texture;
	sf::Texture bg_texture;
	
	bool move_board = false;
	bool move_ball = false;
	
	int direction = 0;
	
	void MoveBall();
	void MoveBoard();
	void GenerateLvl();
	
	sf::Vector2f ball_velocity;
	sf::Vector2f board_velocity;
	
	float ball_speed = 10;
	float board_speed = 16;
	
	int width = 700;
	int height = 600;
	
	// All the bricks on the level
	std::vector<sf::Sprite> bricks;
	
	// Animation 
	sf::IntRect frame_rect;
	float animation_frame_time;
	
};

#endif