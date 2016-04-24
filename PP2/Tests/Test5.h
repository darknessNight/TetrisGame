#pragma once
#include "../TetrisGame.h"
#include "../Block.h"

int Test5() {
	TetrisGame::TetrisGame tg;
	for (int i = 4; i < tg.settings.boardWidth; i++)
		tg.board[tg.settings.boardHeight][i] = 0xFFFF0000U;
	tg.activeBlock = TetrisGame::ReadyShapes()[0];
	//tg.BlockDown();

	tg.Play();
	return 0;
}