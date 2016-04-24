#include "ScoreTable.h"
#include "Libraries\stringNSTL.hpp"


namespace TetrisGame {
	ScoreTable::ScoreTable(SdlLibrary::Window* window):keyHandl(this,(ScoreTable::memberFunction)&ScoreTable::KeyDown),
		rendHandl(this, (ScoreTable::memberFunction)&ScoreTable::WindowRender)
	{
		this->window = window;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------

	ScoreTable::~ScoreTable()
	{
		window->RenderStart -= rendHandl;
		window->KeyDown -= keyHandl;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void ScoreTable::Show(int score)
	{
		window->RenderStart += rendHandl;
		window->KeyDown += keyHandl;
		LoadHighscores();
		AddHighscores(score);
		ViewHighscores();
		inputLoop = true;
		while (inputLoop) {//podtrzymywanie ekranu wyœwietlania
			window->Events();
			_sleep(10);
		}
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void ScoreTable::KeyDown(Events::KeyboardEventArgs * e)
	{
		switch (e->vKeyCode) {
		case Events::KeyboardEventArgs::VKeyCode::ENTER:
			if(current.name.size()>0)
				inputLoop = false;
			break;
		case Events::KeyboardEventArgs::VKeyCode::ESCAPE:
			current.name = "";
			inputLoop = false;
			break;
		case Events::KeyboardEventArgs::VKeyCode::BACKSPACE:
			current.name.pop_back(); current.name.pop_back(); current.name.push_back('\0');
			break;
		default: if ((e->vKeyCode >= Events::KeyboardEventArgs::VKeyCode::KEY_A && e->vKeyCode <= Events::KeyboardEventArgs::VKeyCode::KEY_0 
			&& current.name.size()<=Settings::nameMaxSize) || e->vKeyCode== Events::KeyboardEventArgs::VKeyCode::SPACE)
			current.name += e->keyCode +(e->sysKeys&e->Shift?-32:0);
		}
		e->Cancel = true;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void ScoreTable::WindowRender(Events::EventArgs * e)
	{
		if (inputLoop) {
			window->Clear();
			window->SetBrush(SdlLibrary::Brush(SdlLibrary::Color::Purple, SdlLibrary::Color::Gray));
			SdlLibrary::Rect rect = window->GetClientRect();
			rect.top = rect.bottom / 3;
			rect.bottom -= rect.top;
			rect.left = rect.right / 4;
			rect.right -= rect.left;
			window->FillRect(rect);
			window->DrawString({ rect.left + (int)Settings::margin, rect.top + (int)Settings::margin }, Settings::getNameText().c_str());
			window->DrawString({ rect.right / 2 , rect.top + (rect.bottom - rect.top) / 2 }, current.name.c_str());
		}
		e->Cancel = true;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void ScoreTable::LoadHighscores()
	{
		FILE* file = fopen("highscores.txt", "r");
		if (!file) file = fopen("highscores.txt", "w");
		if (!file) throw std::exception("Cannot open file");

		int i = 0,tmp=0, ret;
		char buff[100];
		for (; i < Settings::scoresListSize; i++) {
			if(fscanf(file, "%s %d\n", buff, &tmp)<0) break;
			list[i].name = buff;
			list[i].score = tmp;
		}
		fclose(file);

		for (; i < Settings::scoresListSize; i++) {
			list[i].name = "";
			list[i].score = 0;
		}
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void ScoreTable::AddHighscores(int score)
	{
		//wyszukiwanie pozycji rankigowej
		int pos = -1;
		for (int i = 0; i < Settings::scoresListSize; i++) {
			if (score > list[i].score) { pos = i; break; }
		}
		current.score = score;

		if (pos >= 0) {
			inputLoop = true;
			while (inputLoop) {
				window->Events();
				window->Render();
			}

			if (current.name.size()>0) {//jeœli jest nazwa to dopisz j¹ do listy
				FILE* file = fopen("highscores.txt", "w");
				if (!file) throw std::exception("Cannot open file");

				char buff[100];
				int i = 0;
				for (; i < pos; i++) //wypisanie pozycji przed
					if(list[i].name.size()>0)
						fprintf(file, "%s %d\n", list[i].name.c_str(), list[i].score);

				fprintf(file, "%s %d\n", current.name.c_str(), current.score);//obecny wynik
				
				for (; i < Settings::scoresListSize-1;i++)//wypisanie pozycji za
					if (list[i].name.size()>0)
						fprintf(file, "%s %d\n", list[i].name.c_str(), list[i].score);
				
				fclose(file);

				LoadHighscores();
			}
		}
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
	void ScoreTable::ViewHighscores()
	{
		//rysowanie t³a
		window->Clear();
		window->SetBrush(SdlLibrary::Brush(SdlLibrary::Color::Purple, SdlLibrary::Color::Gray));
		SdlLibrary::Rect rect = window->GetClientRect();
		rect.top = rect.bottom / 16;
		rect.bottom -= rect.top;
		rect.left = rect.right / 4;
		rect.right -= rect.left;
		window->FillRect(rect);

		//wypisanie listy wyników
		char buff[1000];
		for (int i = 0; i < Settings::scoresListSize; i++) {
			if (list[i].name.size()>0)
				sprintf(buff, "%d . %s : %d", (i + 1), list[i].name.c_str(), list[i].score);
			else sprintf(buff, "%d . -------- : ---------", (i + 1));
			window->DrawString({ (int)(rect.left + Settings::margin), (int)(rect.top + Settings::margin + Settings::textLineHeight*i) }, buff);
		}


		window->Render();
	}
	//------------------------------------------------------------------------------------------------------------------------------------------
}