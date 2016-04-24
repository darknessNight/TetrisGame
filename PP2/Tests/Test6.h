#pragma once
#include "../TetrisGame.h"
#include "../Block.h"

int Test6() {
	TetrisGame::TetrisGame tg;

	tg.level = 0;
	tg.score = 0;
	//TEST1
	for (int i = 0; i < tg.settings.boardWidth;i++) {
		tg.board[tg.settings.boardWidth][i] = 1;
	}
	tg.ClearLine();
	if(tg.score!=tg.settings.oneLinePoints)
		tg.window->DrawString({ (int)(tg.settings.boardHeight*tg.settings.blockSize / 2), (int)(tg.settings.boardWidth*tg.settings.blockSize / 2) }, "TEST1: FAILED!");
	else tg.window->DrawString({ (int)(tg.settings.boardHeight*tg.settings.blockSize / 2), (int)(tg.settings.boardWidth*tg.settings.blockSize / 2) }, "TEST1: PASSED!");

	//TEST2
	tg.score = 0;
	for (int i = 0; i < tg.settings.boardWidth; i++) {
		tg.board[tg.settings.boardWidth][i] = 1;
		tg.board[tg.settings.boardWidth - 1][i] = 1;
	}
	tg.ClearLine(); 
	if (tg.score != tg.settings.oneLinePoints*2)
		tg.window->DrawString({ (int)(tg.settings.boardHeight*tg.settings.blockSize / 2), (int)(tg.settings.boardWidth*tg.settings.blockSize / 2)+10 }, "TEST2: FAILED!");
	else tg.window->DrawString({ (int)(tg.settings.boardHeight*tg.settings.blockSize / 2), (int)(tg.settings.boardWidth*tg.settings.blockSize / 2)+10 }, "TEST2: PASSED!");

	//TEST3
	tg.score = 0;
	for (int i = 0; i < tg.settings.boardWidth; i++) {
		tg.board[tg.settings.boardWidth - 1][i] = 1;
		tg.board[tg.settings.boardWidth - 2][i] = 1;
		tg.board[tg.settings.boardWidth - 3][i] = 1;
	}
	tg.ClearLine();
	if (tg.score != tg.settings.oneLinePoints*4)
		tg.window->DrawString({ (int)(tg.settings.boardHeight*tg.settings.blockSize / 2), (int)(tg.settings.boardWidth*tg.settings.blockSize / 2)+20 }, "TEST3: FAILED!");
	else tg.window->DrawString({ (int)(tg.settings.boardHeight*tg.settings.blockSize / 2), (int)(tg.settings.boardWidth*tg.settings.blockSize / 2)+20 }, "TEST3: PASSED!");

	//TEST4
	tg.score = 0;
	for (int i = 0; i < tg.settings.boardWidth; i++) {
		for (int j = 1; j <= tg.settings.maxPointedLines;j++)
		tg.board[tg.settings.boardWidth - j][i] = 1;
	}
	tg.ClearLine();
	if (tg.score != tg.settings.maxLinesPoint)
		tg.window->DrawString({ (int)(tg.settings.boardHeight*tg.settings.blockSize / 2), (int)(tg.settings.boardWidth*tg.settings.blockSize / 2+30) }, "TEST4: FAILED!");
	else tg.window->DrawString({ (int)(tg.settings.boardHeight*tg.settings.blockSize / 2), (int)(tg.settings.boardWidth*tg.settings.blockSize / 2+30) }, "TEST4: PASSED!");

	//TEST5
	tg.score = 0;
	for (int i = 0; i < tg.settings.boardWidth; i++) {
		for (int j = 1; j <= tg.settings.maxPointedLines; j++)
			tg.board[tg.settings.boardWidth - j][i] = 1;
	}
	tg.ClearLine();
	if (tg.score != tg.settings.maxLinesPointDouble)
		tg.window->DrawString({ (int)(tg.settings.boardHeight*tg.settings.blockSize / 2), (int)(tg.settings.boardWidth*tg.settings.blockSize / 2 + 40) }, "TEST5: FAILED!");
	else tg.window->DrawString({ (int)(tg.settings.boardHeight*tg.settings.blockSize / 2), (int)(tg.settings.boardWidth*tg.settings.blockSize / 2 + 40) }, "TEST5: PASSED!");

	//TEST6
	tg.score = 0;
	tg.level = 2;
	for (int i = 0; i < tg.settings.boardWidth; i++) {
		for (int j = 1; j <= tg.settings.maxPointedLines; j++)
			tg.board[tg.settings.boardWidth - j][i] = 1;
	}
	tg.ClearLine();
	if (tg.score != 3 * tg.settings.maxLinesPointDouble)
		tg.window->DrawString({ (int)(tg.settings.boardHeight*tg.settings.blockSize / 2), (int)(tg.settings.boardWidth*tg.settings.blockSize / 2 + 50) }, "TEST6: FAILED!");
	else tg.window->DrawString({ (int)(tg.settings.boardHeight*tg.settings.blockSize / 2), (int)(tg.settings.boardWidth*tg.settings.blockSize / 2 + 50) }, "TEST6: PASSED!");

	//TEST7
	tg.score = 0;
	for (int i = 0; i < tg.settings.boardWidth; i++) {
		tg.board[tg.settings.boardWidth - 1][i] = 1;
	}
	tg.ClearLine();
	if (tg.score != 3 * tg.settings.oneLinePoints)
		tg.window->DrawString({ (int)(tg.settings.boardHeight*tg.settings.blockSize / 2), (int)(tg.settings.boardWidth*tg.settings.blockSize / 2 + 60) }, "TEST7: FAILED!");
	else tg.window->DrawString({ (int)(tg.settings.boardHeight*tg.settings.blockSize / 2), (int)(tg.settings.boardWidth*tg.settings.blockSize / 2 + 60) }, "TEST7: PASSED!");


	//TEST8
	tg.score = 0;
	for (int i = 0; i < tg.settings.boardWidth; i++) {
		for (int j = 1; j <= tg.settings.maxPointedLines; j++)
			tg.board[tg.settings.boardWidth - j][i] = 1;
	}
	tg.ClearLine();
	if (tg.score != 3* tg.settings.maxLinesPoint)
		tg.window->DrawString({ (int)(tg.settings.boardHeight*tg.settings.blockSize / 2), (int)(tg.settings.boardWidth*tg.settings.blockSize / 2 + 70) }, "TEST8: FAILED!");
	else tg.window->DrawString({ (int)(tg.settings.boardHeight*tg.settings.blockSize / 2), (int)(tg.settings.boardWidth*tg.settings.blockSize / 2 + 70) }, "TEST8: PASSED!");


	
	tg.window->RenderStart.clear();
	tg.window->Render();
	_sleep(10000);
	return 0;
}