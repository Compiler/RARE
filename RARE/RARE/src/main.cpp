#include <RareCore.h>



int main() {
	
	
	
	Rare::RareCore core = Rare::RareCore("Rare Core");
	core.init();
	while (!core.shouldClose()) {
		core.update(); core.render();
	}
	
	core.dispose();
	//RARE_FATAL("App End");
	return 4;
}

