#include "OGame.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("AgiborCraft <id>\n");
		return -1;
	}
	int id = strtol(argv[1], 0, 10);
	OGame game(id);
	game.Run();
	return 0;
}