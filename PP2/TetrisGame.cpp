#include "TetrisGame.h"
#include "SdlException.h"
#include "ScoreTable.h"
#include <cstdlib>
#include <ctime>

#define RIGHT_ANGLE 90

namespace TetrisGame {

	TetrisGame::TetrisGame()
	{
		window = new SdlLibrary::Window(settings.WindowWidth, settings.WindowHeight, settings.Fullscreen, settings.ShowCursor);
		LoadBackground();
		LoadInfoTexts();
		if (settings.boardWidth <= 0 || settings.boardHeight <= 0)
			throw SdlLibrary::Exception("Board size error");

		board = new int*[settings.boardHeight+1];
		for (int i = 0; i < settings.boardHeight+1; i++) {
			board[i] = new int[settings.boardWidth+1];
		}

		//ustawianie eventów i timerów
		window->RenderStart += Events::MethodHandler(this, (Events::Object::memberFunction)&TetrisGame::WindowRender);
		window->KeyDown += Events::MethodHandler(this, (Events::Object::memberFunction)&TetrisGame::KeyDown);
		timerFullMove.method = Events::MethodHandler(this, (TetrisGame::memberFunction)&TetrisGame::TimerBlockDown);
		timerRendMove.method = Events::MethodHandler(this, (TetrisGame::memberFunction)&TetrisGame::TimerRendBlockMove);
		timerRendDown.method = Events::MethodHandler(this, (TetrisGame::memberFunction)&TetrisGame::TimerRendBlockDown);
		timerNextLevel.method = Events::MethodHandler(this, (TetrisGame::memberFunction)&TetrisGame::TimerNextLevel);
		timerFullMove.interval = settings.blockDownTime;
		timerRendMove.interval = settings.blockMoveTime;
		timerRendDown.interval = settings.blockDownTime / settings.blockSize;
		timerNextLevel.interval = settings.nextLevelTimer;
		
		window->timers += timerFullMove;
		window->timers += timerRendMove;
		window->timers += timerRendDown;
		window->timers += timerNextLevel;

		//ustawienie ziarna dla losowania liczb
		srand(time(NULL));

		Reset();
	}
	//------------------------------------------------------------------------------------------------------------------------------------------

	TetrisGame::~TetrisGame()
	{
		if (window != nullptr)
			delete window;
		if (background != nullptr)
			delete background;
		if (texts != nullptr)
			delete texts;
		if (board != nullptr) {
			for (int i = 0; i < settings.boardHeight + 1; i++) {
				delete[] board[i];
			}
			delete[] board;
		}
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void TetrisGame::Play()
	{
		rendering = true;
		window->timers.Start();

		while (rendering) {
			window->DoAll();
			if (gameOver || paused) _sleep(10);
		}
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void TetrisGame::KeyDown(Events::KeyboardEventArgs * e)
	{
		KeysConfig key;
		for (int i = 0; i < settings.keysCount;i++)
			if (settings.keys[i].key == e->vKeyCode) {
				key = settings.keys[i];
				break;
			}

		switch (key.type) {
		case KeysConfig::Type::Escape: rendering = false; break;
		case KeysConfig::Type::NewGame: Reset(); break;
		case KeysConfig::Type::NextLevel: if (!paused)TimerNextLevel(e); break;
		case KeysConfig::Type::Left:if (!paused)MoveBlock(-1); break;
		case KeysConfig::Type::Right:if (!paused)MoveBlock(1); break;
		case KeysConfig::Type::Rotate:if (!paused)RotateBlock(); break;
		case KeysConfig::Type::GoDown:if(!paused)BlockDown(); break;
		case KeysConfig::Type::Pause: if(!gameOver) {paused = !paused;  window->timers.Pause();} break;
		case KeysConfig::Type::SaveGame: if (!gameOver) SaveGame(); break;
		case KeysConfig::Type::LoadGame:LoadGame(); break;
		}
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void TetrisGame::MoveBlock(int step)
	{
		SdlLibrary::Point pos = activeBlock.pos, rendPos=activeBlock.rendPos;
		if (step < 0) { pos.x--; rendPos.x += settings.blockSize; }
		else { pos.x++; rendPos.x -= settings.blockSize;}

		for (int i = 0; i < Block::size; i++)
			for (int j = 0; j < Block::size; j++) {
				if (activeBlock.shape[i][j] && !(pos.x+i < settings.boardWidth && pos.x+i>=0 && board[pos.y + j][pos.x + i] == 0)) {
					return;
				}
			}
		activeBlock.pos = pos;
		activeBlock.rendPos = rendPos;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void TetrisGame::RotateBlock()
	{
		//TODO obrót wokó³ œrodka ciê¿koœci
		bool tmp[Block::size][Block::size];
		bool emptyLine[Block::size];
		SdlLibrary::Point pos = activeBlock.pos;
		memset(emptyLine, false, sizeof(bool)*Block::size);

		for (int i = 0; i < Block::size; i++)//obrót wokó³ œrodka siatki
			for (int j = 0; j < Block::size; j++)
				tmp[j][Block::size - i - 1] = activeBlock.shape[i][j];

		double xpos = 0, ypos = 0; bool flag1, flag2; int xsize = 0, ysize = 0;

		for (int i = 0; i < Block::size; i++) {
			flag2 = flag1 = false;
			for (int j = 0; j < Block::size; j++) {
				if (!flag1 && tmp[i][j]) { flag1 = true; }
				if (!flag2 && tmp[j][i]) { flag2 = true; }
			}
			if (!flag1)ypos++;
			if (!flag2)xpos++;
		}

		for (int i = 0; i < Block::size; i++) {
			flag2 = flag1 = false;
			for (int j = 0; j < Block::size; j++) {
				if (!flag1 && activeBlock.shape[i][j]) { flag1 = true; }
				if (!flag2 && activeBlock.shape[j][i]) { flag2 = true; }
			}
			if (flag1)xsize++;
			if (flag2)ysize++;
		}

		//przesuniêcie do lewej górnej krawêdzi
		bool flag = false; int count = 0;
		do {
			flag = true;
			//dosuniêcie do lewej
			for (int i = 0; i < Block::size; i++)
				if (tmp[i][0]) { flag = false; break; }
			if (flag)for (int i = 1; i < Block::size;i++)for (int j = 0; j < Block::size; j++) {
					tmp[j][i-1] = tmp[j][i]; tmp[j][i] = false;
				}
			count++;
		} while (flag && count < Block::size);

		do {
			flag = true;
			//dosuniêcie do góry
			for (int i = 0; i < Block::size; i++)
				if (tmp[0][i]) { flag = false; break; }
			if (flag)for (int i = 1; i < Block::size; i++)for (int j = 0; j < Block::size; j++) {
						tmp[i - 1][j] = tmp[i][j]; tmp[i][j] = false;
					}
			count++;
		} while (flag && count < Block::size);


		for (int i = 0; i < Block::size; i++)//sprawadzenie, czy obrót jest mo¿liwy
			for (int j = 0; j < Block::size; j++) {
				if (tmp[i][j] && ((pos.y + j>settings.boardHeight || board[pos.y + j][pos.x + i] != 0) || !(pos.x + i < settings.boardWidth && pos.x + i >= 0 && board[pos.y + j][pos.x + i] == 0)))
					return;
			}

		activeBlock.rendPos.x -= (ysize - xsize) * settings.blockSize;
		activeBlock.rendPos.y -= (ysize - xsize) * settings.blockSize;

		activeBlock.rot += RIGHT_ANGLE;
		for (int i = 0; i < Block::size; i++) {//kopiowanie
			memcpy(activeBlock.shape[i], tmp[i], sizeof(bool)*Block::size);
		}
		

	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void TetrisGame::BlockDown()
	{
		activeBlock.rendPos.y=0;
		Events::EventArgs e;
		e.sender = this;
		while (!e.Cancel)
			TimerBlockDown(&e);
		speedMove = true;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void TetrisGame::TimerBlockDown(Events::EventArgs* e)
	{
		SdlLibrary::Point pos = activeBlock.pos;pos.y++;//nowa pozycja klocka
		//sprzawdzenie, czy przesuniêcie jest mo¿liwe
		for (int i = 0; i < Block::size; i++)
			for (int j = 0; j < Block::size; j++) {
				if (activeBlock.shape[i][j] && (pos.y+j>settings.boardHeight || board[pos.y+j][pos.x + i]!=0)) {
					if (e->sender != this) {//jeœli nie, a nie wywo³ano z BlockDown to przepisz do board i utwórz nowy klocek
						RewriteActiveBlock();
						ClearLine();
						NewBlock();
					}
					e->Cancel = true;//zatrzymaj dalsze wykonywanie
					return;
				}
			}
		//przypisz now¹ pozyjê do aktywnego klocka(jest ona mo¿liwa)
		activeBlock.rendPos.y += settings.blockSize;
		activeBlock.pos = pos;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void TetrisGame::TimerRendBlockMove(Events::EventArgs * e)
	{
		if (activeBlock.rendPos.x != 0)
			if (activeBlock.rendPos.x > 0) activeBlock.rendPos.x -= (activeBlock.rendPos.x/settings.blockSize+1);
			else activeBlock.rendPos.x -= (activeBlock.rendPos.x / (int)settings.blockSize)-1;
			if (activeBlock.rot != 0) activeBlock.rot -= activeBlock.rot/RIGHT_ANGLE + 1;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void TetrisGame::TimerRendBlockDown(Events::EventArgs * e) {
		if (activeBlock.rendPos.y > 0)
			activeBlock.rendPos.y-= (activeBlock.rendPos.y/settings.blockSize*(speedMove?settings.speedMove:1));
		else if (activeBlock.rendPos.y != 0) activeBlock.rendPos.y++;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void TetrisGame::TimerNextLevel(Events::EventArgs * e)
	{
		timerNextLevel.Stop();
		if (level < settings.maxLevel) {
			timerFullMove.interval *= settings.levelFaster;
			timerRendDown.interval = timerFullMove.interval / settings.blockSize-1;
			level++;
			timerNextLevel.Start();
		}
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void TetrisGame::NewBlock()
	{
		activeBlock = ReadyShapes()[rand() % ReadyShapesCount()];
		activeBlock.pos = { (int)settings.boardWidth / 2 - (int)Block::size / 2, 0 };
		activeBlock.rendPos = { 0,0 };
		bool cancel = false;
		//sprawdzanie, czy mo¿na postawiæ nowy klocek
		for (int i = settings.boardWidth/2 - Block::size/2; i < settings.boardWidth / 2 + Block::size / 2; i++){
			for (int j = 0; j < Block::size; j++) {
				if (board[j][i] != 0 && activeBlock.shape[i - settings.boardWidth / 2][j]) {
					cancel = true;
					break;
				}
			}
		}
		speedMove = false;
		
		if (cancel) {
			paused = true;
			gameOver = true;
			GameOver();
		}
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void TetrisGame::RewriteActiveBlock()
	{
		SdlLibrary::Point pos = activeBlock.pos;
		for (int i = 0; i < Block::size; i++)
			for (int j = 0; j < Block::size; j++) {
				if (activeBlock.shape[i][j]) {
					board[pos.y + j][pos.x + i] = activeBlock.color;
				}
			}
	}
	//============================================================================================================================================

	unsigned TetrisGame::NonEmptyLine()
	{
		bool flag;
		int lines = 0;
		for (int i = settings.boardHeight; i >= 0; i--) {
			flag = true;
			//sprawdzanie, czy linia jest pe³na
			for (int j = 0; j < settings.boardWidth; j++) {
				if (board[i][j] != 0) {
					flag = false;
					break;
				}
			}

			//przenoszenie o jeden wiersz w dó³
			if (flag) {
				lines++;
			}
		}
		return settings.boardHeight-lines+1;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void TetrisGame::ClearLine()
	{
		bool flag;
		int lines = 0;
		for (int i = settings.boardHeight; i >= 0; i--) {
			flag = true;
			//sprawdzanie, czy linia jest pe³na
			for (int j = 0; j < settings.boardWidth; j++) {
				if (board[i][j] == 0) {
					flag = false;
					break;
				}
			}

			//przenoszenie o jeden wiersz w dó³
			if (flag) {
				lines++;
				for (int j = i - 1; j >= 0; j--)
					memcpy(board[j + 1], board[j], sizeof(SdlLibrary::Color)*settings.boardWidth);

				i++;
			}
		}
		AddScore(lines);
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void TetrisGame::AddScore(unsigned lines)
	{
		if (lines <= 0) return;
		if (lines < settings.maxPointedLines) {
			score += (level + 1)*settings.oneLinePoints*pow(2, lines - 1);
			doubleScore = false;
		}
		else {
			if (doubleScore)
				score += settings.maxLinesPointDouble*(level + 1);
			else score += settings.maxLinesPoint*(level + 1);
			doubleScore = true;
		}
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void TetrisGame::Reset()
	{
		for (int i = 0; i < settings.boardHeight + 1; i++)
			memset(board[i], 0, sizeof(int)*(settings.boardWidth + 1));
		paused = false;
		gameOver = false;
		doubleScore = false;
		score = 0;
		level = 0;
		timeStart = SDL_GetTicks();
		timerFullMove.interval = settings.blockDownTime;
		timerRendDown.interval = settings.blockDownTime / settings.blockSize-1;
		window->timers.Start();
		NewBlock();
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void TetrisGame::GameOver()
	{
		window->timers.Stop();
		ScoreTable scoreTable(window);
		scoreTable.Show(score);
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void TetrisGame::SaveGame()
	{
		if (gameOver || saves>=settings.maxSaves) return;
		saves++;
		FILE* file = fopen("save.txt", "w");
		if (file) {
			fprintf(file, "%d\n%d\n%d\n%d\n%d\n", score, level, doubleScore, SDL_GetTicks() - timeStart, saves);
			for (int i = 0; i < settings.boardHeight + 1; i++)
				for (int j = 0; j < settings.boardWidth + 1; j++)
					fprintf(file, "%X ", board[i][j]);
			fprintf(file, "\n");
			for (int i = 0; i < Block::size; i++)
				for (int j = 0; j < Block::size; j++)
					fprintf(file, "%d ", activeBlock.shape[i][j]);
			fprintf(file, "\n%X %X %X", activeBlock.pos.x, activeBlock.pos.y, activeBlock.color);
			fclose(file);
		}
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void TetrisGame::LoadGame()
	{
		Reset();
		//TODO dodaæ sprawdzanie poprawnoœci
		FILE* file = fopen("save.txt", "r");
		if (file) {
			int tmp=0;
			fscanf(file, "%d\n%d\n%d\n%d\n%d\n", &score, &level, &doubleScore, &tmp, &saves);
			timeStart = SDL_GetTicks() - tmp;
			timerFullMove.interval = settings.blockDownTime * pow(settings.levelFaster, level);
			timerRendDown.interval = timerFullMove.interval / settings.blockSize;

			for (int i = 0; i < settings.boardHeight + 1; i++)
				for (int j = 0; j < settings.boardWidth + 1; j++) {
					fscanf(file, "%X ", &tmp);
					board[i][j] = tmp;
				}
			fscanf(file, "\n");
			for (int i = 0; i < Block::size; i++)
				for (int j = 0; j < Block::size; j++) {
					fscanf(file, "%d ", &tmp);
					activeBlock.shape[i][j] = tmp;
				}
			fscanf(file, "\n%X %X %X", &activeBlock.pos.x, &activeBlock.pos.y, &activeBlock.color);
			fclose(file);
		}
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
}