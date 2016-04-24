#include "SdlClass.h"
#include "stdafx.h"

#ifdef TEST

#include TEST_C

int main(int argc, char** argv) {
	TEST();
	return 0;
}
#else
#include "TetrisGame.h"

int main(int argc, char** argv) {
	TetrisGame::TetrisGame tg;
	tg.Play();
	return 0;
}
#endif
