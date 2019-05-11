#include "Game.h"
#include <cmath>
#include <fstream>

// define V1 fro the first version of ball physics, V2 for the second
#define V1
//#define V2

std::istream& operator>>(std::istream& is, sf::Vector2f& pos)
{
    is >> pos.x >> pos.y;
    return is;
}

std::istream& operator>>(std::istream& is, sf::IntRect& texture)
{
    is >> texture.left >> texture.top >> texture.width >> texture.height;
    return is;
}


Game::Game()
{
	window.create(sf::VideoMode(width, height), "Arkandroid");
	srand(time(0));
	//window.setVerticalSyncEnabled(true);

	////////////////////////////////////////////////////
	bg_texture.loadFromFile("background.png");
	bg.setTexture(bg_texture);
	
	if(!texture.loadFromFile("t.png")){
		return;
	}
	
	brick.setTexture(texture);
	brick.scale(2.0f, 2.0f);
	
	frame_rect = sf::IntRect(25, 105, 35, 11);
	
	board.setTexture(texture);
	board.setTextureRect(frame_rect);
	board.setOrigin(35/2, 11/2);
	board.setPosition(350, 590);
	board.scale(2.0f, 2.0f);
	
	board_velocity.x = 0;
	board_velocity.y = 0;
	
	
	ball.setTexture(texture);
	ball.setTextureRect(sf::IntRect(42, 92, 8, 8));
	ball.setOrigin(8/2, 8/2);
	ball.scale(2.0f, 2.0f);
	
	ball_velocity.x = 8;
	ball_velocity.y = 8;
	
	//GenerateLvl();
	LoadLevel();
	
	////////////////////////////////////////////////////
	
	current_state = States::game_state;
	clock.restart();
}

void Game::Reset(bool load_new_level)
{	
	if(load_new_level){
		LoadLevel();		
	}
	move_ball = false;
}

void Game::LoadLevel()
{
	Block block;
	std::ifstream level_file;
	static std::streampos new_level = 0;
	level_file.open("level_1.lvl");
	
	// Read level size first
	level_file.seekg (new_level);
	level_file >> level_size;
	
	for(int i = 0; i < level_size; ++i){
		level_file >> block.position >> block.texture;
		brick.setPosition(block.position.x, block.position.y);
		brick.setTextureRect(block.texture);	
		bricks.push_back(brick);
	}
	new_level = level_file.tellg();
	level_file.close();
}

void Game::Update()
{
	while(window.pollEvent(event)){
		if(event.type == sf::Event::Closed){
			window.close();
		}
	}

	if(elapsed.asSeconds() >= frametime){
		if(current_state == States::game_over){
			GameOverAnimation();
		}
		if(current_state == States::game_state){
			if(move_board){
				MoveBoard();			
			}	
			if(move_ball){
				MoveBall();
			} else {
				ball.setPosition(board.getPosition().x, board.getPosition().y-25);
			}

			BoardAnimation();
		}
		elapsed -= sf::seconds(frametime);
	}
}

void Game::HandleInput()
{
	if ((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Escape)){
			was_released = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
		if(current_state == States::game_state && was_released){
			current_state = States::pause_state;
			was_released = false;
			return;
		}
		if(current_state == States::pause_state && was_released){
			current_state = States::game_state;
			was_released = false;
			elapsed -= elapsed;
			return;
		}

	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){move_board = true; direction = -1;}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){move_board = true; direction = 1;}
	if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){board_velocity.x = 0; direction = 0;}
	
	// Reset
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)){ball.setPosition(320, 240); current_state = States::game_state;}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){move_ball = true;}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){std::cout << ball_velocity.x << " " << direction << std::endl;}
	
	
	// Mouse input
	if(restart_btn.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))){
		restart_btn.setOutlineThickness(2);
		restart_btn.setOutlineColor(sf::Color(0, 0, 0));
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && current_state == States::game_over){
			
			current_state = States::game_state;
		}
	} else {restart_btn.setOutlineThickness(0);}
}

void Game::BoardAnimation()
{
	static bool reverse = false;
	static int frame_counter = 0;
	if(frame_counter == 3){reverse = true;}
	if(frame_counter == 0){reverse = false;}
	
	animation_frame_time += elapsed.asSeconds();

	if(animation_frame_time >= 0.2f){
		switch(frame_counter){
			case 0: {frame_rect = sf::IntRect(25, 105, 35, 11);}
			break;
			
			case 1: {frame_rect = sf::IntRect(64, 105, 35, 11);}
			break;
			
			case 2: {frame_rect = sf::IntRect(105, 105, 35, 11);}
			break;
			
			case 3: {frame_rect = sf::IntRect(146, 105, 35, 11);}
			break;
		}
		board.setTextureRect(frame_rect);
		animation_frame_time = 0;
		if(reverse){
			--frame_counter;
		} else {
			++frame_counter;			
		}
	}
}

void Game::GameOverAnimation()
{
	static int frame_counter = 0;
	
	
	animation_frame_time += elapsed.asSeconds();
	
	if(animation_frame_time >= 0.1f && frame_counter < 5){
		switch(frame_counter){
			
			case 0: {frame_rect = sf::IntRect(25, 127, 36, 13);}
			break;
			
			// Compensate height of the new textures by moving the board up 5 p. (reseting the origin point of course)
			case 1: {
				frame_rect = sf::IntRect(66, 124, 30, 16);
				board.setOrigin(30/2, 16/2);
				board.setPosition(board.getPosition().x, 590-5);
			}
			break;
			
			case 2: {frame_rect = sf::IntRect(100, 124, 32, 16);}
			break;
			
			case 3: {frame_rect = sf::IntRect(138, 124, 30, 16);}
			break;
			
			case 4: {frame_rect = sf::IntRect(180, 124, 0, 0);}
			break;
		}
		board.setTextureRect(frame_rect);
		animation_frame_time = 0;
		++frame_counter;
	}
}

void Game::Render()
{
	window.clear(sf::Color::White);
	///////////////////////
	window.draw(bg);
	window.draw(board);
	window.draw(ball);
	for(int i = 0; i < bricks.size(); ++i){
		window.draw(bricks[i]);
	}
	//////////////////////
	window.display();
}

void Game::MoveBoard(){
	
	if(board_velocity.x < board_speed){
		board_velocity.x += 2;		
	}
	
	switch(direction){
		case 1:{
			if(board.getPosition().x + board_velocity.x+22 < 687){
				board.setPosition(board.getPosition().x + board_velocity.x, board.getPosition().y);				
			}
		}
		break;
		case -1:{
			if(board.getPosition().x-2-30 > 17){
				board.setPosition(board.getPosition().x - board_velocity.x, board.getPosition().y);				
			}
		}
		break;
	}
	move_board = false;
}

void Game::MoveBall(){
	
	for(int i = 0; i < bricks.size(); ++i){
		if(ball.getGlobalBounds().intersects(bricks[i].getGlobalBounds())){
			ball_velocity.y *= -1;
			
			bricks[i].setPosition(-100, -100);
			
			if(--level_size == 0){
				Reset(true);
			}
			
			if(std::abs(ball_velocity.x) + 0.4 < 20 && ball_velocity.x > 0){
				ball_velocity.x += 0.1;
			}
			if(std::abs(ball_velocity.x) + 0.4 < 20 && ball_velocity.x < 0){
				ball_velocity.x -= 0.1;
			}
		}
	}
#ifdef V1
	if(ball.getGlobalBounds().intersects(board.getGlobalBounds())){
		if(direction > 0 && ball_velocity.x > 0){
			std::cout << "Same! Going right" << std::endl;
			if((std::abs(ball_velocity.x) - (board_velocity.x)/5) > 10){
				ball_velocity.x = std::abs(ball_velocity.x) - (board_velocity.x)/5;
				std::cout << "Speed down!" << std::endl;
			}
		}
		if(direction < 0 && ball_velocity.x < 0){
			std::cout << "Same! Going left" << std::endl;
			if(ball_velocity.x - (board_velocity.x)/5 > 10){
				ball_velocity.x = ball_velocity.x - (board_velocity.x)/5;
				std::cout << "Speed down!" << std::endl;
			}
		}
		
		if(direction > 0 && ball_velocity.x < 0){
			std::cout << "Opposite! Going right" << std::endl;
			if((std::abs(ball_velocity.x) + std::abs(board_velocity.x)/5) < 24){
				ball_velocity.x = std::abs(ball_velocity.x) + (board_velocity.x)/5;				
				std::cout << "Speed up!" << std::endl;
			}
		}
		if(direction < 0 && ball_velocity.x > 0){
			std::cout << "Opposite! goint left" << std::endl;
			if((std::abs(ball_velocity.x) + std::abs(board_velocity.x)/5) < 24){
				ball_velocity.x = std::abs(ball_velocity.x) + (board_velocity.x)/5;				
				std::cout << "Speed up!" << std::endl;
			}
			ball_velocity.x *= -1;
		}

		ball_velocity.y *= -1;
		std::cout << "BOUNCE!" << std::endl;
	}
#endif

#ifdef V2
	//Slightly slow down the ball over time (once per tick)
	slow_down_time += elapsed.asSeconds();

	if(std::abs(ball_velocity.x) - 0.1 > 10 && ball_velocity.x > 0 && slow_down_time >= 0.8){
		std::cout << "sloooooowing down"<< std::endl;
		ball_velocity.x -= 0.1;
		slow_down_time = 0.0;
	}
	if(std::abs(ball_velocity.x) - 0.1 > 10 && ball_velocity.x < 0 && slow_down_time >= 0.8){
		std::cout << "sloooooowing down"<< std::endl;
		ball_velocity.x += 0.1;
		slow_down_time = 0.0;
	}
		
	if(ball.getGlobalBounds().intersects(board.getGlobalBounds())){
		
		std::cout << slow_down_time << std::endl;
	
		if(direction > 0 && ball_velocity.x > 0){
			std::cout << "Same! Going right" << std::endl;
			if((std::abs(ball_velocity.x) + (board_velocity.x)/5) < 20){
				ball_velocity.x = std::abs(ball_velocity.x) + (board_velocity.x)/5;	
				std::cout << "Speed Up!" << std::endl;
			}
		}
		if(direction < 0 && ball_velocity.x < 0){
			std::cout << "Same! Going left" << std::endl;
			if(std::abs(ball_velocity.x) + (board_velocity.x)/5 < 20){
				ball_velocity.x = ball_velocity.x + (board_velocity.x)/5;	
				std::cout << "Speed Up!" << std::endl;
			}
		}
		if(direction > 0 && ball_velocity.x < 0){
			std::cout << "Opposite! Going right" << std::endl;
			if(std::abs(ball_velocity.x) + board_velocity.x/5 < 20){
				ball_velocity.x = ball_velocity.x - (board_velocity.x)/5;				
				std::cout << "Speed up!" << std::endl;
			}
			ball_velocity.x *= -1;
		}
		if(direction < 0 && ball_velocity.x > 0){
			std::cout << "Opposite! goint left" << std::endl;
			if((std::abs(ball_velocity.x) + std::abs(board_velocity.x)/5) < 20){
				ball_velocity.x = ball_velocity.x + (board_velocity.x)/5;				
				std::cout << "Speed up!" << std::endl;
			}
			ball_velocity.x *= -1;
		}
		
		if(direction == 0){
			ball_velocity.x -= 0.5;
		}
		ball_velocity.y *= -1;
		std::cout << "BOUNCE!" << std::endl;
	}
#endif
		
	if(ball.getPosition().x - 8/2 - 17 <= 0){
		ball_velocity.x *= -1;
	}
	if(ball.getPosition().x + 8/2 >= width-17){

		ball_velocity.x *= -1;
	}
	if(ball.getPosition().y + 8/2 + 17 > 600 || ball.getPosition().y + 8/2 > board.getPosition().y+11/2){
		current_state = States::game_over;
		Reset(false);
		return;
	}
	if(ball.getPosition().y - 8/2 <= 17){
		ball_velocity.y *= -1;
	}
	
	ball.setPosition(ball.getPosition().x + ball_velocity.x, ball.getPosition().y + ball_velocity.y);
}

