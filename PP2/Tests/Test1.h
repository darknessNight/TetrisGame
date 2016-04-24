#define _USE_MATH_DEFINES
#include<math.h>
#include<iostream>
#include<string.h>
#include"../SdlClass.h"

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

int y = 0;

class Klasa: public Events::Object{
	int* quit;
	double* etiSpeed;
public:
	Klasa(int& quit, double &etiSpeed) {
		this->quit = &quit;
		this->etiSpeed = &etiSpeed;
	}

	void OnKeyDown(Events::KeyboardEventArgs *e) {
		switch (e->vKeyCode) {
		case Events::KeyboardEventArgs::VKeyCode::ESCAPE:(*quit) = 1; break;
		case Events::KeyboardEventArgs::VKeyCode::UP:(*etiSpeed) = 2; break;
		case Events::KeyboardEventArgs::VKeyCode::DOWN:(*etiSpeed) = 0.3; break;
			break;
		}
	}
	void OnKeyUp(Events::KeyboardEventArgs *e) {
		(*etiSpeed) = 1;
	}

	void TimerMethod(Events::EventArgs *e) {
		if(y<SCREEN_HEIGHT/2)
			y += 1;
		else y = 1;
	}
};

int Test1() {
	int t1, t2, quit, frames;
	double delta, worldTime, fpsTimer, fps, distance, etiSpeed, tmpFps;
	SDL_Event event;
	char buff[1000];

	SdlLibrary::Window sdlObject(800, 640, false, false);
	sdlObject.SetWindowTitle("Projekt PP nr 2");

	SdlLibrary::Bitmap eti("./eti.bmp");
	SdlLibrary::Bitmap text(sdlObject.GetClientRect().right-2, 50);
	//SdlLibrary::Bitmap text("file.bmp");
	bool test3=text.LoadCharset("cs8x8.bmp");

	// tekst informacyjny
	text.SetBrush(SdlLibrary::Brush(SdlLibrary::Color::Red, SdlLibrary::Color::Blue,SdlLibrary::Brush::Type::Dotted,4));
	text.FillRect({ 4, 4, sdlObject.GetClientRect().right - 8, 36 });
	text.DrawRect({ 4, 4, sdlObject.GetClientRect().right - 8 , 36 });
	sprintf(buff, "Esc - wyjscie, \030 - przyspieszenie, \031 - zwolnienie");
	text.DrawString({ (int)(sdlObject.GetClientRect().right / 2 - strlen(buff) * 8 / 2), 26 }, buff);

	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	worldTime = 0;
	distance = 0;
	etiSpeed = 1;

	Klasa obj(quit, etiSpeed);
	Events::MethodHandler hand;
	hand.method = (Klasa::memberFunction)&Klasa::OnKeyDown;
	hand.object = &obj;
	sdlObject.KeyDown += hand;
	hand.method = (Klasa::memberFunction)&Klasa::OnKeyUp;
	sdlObject.KeyUp += hand;

	hand.method = (Klasa::memberFunction)&Klasa::TimerMethod;
	SdlLibrary::Timer timer(hand);
	timer.interval = 100;

	sdlObject.timers += timer;
	sdlObject.fpsHelper.interval = 400;
	t1 = SDL_GetTicks();
	while (!quit) {
		t2 = SDL_GetTicks();
		delta =  (t2 - t1)*0.001;
		t1 = t2;

		distance += etiSpeed * delta;
		sdlObject.Clear();

		sdlObject.DrawBitmap(eti, { (int)(SCREEN_WIDTH / 2 + sin(distance) * SCREEN_HEIGHT / 3),
				(int)(SCREEN_HEIGHT / 2 + cos(distance) * SCREEN_HEIGHT / 3) });
		sdlObject.DrawBitmap(text, { 1,1+y });

		sprintf(buff, "Szablon drugiego zadania, czas trwania = %.1lf s  %.0lf klatek / s", SDL_GetTicks()*0.001 , sdlObject.GetSettings().fps);
		sdlObject.DrawString({ (int)(sdlObject.GetClientRect().right / 2 - strlen(buff) * 8 / 2), 10 }, buff);

		sdlObject.DoAll();
		frames++;
		};
	return 0;
	};
