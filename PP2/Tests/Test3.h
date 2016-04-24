#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include<iostream>
#include<string.h>
#include"../SdlClass.h"

#define SCREEN_WIDTH	500
#define SCREEN_HEIGHT	500

int Test3() {
	int quit;
	char buff[1000];

	SdlLibrary::Window sdlObject(SCREEN_WIDTH, SCREEN_HEIGHT, false, false);
	sdlObject.SetWindowTitle("Projekt PP nr 2");

	SdlLibrary::Brush br(SdlLibrary::Color::White, SdlLibrary::Color::Red, SdlLibrary::Brush::Solid, 5);
	sdlObject.Clear();

	sdlObject.SetBrush(br);
	int r = 250;
	int x, y;
	for (double i = 0; i < 2*M_PI; i += 0.05) {
		sdlObject.SetBrush(SdlLibrary::Brush(SdlLibrary::Color(255*sin(i),255*cos(i), 255*tan(i)), SdlLibrary::Color::Red));
		x = sin(i)*r+250;
		y = cos(i)*r+250;
		sdlObject.DrawLine({ 250,250 }, { x,y });
	}
	

	sdlObject.Render();
	_sleep(10000);
	return 0;
};
