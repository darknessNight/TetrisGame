/*G³ówna klasa obs³uguj¹ca SDL pozwala na utworzenie okna w którym bêdzie renderowany obraz
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

		void ToogleCursor();//prze³¹czanie widocznoœci cursora myszki
		void SetWindowTitle(NonSTL::string title);

		const Settings GetSettings();

		bool Load(NonSTL::string) = delete;
		bool Render();
		void Events();

		void DoAll();//wykonaj wszystkie zdarzenia, timery i renderuj
	private:
		//zestaw metod przygotowuj¹cych struktury dla zdarzeñ
		void OnKeyDown(SDL_KeyboardEvent &e);
		void OnKeyUp(SDL_KeyboardEvent &e);
		void OnMouseButtonDown(SDL_MouseButtonEvent &e);
		void OnMouseButtonUp(SDL_MouseButtonEvent &e);
		void OnMouseWheel(SDL_MouseWheelEvent &e);
		void OnMouseMove(SDL_MouseMotionEvent &e);
		void OnClose(SDL_QuitEvent &e);
		void OnRender();
	public:
		//zdarzenia do których mo¿na podpi¹c zewnêtrzne funkcje
		Events::EventHandler<Events::MethodHandler, Events::KeyboardEventArgs&> KeyDown;
		Events::EventHandler<Events::MethodHandler, Events::KeyboardEventArgs&> KeyUp;
		Events::EventHandler<Events::MethodHandler, Events::MouseEventArgs&> MouseDown;
		Events::EventHandler<Events::MethodHandler, Events::MouseEventArgs&> MouseUp;
		Events::EventHandler<Events::MethodHandler, Events::MouseEventArgs&> MouseMove;
		Events::EventHandler<Events::MethodHandler, Events::MouseEventArgs&> MouseWheel;
		Events::EventHandler<Events::MethodHandler, Events::EventArgs&> RenderStart;
		Events::EventHandler<Events::MethodHandler, Events::EventArgs&> Close;

		FpsHelper fpsHelper;//klasa pomocnicza do liczenia FPS-ów
		Timers timers;//zestaw timerów, które maj¹ siê wykonywaæ w trakcie dzia³ania programu
	private:
		//zmienne pomocnicze
		SDL_Event event;
		//using Bitmap::pict;
		SDL_Texture *scrtex;
		SDL_Window *window;
		SDL_Renderer *renderer;

		//ustawienia ogólne okna
		Settings settings;
	};

}