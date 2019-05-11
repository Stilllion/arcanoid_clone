#include "Game.h"

int main()
{
	Game g;
	while(g.window.isOpen()){
		g.HandleInput();
		g.Update();
		g.Render();
		g.elapsed += g.clock.restart();
	}
}









