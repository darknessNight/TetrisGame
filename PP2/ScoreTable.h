/*Klasa wyœwietlaj¹ca lista najelepszy wyników na koñcu gry
*/
#pragma once
#include "SdlClass.h"
#include "TetrisGameSettings.h"

namespace TetrisGame{
	struct Score {
		int score;
		NonSTL::string name;
	};

	class ScoreTable : Events::Object
	{
	public:
		ScoreTable(SdlLibrary::Window* window);
		~ScoreTable();
		void Show(int score);
	private:
		void KeyDown(Events::KeyboardEventArgs* e);
		void WindowRender(Events::EventArgs* e);
		void LoadHighscores();
		void AddHighscores(int score);
		void ViewHighscores();
	private:
		Events::MethodHandler keyHandl;
		Events::MethodHandler rendHandl;
		SdlLibrary::Window* window;
		bool inputLoop = false;

		Score current;
		Score list[Settings::scoresListSize];//lista wyników
	};

}