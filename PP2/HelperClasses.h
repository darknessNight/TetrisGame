/*Zestaw klas i struktur pomocniczych do ob³usgi SDL
*/
#pragma once
#include "stdafx.h"

#include"../sdl/include/SDL.h"
#include"../sdl/include/SDL_main.h"
#include "Libraries/EventHandlers.h"
#include "Libraries/stringNSTL.h"

namespace SdlLibrary {
	class Bitmap;
	class Window;

	//=======================================Struktury pomocnicze==============================================================================
	//-----------------------------------------------------------------------------------------------------------------------------------------
	
	struct Point {
		int x, y;
	};
	//-----------------------------------------------------------------------------------------------------------------------------------------
	
	struct Rect {
		int left, top, right, bottom;
	};
	//-----------------------------------------------------------------------------------------------------------------------------------------

	struct Settings {
		bool showCursor = true;
		NonSTL::string windowTitle = "";
		unsigned startTime = 0;
		unsigned lastRendTime = 0;
		double fps = 0;
		unsigned frames = 0;
		Rect clientRect;
	};
	//-----------------------------------------------------------------------------------------------------------------------------------------
	//===========================================Klasy pomocnicze==============================================================================

	class Color {
	public:
		enum Names {//Nazwy kolorów
			Black = 0xFF000000U,
			White = 0xFFFFFFFFU,
			Green = 0xFF008000U,
			Blue = 0xFF0000FFU,
			Red = 0xFFFF0000U,
			Silver = 0xFFC0C0C0U,
			Gray = 0xFF808080U,
			Maroon = 0xFF800000U,
			Purple = 0xFF800080U,
			Fuchsia = 0xFFFF00FFU,
			Lime = 0xFF00FF00U,
			Olive = 0xFF808000U,
			Yellow = 0xFFFFFF00U,
			Navy = 0xFF000080U,
			Teal = 0xFF008080U,
			Aqua = 0xFF00FFFFU,
			Transparent = 0x000000U
		};
		operator long int();
		void operator=(Color&);
		void operator=(unsigned c);

		Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a=0xFF);
		Color(long int hex);
		Color(Names);
		Color();

	private:
		friend Window;
		static void SetFormat(SDL_PixelFormat*);//ustawia format kolorów
		void Prepare(unsigned int, unsigned int, unsigned int, unsigned int a=0xFF000000);//rzutowania z systemu ARGB na system u¿ywany przez SDL
	private:
		long int prepared=0;
		static SDL_PixelFormat* format;
	};
	//-----------------------------------------------------------------------------------------------------------------------------------------
	
	class Brush {
	public:
		enum Type {
			Solid,
			Dotted,
			Dashed
		};

		Brush(Color fore, Color fill, Type type = Type::Solid, int size = 1);
		void operator=(Brush &el);
		
	private:
		friend Bitmap;
		bool lineDraw(int i);//informacja dla Bitmap::DrawLine czy powinno rysowaæ w danym momencie, czy nie, by uzyskaæ porz¹dany efekt wygl¹du linii
	private:
		int size;
		Type type;
		Color color;
		Color fillColor;
		const int dotLenght = 3;
		const int lineLenght = 10;
	};
	//-----------------------------------------------------------------------------------------------------------------------------------------

	class FpsHelper {
		int frames = 0;//liczba klatek od ostatniego obliczenia
		int lastRend = 0;//czas ostatniego obliczenia
		double lastFps = 0;//ostatnia obliczona wartoœæ
	public:
		unsigned interval = 333;//co ile ma aktualizowaæ
		double operator()();//obliczenie i zwrócenie liczby FPS
	};
	//-----------------------------------------------------------------------------------------------------------------------------------------

	class Timer{
	public:
		Timer(Events::MethodHandler mh) {method = mh;function = nullptr;}
		Timer(void(*func)()) {function = func;}
		Timer(){function = nullptr;}
		void operator()();//wykonanie dzia³ania, jeœli min¹³ czas
		void Start() {lastExecute = SDL_GetTicks();active = true;}
		void Stop() {active = false;}
		void Pause();//wstrzymuje/odnawia odliczanie
		bool operator==(Timer &t);
	public:
		unsigned interval = 100;//czas pomiêdzy wykonaniami akcji
		void(*function)() = nullptr;
		Events::MethodHandler method;
	private:
		bool active = false;
		int lastExecute=0;//czas ostatniego wykonania (lub czas od ostatniego wykonania przed wstrzymaniem)
	};
	//-----------------------------------------------------------------------------------------------------------------------------------------
	
	class Timers {
	public:
		void operator()();
		void operator+=(Timer &t) {if (search(&t) == -1) timers.push_back(&t);}
		void operator-=(Timer &t) { timers.remove(&t); }
		void Pause();
		void Start();
		void Stop();
	private:
		int search(Timer*);
		NonSTL::vector<Timer*> timers;
	};
}