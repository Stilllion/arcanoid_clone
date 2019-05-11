#include "win.h"

int main()
{
	Win w;
	while(w.window.isOpen()){
		w.HandleInput();
		w.Update();		
		w.Render();
		w.elapsed += w.clock.restart();
	}
}









