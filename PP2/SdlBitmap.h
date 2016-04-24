/*Klasa pozwalaj�ca rysowa� i przechowywa� bitmapy w pami�ci komputera
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

		bool LoadCharset(NonSTL::string file);//za�adowanie zestawu znak�w
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
		void Clear();//Wype�nia kolorem czarnym
		void SetBrush(Brush brush);//ustaw p�dzel
		Rect GetClientRect();//pobiera wymiary przestrzeni w kt�rej mo�na rysowa�
	protected:
	//public:
		SDL_Surface *pict=nullptr;
	protected:
		Brush brush;
		SDL_Surface* charset=nullptr;
		static SDL_Surface* staticCharset;

		const double circleDrawStep = 0.0001;//zmienna pomocnicza przy rysowaniu okr�gu (k�t w radianach)
	};
}