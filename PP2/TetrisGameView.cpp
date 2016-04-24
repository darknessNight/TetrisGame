#include "TetrisGame.h"
#include "Libraries\stringNSTL.hpp"


namespace TetrisGame {

	void TetrisGame::LoadBackground()
	{
		background = new SdlLibrary::Bitmap(settings.WindowWidth, settings.WindowHeight);
		background->SetBrush(SdlLibrary::Brush(settings.menuBackColor, settings.boardBackColor));
		background->FillRect(background->GetClientRect(), true);
		background->SetBrush(SdlLibrary::Brush(settings.borderColor, settings.boardBackColor, settings.borderStyle, settings.borderSize));

		//lewa kolumna
		SdlLibrary::Rect rect = background->GetClientRect();
		rect.top += settings.margin;
		rect.bottom -= settings.margin;
		rect.left += settings.margin;
		rect.right -= settings.rightColW + settings.margin;
		background->FillRect(rect);
		background->DrawRect(rect);

		settings.blockSize = (rect.right - rect.left) / settings.boardWidth;
		settings.boardHeight = (rect.bottom - rect.top) / settings.blockSize;

		//prawa kolumna
		rect.left = rect.right + settings.margin;
		rect.right += settings.rightColW;
		rect.bottom = settings.margin + settings.rightColH;
		background->FillRect(rect);
		background->DrawRect(rect);
		settings.rightColPos.x = rect.left;
		settings.rightColPos.y = rect.top;


	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void TetrisGame::LoadInfoTexts()
	{
		texts = new SdlLibrary::Bitmap(settings.rightColW, settings.rightColH);
		int h = settings.margin / 2,
			left = (int)settings.margin / 2,
			tmp = window->GetClientRect().right - settings.rightColW - left;
		texts->DrawString({ left, h }, settings.fpsText.c_str());
		settings.fpsTextPos.x = tmp + settings.fpsText.lenght()*settings.charWidth;
		settings.fpsTextPos.y = h + settings.margin;

		h += settings.textLineHeight;
		texts->DrawString({ left, h }, settings.levelText.c_str());
		settings.levelTextPos.x = tmp + settings.levelText.lenght()*settings.charWidth;
		settings.levelTextPos.y = h + settings.margin;

		h += settings.textLineHeight;
		texts->DrawString({ left, h }, settings.scoreText.c_str());
		settings.scoreTextPos.x = tmp + settings.scoreText.lenght()*settings.charWidth;
		settings.scoreTextPos.y = h + settings.margin;

		h += settings.textLineHeight;
		texts->DrawString({ left, h }, settings.timeText.c_str());
		settings.timeTextPos.x = tmp + settings.timeText.lenght()*settings.charWidth;
		settings.timeTextPos.y = h + settings.margin;

		h += settings.textLineHeight;
		texts->DrawString({ left, h }, settings.savesText.c_str());
		settings.savesTextPos.x = tmp + settings.savesText.lenght()*settings.charWidth;
		settings.savesTextPos.y = h + settings.margin;

		h += settings.textLineHeight;
		texts->DrawString({ left, h }, settings.settingsText.c_str());

		//h += settings.textLineHeight;
		texts->DrawString({ left, h }, settings.nonEmptyLineText.c_str());
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void TetrisGame::DrawInfo()
	{
		char buff[100];
		sprintf(buff, " %.01f", window->GetSettings().fps);
		window->DrawString(settings.fpsTextPos, buff);

		sprintf(buff, " %d", level);
		window->DrawString(settings.levelTextPos, buff);

		sprintf(buff, " %d", score);
		window->DrawString(settings.scoreTextPos, buff);

		sprintf(buff, " %d", (SDL_GetTicks() - timeStart) / 1000);
		window->DrawString(settings.timeTextPos, buff);

		sprintf(buff, " %d", settings.maxSaves - saves);
		window->DrawString(settings.savesTextPos, buff);

		sprintf(buff, " %d", NonEmptyLine());
		window->DrawString({ settings.savesTextPos.x,  settings.savesTextPos.y+10}, buff);
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void DrawFillRectRot(SdlLibrary::Bitmap* bmp, SdlLibrary::Rect, int angle, SdlLibrary::Point center, int borderSize);

	void TetrisGame::WindowRender(Events::EventArgs * e)
	{
#ifdef SMOKE
		if (activeBlock.rot == 0) 
#endif
		{
			window->DrawBitmap(*background, { 0,0 });
			window->DrawBitmap(*texts, settings.rightColPos);
			DrawInfo();
		}

		int marg = window->GetClientRect().bottom - (settings.boardHeight)*settings.blockSize - settings.margin;

		double xpos = 0, ypos = 0; bool flag1, flag2;
		
		for (int i = 0; i < Block::size; i++) {
			flag2 = flag1 = false;
 			for (int j = 0; j < Block::size; j++) {
				if (!flag1 && activeBlock.shape[i][j]) { flag1 = true; }
				if (!flag2 && activeBlock.shape[j][i]) { flag2 = true; }
			}
			if (flag1)xpos++;
			if (flag2)ypos++;
		}

		xpos = xpos / 2;
		ypos = ypos / 2;
				

		//wyœwietlenie aktywnego klocka
		for (int i = 0; i < Block::size; i++)
			for (int j = 0; j < Block::size; j++)
				if (activeBlock.shape[i][j]) {
					window->SetBrush(SdlLibrary::Brush(activeBlock.color, activeBlock.color));
					/*window->FillRect({ (int)((activeBlock.pos.x + i)*settings.blockSize + settings.margin + activeBlock.rendPos.x),
						(int)((activeBlock.pos.y + j)*settings.blockSize + marg - activeBlock.rendPos.y),
						(int)((activeBlock.pos.x + i + 1)*settings.blockSize - 3 + settings.margin + activeBlock.rendPos.x),
						(int)((activeBlock.pos.y + j + 1)*settings.blockSize - 3 + marg - activeBlock.rendPos.y) });*/
					DrawFillRectRot(window, { (int)((activeBlock.pos.x + i)*settings.blockSize + settings.margin + activeBlock.rendPos.x),
						(int)((activeBlock.pos.y + j)*settings.blockSize + marg - activeBlock.rendPos.y),
						(int)((activeBlock.pos.x + i + 1)*settings.blockSize - 2 + settings.margin + activeBlock.rendPos.x),
						(int)((activeBlock.pos.y + j + 1)*settings.blockSize - 2 + marg - activeBlock.rendPos.y) },
						activeBlock.rot, { (int)((activeBlock.pos.x)*settings.blockSize + settings.margin + activeBlock.rendPos.x + settings.blockSize*xpos),
							(int)((activeBlock.pos.y)*settings.blockSize + marg - activeBlock.rendPos.y + settings.blockSize * ypos) },
						settings.borderSize);
				}

		//wypisanie planszy
		for (int i = 0; i < settings.boardHeight + 1; i++)
			for (int j = 0; j < settings.boardWidth + 1; j++)
				if (board[i][j] != 0) {
					window->SetBrush(SdlLibrary::Brush(board[i][j], board[i][j]));
					window->FillRect({ (int)((j)*settings.blockSize + settings.margin),(int)((i - 1)*settings.blockSize + marg),
						(int)((j + 1)*settings.blockSize - 3 + settings.margin),(int)((i)*settings.blockSize - 3 + marg) });
				}

		//wyœwietlenie pausy
		if (paused) {
			if (gameOver) window->DrawString({ (int)(settings.boardHeight*settings.blockSize / 2), (int)(settings.boardWidth*settings.blockSize / 2) }, "GAME OVER");
			else window->DrawString({ (int)(settings.boardHeight*settings.blockSize / 2), (int)(settings.boardWidth*settings.blockSize / 2) }, "PAUSED");
		}
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	SdlLibrary::Point RotatePoints(SdlLibrary::Point p, double angle, SdlLibrary::Point center) {
		if (angle == 0) return p;
		angle = angle*M_PI / 180;

		int dx, dy;
		dx = p.x - center.x;
		dy = p.y - center.y;
		p.x = center.x + cos(angle)*dx - sin(angle)*dy;
		p.y = center.y + sin(angle)*dx + cos(angle)*dy;
		return p;
	}


	void DrawFillRectRot(SdlLibrary::Bitmap* bmp, SdlLibrary::Rect rect, int angle, SdlLibrary::Point center, int borderSize) {
		for (int i = 0; i < borderSize; i++) {
			bmp->DrawLine(RotatePoints({ rect.right,rect.top }, angle, center), RotatePoints({ rect.right, rect.bottom }, angle, center));
			bmp->DrawLine(RotatePoints({ rect.left,rect.top }, angle, center), RotatePoints({ rect.left, rect.bottom }, angle, center));
			bmp->DrawLine(RotatePoints({ rect.left,rect.top }, angle, center), RotatePoints({ rect.right, rect.top }, angle, center));
			bmp->DrawLine(RotatePoints({ rect.left,rect.bottom }, angle, center), RotatePoints({ rect.right, rect.bottom }, angle, center));

			rect.left++; rect.right--;
			rect.top++; rect.bottom--;
		}
	}
}