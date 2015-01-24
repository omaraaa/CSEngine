#include "../include/CS.h"

using namespace std;

int main(int argc, char **argv){
	CS cs;
	cs.init();

	cs.loadLuaFunction(ENTITIES::Box);
	cs.loadLua("../scripts/test.lua");

	auto player = ENTITIES::player(cs, 0, 0);
	
	while(cs.isRunning()){
		cs.update();
	}

	cs.quit();
	return 0;
}