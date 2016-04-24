/*G³ówny klasa z wszystkimi podstawowymi funckjami gry
*/
#pragma once
#include "stdafx.h"
#include "Block.h"
#include "TetrisGameSettings.h"

#ifdef TEST
int TEST();
#endif

namespace TetrisGame {
	

	class TetrisGame:Events::Object
	{
#ifdef TEST
		friend int ::TEST();
#endif
	public:
		TetrisGame();
		~TetrisGame();
		void Play();
	private:
		//funkcje do rysowania (za³apa³y siê tutaj na doczepkê)
		void LoadBackground();
		void LoadInfoTexts();
		void DrawInfo();
		void WindowRender(Events::EventArgs* e);

		//akcje klawiszy
		void KeyDown(Events::KeyboardEventArgs* e);
		void MoveBlock(int step);
		void RotateBlock();
		void BlockDown();

		//akcje timerów
		void TimerBlockDown(Events::EventArgs* e);
		void TimerRendBlockMove(Events::EventArgs* e);
		void TimerRendBlockDown(Events::EventArgs* e);
		void TimerNextLevel(Events::EventArgs* e);

		//pomocnicze
		void NewBlock();
		void RewriteActiveBlock();
		void ClearLine();
		unsigned NonEmptyLine();
		void AddScore(unsigned lines);
		void Reset();
		void GameOver();
		void SaveGame();
		void LoadGame();
	private:
		Block activeBlock;
		int **board=nullptr;
		//timery wydarzeñ
		SdlLibrary::Timer timerFullMove;
		SdlLibrary::Timer timerRendMove;
		SdlLibrary::Timer timerRendDown;
		SdlLibrary::Timer timerNextLevel;
		//zmienne pomocnicze
		SdlLibrary::Window* window;
		SdlLibrary::Bitmap* background;
		SdlLibrary::Bitmap* texts;
		Settings settings;
		bool rendering;
		//dane gry
		unsigned score=0;
		bool doubleScore = false;
		unsigned level = 1;
		bool paused = false;
		bool gameOver = false;
		int timeStart = 0;
		bool speedMove = false;
		unsigned saves = 0;
	};
}