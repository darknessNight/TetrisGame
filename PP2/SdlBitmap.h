/*Klasa pozwalaj¹ca rysowaæ i przechowywaæ bitmapy w pamiêci komputera
*/
#pragma once
#include "HelperClasses.h"

namespace SdlLibrary {
	class Bitmap {
	public:
		Bitmap(NonSTL::string file);
		Bitmap();
		Bitmap(unsigned width, unsigned height);
		~Bitmap();
		bool Load(NonSTL::string file);

		bool LoadCharset(NonSTL::string file);//za³adowanie zestawu znaków
		bool DrawString(Point p, const char *text);
		bool DrawBitmap(Bitmap &sprite, Point p);
		void DrawPixel(Point p, Color color);
		void DrawPixel(Point p);
		void DrawLine(Point p1, Point p2, int size=0, bool foreColor=true);
		void DrawPoint(Point p1);
		void DrawRect(Rect rect);
		void FillRect(Rect, bool foreColor = false);
		void DrawCircle(Point, int radius);
		void FillCircle(Point, int radius, bool foreColor = false);
		void Clear();//Wype³nia kolorem czarnym
		void SetBrush(Brush brush);//ustaw pêdzel
		Rect GetClientRect();//pobiera wymiary przestrzeni w której mo¿na rysowaæ
	protected:
	//public:
		SDL_Surface *pict=nullptr;
	protected:
		Brush brush;
		SDL_Surface* charset=nullptr;
		static SDL_Surface* staticCharset;

		const double circleDrawStep = 0.0001;//zmienna pomocnicza przy rysowaniu okrêgu (k¹t w radianach)
	};
}