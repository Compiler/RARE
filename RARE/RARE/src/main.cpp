#include <RareCore.h>



int main() {
	//haha here's my comment and im also dumb!
	/*Code to put console at top left so i can read logs*/
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, 0, 0, 0, 0, 0, 0x0001 | 0x0004);
	/*End of tmp code*/



	Rare::RareCore core = Rare::RareCore("Rare Core");

	core.init();
	while (!core.shouldClose()) {
		core.update(); core.render();
		core.waitIdle();
	}

	

	core.dispose();

	return 4;//success
}
