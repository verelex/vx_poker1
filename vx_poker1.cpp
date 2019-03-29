#include "PokerGame.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CPokerGame game;
	game.PrepareTable();
	game.Play();
	return 0;
}
