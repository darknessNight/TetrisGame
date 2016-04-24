#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include<iostream>
#include<string.h>
#include"../SdlClass.h"

#define SCREEN_WIDTH	500
#define SCREEN_HEIGHT	500

int Test2() {
	int quit;
	char buff[1000];

	SdlLibrary::Window sdlObject(SCREEN_WIDTH, SCREEN_HEIGHT, false, false);
	sdlObject.SetWindowTitle("Projekt PP nr 2");

	SdlLibrary::Brush br(SdlLibrary::Color::White, SdlLibrary::Color::Red, SdlLibrary::Brush::Solid, 5);
	sdlObject.Clear();

	sdlObject.SetBrush(br);
	sdlObject.DrawLine({ 10,10 }, { 10,SCREEN_WIDTH - 10 });
	sdlObject.DrawLine({ 10,10 }, { 10,SCREEN_WIDTH - 10 }, 1, false);
	sdlObject.SetBrush(SdlLibrary::Brush(SdlLibrary::Color::Red, SdlLibrary::Color::Green));
	sdlObject.DrawLine({ 20,10 }, { 20,SCREEN_WIDTH - 10 });
	sdlObject.DrawLine({ 20,10 }, { 20,SCREEN_WIDTH - 10 }, 1, false);
	sdlObject.SetBrush(SdlLibrary::Brush(SdlLibrary::Color::Blue, SdlLibrary::Color::Green));
	sdlObject.DrawLine({ 30,10 }, { 30,SCREEN_WIDTH - 10 });
	sdlObject.SetBrush(SdlLibrary::Brush(SdlLibrary::Color::White, SdlLibrary::Color::Green));
	sdlObject.DrawLine({ 40,10 }, { 40,SCREEN_WIDTH - 10 });
	sdlObject.SetBrush(SdlLibrary::Brush(SdlLibrary::Color::White, SdlLibrary::Color::Green, SdlLibrary::Brush::Type::Dotted));
	sdlObject.DrawLine({ 40,10 }, { 40,SCREEN_WIDTH - 10 });
	sdlObject.SetBrush(SdlLibrary::Brush(SdlLibrary::Color::White, SdlLibrary::Color::Green, SdlLibrary::Brush::Type::Dotted));
	sdlObject.DrawLine({ 50,10 }, { 50,SCREEN_WIDTH - 10 });
	sdlObject.SetBrush(SdlLibrary::Brush(SdlLibrary::Color::White, SdlLibrary::Color::Green, SdlLibrary::Brush::Type::Dotted, 5));
	sdlObject.DrawLine({ 60,10 }, { 60,SCREEN_WIDTH - 10 });
	sdlObject.SetBrush(SdlLibrary::Brush(SdlLibrary::Color::White, SdlLibrary::Color::Green, SdlLibrary::Brush::Type::Dashed));
	sdlObject.DrawLine({ 70,10 }, { 70,SCREEN_WIDTH - 10 });
	sdlObject.SetBrush(SdlLibrary::Brush(SdlLibrary::Color::White, SdlLibrary::Color::Green, SdlLibrary::Brush::Type::Dashed, 5));
	sdlObject.DrawLine({ 80,10 }, { 80,SCREEN_WIDTH - 10 });



	sdlObject.SetBrush(br);
	sdlObject.FillCircle({ 200,100 }, 50);
	sdlObject.DrawCircle({ 200,100 }, 50);
	sdlObject.FillCircle({ 400,100 }, 50);
	sdlObject.DrawRect({ 150,250, 250, 350 });
	sdlObject.FillRect({ 350,250, 450, 350 });
	sdlObject.DrawString({ 100,400 }, "Test dzialania biblioteki");

	sdlObject.Render();
	_sleep(10000);
	return 0;
};
