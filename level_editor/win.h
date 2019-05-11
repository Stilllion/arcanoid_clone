#ifndef Win_h
#define Win_h
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

struct Win
{
	Win();
	States current_state;
	
	
	void Update();
	void HandleInput();
	void Render();
	void GameOverAnimation();
	void BoardAnimation();
	void SetMesh();
	void SetPalette();
	void PaletteHandler();
	bool Snap();
	void OnButtonClick();
	
	sf::RenderWindow window;
	sf::Event event;
	sf::Clock clock;
	sf::Time elapsed;
	sf::Font font;
	
	//Buttons
	sf::RectangleShape save_btn;
	// TEXT
	sf::Text save_lvl_txt;
	
	sf::RectangleShape lineV;
	sf::RectangleShape lineH;
	
	
	bool was_released = true;
	
	float frametime = 1.0/20.0;
	float slow_down_time;
	
	// Counts actual amout of block on level
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
	
	int width = 900;
	int height = 600;
	
	// All the bricks on the level
	//std::vector<sf::Sprite> bricks;
	std::vector<sf::Sprite> bricks;
	
	// Contains palette bricks
	std::vector<sf::Sprite> palette;
	
	// Contains all the mesh lines
	std::vector<sf::RectangleShape> mesh;
	
	// Point of intercestion
	std::vector<sf::Vector2f> inter_points;
	
	// did user pick the brick to paint 
	bool is_brush_set = false;
	
	// Animation 
	sf::IntRect frame_rect;
	float animation_frame_time;
	
	// level data
	std::vector<Block> level;
	
	// Save dialog box
	sf::RectangleShape dialog_box;
	sf::RectangleShape text_box;
	sf::Text enter_name_txt;
	sf::Text file_name_txt;
	std::string file_name = "";
	
	
	
	// UI elements
	// current selected brush
	sf::RectangleShape highlight_box;
	sf::Sprite selected_brush;
};

#endif