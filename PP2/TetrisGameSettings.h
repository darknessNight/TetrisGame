/*Ustawienia dla obiektów gry
*/
#pragma once
#include "SdlClass.h"

namespace TetrisGame {
	struct KeysConfig {
		enum Type {
			Unknow,
			Escape,
			Right,
			Left,
			Rotate,
			GoDown,
			NewGame,
			NextLevel,
			Pause,
			SaveGame,
			LoadGame
		};
		KeysConfig(Type t, Events::KeyboardEventArgs::VKeyCode k) {
			type = t;
			key = k;
		}
		KeysConfig() = default;
		Type type = Unknow;
		Events::KeyboardEventArgs::VKeyCode key;
	};
	//================================================================================================================================================
	struct Settings {
		const unsigned speedMove = 10;
		const unsigned WindowWidth = 800;
		const unsigned WindowHeight = 640;
		const bool Fullscreen = false;
		const bool ShowCursor = false;
		//ustawawienia rysowania t³a
		static const unsigned margin = 20,
			rightColW = 200,
			rightColH = 400,
			menuBackColor = 0xFF003F00,
			boardBackColor = 0xFF000000,
			borderColor = SdlLibrary::Color::Red,
			borderSize = 2,
			textLineHeight = 10,
			charWidth = 8;
		SdlLibrary::Point rightColPos;
		SdlLibrary::Brush::Type borderStyle = SdlLibrary::Brush::Type::Dashed;

		//wielkoœæ planszy
		const unsigned boardWidth = 20;
		unsigned boardHeight = 20;
		unsigned blockSize = 10;

		//czasy timerów (w milisekundach)
		const unsigned blockDownTime = 1000;
		const unsigned blockMoveTime = 1;
		const unsigned nextLevelTimer = 20000;

		//poziomy
		const double levelFaster = 0.7;
		const unsigned maxLevel = 10;
		//punkty
		const unsigned oneLinePoints = 100;
		const unsigned maxPointedLines = 4;
		const unsigned maxLinesPoint = 800;
		const unsigned maxLinesPointDouble = 1200;

		const unsigned maxSaves = 3;

		//texty wypisywane w bocznej ramce
		SdlLibrary::Point fpsTextPos;
		SdlLibrary::Point levelTextPos;
		SdlLibrary::Point scoreTextPos;
		SdlLibrary::Point timeTextPos;
		SdlLibrary::Point savesTextPos;
		SdlLibrary::Point nonEmptyLineTextPos;
		NonSTL::string fpsText = "FPS:  ";
		NonSTL::string scoreText = "Wynik:  ";
		NonSTL::string levelText = "Poziom:  ";
		NonSTL::string timeText = "Czas:  ";
		NonSTL::string savesText = "Pozosta³e zapisy:  ";
		NonSTL::string settingsText = "\n\nSterowanie:\nPrawo/Lewo - ruch\nDó³ - szybki upadek\nSpacje/Góra\n - rotacja\n"
			"S - zapis\nL - ³adownie zapisu\nF - nastêpny poziom\nN - nowa gra\nP - pausa\nEscape - wyjœcie";
		NonSTL::string nonEmptyLineText = "Niepuste linie";

		//ustawienia dla listy wyników
		static const int scoresListSize = 20;
		static const int nameMaxSize = 20;
		static NonSTL::string getNameText() {return "Twój wynik jest niesamowity podaj swoje imiê:\n(max 20 znaków)";}

		//klawisze
		static const unsigned keysCount = 12;
		KeysConfig keys[keysCount];
		Settings() {
			KeysConfig tmp[keysCount] = {
				{ KeysConfig::Type::Escape,Events::KeyboardEventArgs::VKeyCode::ESCAPE },
				{ KeysConfig::Type::Left,Events::KeyboardEventArgs::VKeyCode::LEFT },
				{ KeysConfig::Type::Right,Events::KeyboardEventArgs::VKeyCode::RIGHT },
				{ KeysConfig::Type::Rotate,Events::KeyboardEventArgs::VKeyCode::UP },
				{ KeysConfig::Type::Rotate,Events::KeyboardEventArgs::VKeyCode::SPACE },
				{ KeysConfig::Type::NewGame,Events::KeyboardEventArgs::VKeyCode::KEY_N },
				{ KeysConfig::Type::GoDown,Events::KeyboardEventArgs::VKeyCode::DOWN },
				{ KeysConfig::Type::NextLevel,Events::KeyboardEventArgs::VKeyCode::KEY_F },
				{ KeysConfig::Type::Pause,Events::KeyboardEventArgs::VKeyCode::KEY_P },
				{ KeysConfig::Type::SaveGame,Events::KeyboardEventArgs::VKeyCode::KEY_S },
				{ KeysConfig::Type::LoadGame,Events::KeyboardEventArgs::VKeyCode::KEY_L },
			};

			memcpy(keys, tmp, keysCount*sizeof(KeysConfig));
		}
	};
}
