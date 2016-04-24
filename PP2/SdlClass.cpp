#include "SdlClass.h"
#include "SdlException.h"


namespace SdlLibrary {
	SDL_PixelFormat* Color::format;
	//-----------------------------------------------------------------------------------------------------------------------------------------
	Window::Window(int width, int height, bool fullscreen, bool showCursor, NonSTL::string charset)
	{
		//³adowanie SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			throw Exception("Init error");
		}

		if (SDL_CreateWindowAndRenderer(width, height, (fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0), &window, &renderer) != 0) {
			SDL_Quit();
			throw Exception("Init error");
		}

		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
		SDL_RenderSetLogicalSize(renderer, width, height);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		//tworzenie powierzchni
		pict = SDL_CreateRGBSurface(0, width, height, 32,
			0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

		scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING, width, height);

		if (pict == nullptr && scrtex == nullptr)
			throw Exception("Nullpointer exception");


		if (!showCursor)
			ToogleCursor();

		//ustawienia globalne
		if (!LoadCharset(charset))
			throw Exception("Cannot load default charset");

		staticCharset = this->charset;

		Color::SetFormat(pict->format);
		settings.startTime = SDL_GetTicks();
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------

	Window::~Window()
	{
		SDL_Quit();
		SDL_DestroyTexture(scrtex);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Window::ToogleCursor() {
		if (settings.showCursor)
			SDL_ShowCursor(SDL_DISABLE);
		else SDL_ShowCursor(SDL_ENABLE);
		settings.showCursor = !settings.showCursor;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Window::SetWindowTitle(NonSTL::string title)
	{
		SDL_SetWindowTitle(window, title.c_str());
		settings.windowTitle = title;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	bool Window::Render()
	{
		OnRender();//wywo³anie zdarzenie RenderStart
		//renderowanie
		SDL_UpdateTexture(scrtex, NULL, pict->pixels, pict->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		//uaktualnienie liczników
		settings.lastRendTime = SDL_GetTicks();
		settings.fps = fpsHelper();
		settings.frames++;

		return true;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Window::Events()
	{
		//wywo³anie odpowiednich zdarzeñ
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN: OnKeyDown(event.key);
				break;
			case SDL_KEYUP: OnKeyUp(event.key);
				break;
			case SDL_QUIT:OnClose(event.quit);
				break;
			case SDL_MOUSEMOTION:OnMouseMove(event.motion);
				break;
			case SDL_MOUSEBUTTONDOWN:OnMouseButtonDown(event.button);
				break;
			case SDL_MOUSEBUTTONUP:OnMouseButtonUp(event.button);
				break;
			case SDL_MOUSEWHEEL:OnMouseWheel(event.wheel);
				break;
			};
		};
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Window::DoAll()
	{
		timers();
		Events();
		Render();
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	Events::KeyboardEventArgs PrepareKeyboardEvent(SDL_KeyboardEvent &e) {//funkcja przygotowuj¹ca strukturê dla zdarzeñ klawiatury
		Events::KeyboardEventArgs args;

		args.keyCode = e.keysym.sym;
		args.vKeyCode = e.keysym.scancode;
		args.sysKeys = e.keysym.mod;
		args.time = e.timestamp;
		return args;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	Events::MouseEventArgs PrepareMouseButtonEvent(SDL_MouseButtonEvent &e) {//funkcja przygotowuj¹ca strukturê dla zdarzeñ myszy
		Events::MouseEventArgs args;
		args.button = (Events::MouseEventArgs::Buttons)e.button;
		args.clicks = e.clicks;
		args.x = e.x;
		args.y = e.y;
		args.time = e.timestamp;
		return args;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Window::OnKeyDown(SDL_KeyboardEvent & e)
	{
		KeyDown.Invoke(PrepareKeyboardEvent(e));
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Window::OnKeyUp(SDL_KeyboardEvent & e)
	{
		KeyUp.Invoke(PrepareKeyboardEvent(e));
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Window::OnMouseButtonDown(SDL_MouseButtonEvent & e)
	{
		MouseDown.Invoke(PrepareMouseButtonEvent(e));
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Window::OnMouseButtonUp(SDL_MouseButtonEvent & e)
	{
		MouseUp.Invoke(PrepareMouseButtonEvent(e));
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Window::OnMouseWheel(SDL_MouseWheelEvent & e)
	{
		Events::MouseEventArgs args;
		args.hWheel = e.x;
		args.vWheel = e.y;
		args.time = e.timestamp;
		MouseWheel.Invoke(args);
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Window::OnMouseMove(SDL_MouseMotionEvent & e)
	{
		Events::MouseEventArgs args;
		args.button = (Events::MouseEventArgs::Buttons)e.state;
		args.x = e.x;
		args.y = e.y;
		args.lastX = e.x - e.xrel;
		args.lastY = e.y - e.yrel;
		args.time = e.timestamp;
		MouseMove.Invoke(args);
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Window::OnClose(SDL_QuitEvent & e)
	{
		Events::EventArgs args;
		args.time = e.timestamp;
		Close.Invoke(args);
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Window::OnRender()
	{
		Events::EventArgs args;
		args.time = SDL_GetTicks();
		RenderStart.Invoke(args);
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	const Settings Window::GetSettings()
	{
		return settings;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
}