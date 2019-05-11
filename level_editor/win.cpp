#include "win.h"
#include <cmath>
#include <fstream>

#define mouse_position static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))

std::ostream& operator<<(std::ostream& os, sf::Vector2f& pos)
{
    os << pos.x << ' ' << pos.y;
    return os;
}

std::ostream& operator<<(std::ostream& os, sf::IntRect& texture)
{
    os << texture.left << ' ' << texture.top << ' ' << texture.width << ' ' << texture.height;
    return os;
}


Win::Win()
{
	window.create(sf::VideoMode(width, height), "Arkandroid");
	srand(time(0));
	window.setVerticalSyncEnabled(true);

	
	// TEXT STUFF
	font.loadFromFile("Roboto-Regular.ttf");
	
	save_lvl_txt.setFont(font);
	save_lvl_txt.setCharacterSize(12);
	save_lvl_txt.setFillColor(sf::Color::Red);
	save_lvl_txt.setString("Save Level");
		
	save_lvl_txt.setPosition(789, 402);
	
	// BUTTON STUFF
	
	save_btn.setSize(sf::Vector2f(70, 20));
	save_btn.setPosition(781, 400);
	
	////////////////////////////////////////////////////
	bg_texture.loadFromFile("background.png");
	bg.setTexture(bg_texture);
	
	texture.loadFromFile("t.png");

	
	brick.setTexture(texture);
	brick.setTextureRect(sf::IntRect(26, 80, 16, 8));
	brick.scale(2.0f, 2.0f);
	
	lineH.setSize(sf::Vector2f(670.f, 1.f));
	lineH.setFillColor(sf::Color::Red);

	
	lineV.setSize(sf::Vector2f(583.f, 1.f));
	lineV.setFillColor(sf::Color::Red);
	lineV.rotate(90);



	dialog_box.setSize(sf::Vector2f(300.f, 100.f));
	dialog_box.setOrigin(150, 50);
	dialog_box.setPosition(350, 300);
	
	text_box.setSize(sf::Vector2f(290.f, 30.f));
	text_box.setOrigin(145, 15);
	text_box.setPosition(350, 300);
	text_box.setOutlineColor(sf::Color::Black);
	text_box.setOutlineThickness(2);
	
	enter_name_txt.setFont(font);
	enter_name_txt.setCharacterSize(20);
	enter_name_txt.setFillColor(sf::Color::Black);
	enter_name_txt.setString("Enter level name");

	enter_name_txt.setPosition(207, 285);
	
	file_name_txt.setFont(font);
	file_name_txt.setCharacterSize(20);
	file_name_txt.setFillColor(sf::Color::Black);
	file_name_txt.setPosition(207, 285);
	
	
	// UI elements
	// currently selected brush
	highlight_box.setSize(sf::Vector2f(50.f, 50.f));
	highlight_box.setPosition(790, 40);
	highlight_box.setFillColor(sf::Color(220, 220, 220, 200));
	highlight_box.setOutlineColor(sf::Color::Black);
	highlight_box.setOutlineThickness(2);
	
	SetMesh();
	SetPalette();

	current_state = States::game_state;
	clock.restart();
}

void Win::SetPalette()
{
	for(int brick_num = 0; brick_num < 8; ++brick_num){
		switch(brick_num){
			case 0 : {brick.setTextureRect(sf::IntRect(26, 80, 16, 8)); }
			break;
			case 1 : {brick.setTextureRect(sf::IntRect(42, 80, 16, 8)); }
			break;
			case 2 : {brick.setTextureRect(sf::IntRect(58, 80, 16, 8)); }
			break;
			case 3 : {brick.setTextureRect(sf::IntRect(74, 80, 16, 8)); }
			break;
			case 4: {brick.setTextureRect(sf::IntRect(90, 80, 16, 8)); }
			break;
			case 5: {brick.setTextureRect(sf::IntRect(106, 80, 16, 8)); }
			break;
			case 6: {brick.setTextureRect(sf::IntRect(122, 80, 16, 8)); }
			break;
			case 7: {brick.setTextureRect(sf::IntRect(138, 80, 16, 8)); }
			break;
		}
		brick.setPosition(800, 100+(brick_num*25));
		palette.push_back(brick);
	}

}

void Win::SetMesh()
{
	int brick_h = 32;
	int brick_v = 16;
	
	int border_offset = 17;
	
	int counter_h = 36;			// 583/brick_v;
	int counter_v = 22;			// 670/brick_h;
	
	sf::Vector2f inter_point(0, 0);
	
	for(int h = 0; h < counter_h; ++h){
		lineH.setPosition(border_offset, h*brick_v+border_offset);
		mesh.push_back(lineH);
		
	}
	for(int v = 0; v < counter_v; ++v){
		lineV.setPosition(v*brick_h+border_offset, border_offset);
		
		mesh.push_back(lineV);
	}

	for(int i = 0; i < mesh.size()-22; ++i){
		for(int j = 35; j < mesh.size()-1; ++j){
			inter_point.y = mesh[i].getPosition().y;
			inter_point.x = mesh[j].getPosition().x;
			inter_points.push_back(inter_point);
		}
		
		
	}

}

void Win::OnButtonClick()
{
	std::ofstream level_file;
	Block block;
	
	// fill in level data
	for(int i = 0; i < bricks.size(); ++i){
		// if brick is visible (not deleted)
		if(bricks[i].getPosition().x > 0){
			block.position = bricks[i].getPosition();
			block.texture = bricks[i].getTextureRect();
			level.push_back(block);
		}
	}
	
	level_file.open (file_name + ".lvl");
	
	// Output level size first
	level_file << level_size << ' ';
	
	for(int i = 0; i < level.size(); ++i){
		level_file << level[i].position << ' ' << level[i].texture << ' ';
	}
	// Clean up all the structures and var. 
	
	level_size = 0;
	bricks.clear();
	level.clear();
	file_name = "";
	
	level_file.close();
}

void Win::Update()
{
	while(window.pollEvent(event)){
		if(event.type == sf::Event::Closed){
			window.close();
		}
		if(current_state == States::save_dialog){
			if(event.type == sf::Event::TextEntered && event.text.unicode != 13){
				if (event.text.unicode < 128){
					enter_name_txt.setPosition(-100, -100);
					
					if(event.text.unicode == 8){
						file_name = file_name.substr(0, file_name.length()-1);
					} else {
						file_name += event.text.unicode;
					}
					file_name_txt.setString(file_name);						
				}
			} 
		}
	}
	
	PaletteHandler();
	
	// Handle Text input for saving file

	if(elapsed.asSeconds() >= frametime){
		Render();
		elapsed -= sf::seconds(frametime);
	}
}

void Win::PaletteHandler()
{
	if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		was_released = true;
	}
	for(int i = 0; i < palette.size(); ++i){
		if(palette[i].getGlobalBounds().contains(mouse_position)){
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				brick = palette[i];
				selected_brush = palette[i];
				selected_brush.setPosition(800, 58);
				
				is_brush_set = true;
				was_released = false;
			}
		}
	}
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && is_brush_set && was_released && mouse_position.x < 700){
		// if brick snaps then Snap() sets it's position, if not set position by mouse cursor
		if(!Snap()){
			brick.setPosition(mouse_position.x, mouse_position.y);
		}
		bricks.push_back(brick);
		++level_size;
		was_released = false;
	}
	
}

bool Win::Snap()
{
	for(int i = 0; i < inter_points.size(); ++i){
		if(std::abs(mouse_position.x-inter_points[i].x) < 15 && std::abs(mouse_position.y-inter_points[i].y) < 14){
			brick.setPosition(inter_points[i].x, inter_points[i].y);
			return true;
		}
		if(std::abs(mouse_position.x-inter_points[i].x) < 30 && std::abs(mouse_position.y-inter_points[i].y) < 14){
			brick.setPosition(inter_points[i].x+16, inter_points[i].y);
			return true;
		}
	}
	return false;
}

void Win::HandleInput()
{
	if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		was_released = true;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && current_state == States::save_dialog){
		OnButtonClick();
		current_state = States::game_state;
	}
	// Deleting bricks by right clicking
	for(int i = 0; i < bricks.size(); ++i){
		if(bricks[i].getGlobalBounds().contains(mouse_position)){
			if(sf::Mouse::isButtonPressed(sf::Mouse::Right) && was_released){
				// !TO-DO: That is not deleting but hiding. Implement proper deletion!
				bricks[i].setPosition(-100, -100);
				--level_size;
				was_released = false;
			}
		}
	}
	if(save_btn.getGlobalBounds().contains(mouse_position)){
		
		save_btn.setOutlineThickness(2);
		save_btn.setOutlineColor(sf::Color(0, 0, 0));
		
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && was_released){
			current_state = States::save_dialog;
			was_released = false;
		}
	} else {save_btn.setOutlineThickness(0);}
}




void Win::Render()
{
	window.clear(sf::Color(127, 127, 127));
	window.draw(bg);
	window.draw(save_btn);
	window.draw(save_lvl_txt);
	
	for(int i = 0; i < mesh.size(); ++i){
		window.draw(mesh[i]);
	}
	for(int i = 0; i < bricks.size(); ++i){
		window.draw(bricks[i]);
	}
	for(int i = 0; i < palette.size(); ++i){
		window.draw(palette[i]);
	}
	
	if(current_state == States::save_dialog){
		
		sf::RectangleShape pause_screen(sf::Vector2f(900, 600));
		pause_screen.setFillColor(sf::Color(220, 220, 220, 200));
		window.draw(pause_screen);
		window.draw(dialog_box);
		window.draw(text_box);
		window.draw(enter_name_txt);
		window.draw(file_name_txt);
		
	}
	window.draw(highlight_box);
	window.draw(selected_brush);
	window.display();
}

