/*G��wna klasa obs�uguj�ca SDL pozwala na utworzenie okna w kt�rym b�dzie renderowany obraz
*/
#pragma once
#include "SdlBitmap.h"
#include "Libraries/EventHandlers.h"

namespace SdlLibrary {
	class Window :public Bitmap
	{
	public:
		Window(int width = 800, int height = 640, bool fullscreen = false, bool showCursor = false, NonSTL::string charset = NonSTL::string("cs8x8.bmp"));
		~Window();

		void ToogleCursor();//prze��czanie widoczno�ci cursora myszki
		void SetWindowTitle(NonSTL::string title);

		const Settings GetSettings();

		bool Load(NonSTL::string) = delete;
		bool Render();
		void Events();

		void DoAll();//wykonaj wszystkie zdarzenia, timery i renderuj
	private:
		//zestaw metod przygotowuj�cych struktury dla zdarze�
		void OnKeyDown(SDL_KeyboardEvent &e);
		void OnKeyUp(SDL_KeyboardEvent &e);
		void OnMouseButtonDown(SDL_MouseButtonEvent &e);
		void OnMouseButtonUp(SDL_MouseButtonEvent &e);
		void OnMouseWheel(SDL_MouseWheelEvent &e);
		void OnMouseMove(SDL_MouseMotionEvent &e);
		void OnClose(SDL_QuitEvent &e);
		void OnRender();
	public:
		//zdarzenia do kt�rych mo�na podpi�c zewn�trzne funkcje
		Events::EventHandler<Events::MethodHandler, Events::KeyboardEventArgs&> KeyDown;
		Events::EventHandler<Events::MethodHandler, Events::KeyboardEventArgs&> KeyUp;
		Events::EventHandler<Events::MethodHandler, Events::MouseEventArgs&> MouseDown;
		Events::EventHandler<Events::MethodHandler, Events::MouseEventArgs&> MouseUp;
		Events::EventHandler<Events::MethodHandler, Events::MouseEventArgs&> MouseMove;
		Events::EventHandler<Events::MethodHandler, Events::MouseEventArgs&> MouseWheel;
		Events::EventHandler<Events::MethodHandler, Events::EventArgs&> RenderStart;
		Events::EventHandler<Events::MethodHandler, Events::EventArgs&> Close;

		FpsHelper fpsHelper;//klasa pomocnicza do liczenia FPS-�w
		Timers timers;//zestaw timer�w, kt�re maj� si� wykonywa� w trakcie dzia�ania programu
	private:
		//zmienne pomocnicze
		SDL_Event event;
		//using Bitmap::pict;
		SDL_Texture *scrtex;
		SDL_Window *window;
		SDL_Renderer *renderer;

		//ustawienia og�lne okna
		Settings settings;
	};

}