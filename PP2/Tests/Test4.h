#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include<iostream>
#include<string.h>
#include"../SdlClass.h"

class MainClass : Events::Object{
public:
	MainClass();
	void start();
private:
	void Sdl_Render(Events::EventArgs *e);
	void Tick(Events::EventArgs *e);
	void Sdl_KeyDown(Events::KeyboardEventArgs* e);
	void preparePicts();
private:
	unsigned margin = 20;
	unsigned rightColW = 200;
	unsigned rightColH = 400;
	SdlLibrary::Window *sdl;
	SdlLibrary::Bitmap *background;
	SdlLibrary::Bitmap *block;
	int blockPosY = 0;
	int blockPosX = 100;
	SdlLibrary::Timer *timer;
	bool active = true;

	const unsigned SCREEN_WIDTH=800;
	const unsigned SCREEN_HEIGHT=640;
};

int Test4();
