#include "HelperClasses.h"
#include <iostream>

namespace SdlLibrary {
	//Metody clasy Color
	//
	Color::operator long int() {
		return prepared;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Color::operator=(Color &c)
	{
		this->prepared = c.prepared;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Color::operator=(unsigned hex)
	{
		Prepare((hex & 0xFF0000) >> 16, (hex & 0x00FF00) >> 8, (hex & 0x0000FF), (hex & 0xff000000) >> 24);
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Color::SetFormat(SDL_PixelFormat *f)
	{
		format = f;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	Color::Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
	{
		Prepare(r, g, b, a);
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	Color::Color(long int hex)
	{
		Prepare((hex & 0xFF0000)>>16, (hex & 0x00FF00)>>8, (hex & 0x0000FF), (hex&0xff000000)>>24);
		//Prepare(hex, hex, hex, hex);
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	Color::Color(Names name)
	{
		long int hex = (int)name;
		Prepare((hex & 0xFF0000) >> 16, (hex & 0x00FF00) >> 8, (hex & 0x0000FF), (hex & 0xff000000) >> 24);
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	Color::Color()
	{
		Color((int)Names::Transparent);
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Color::Prepare(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
	{
		if (format != nullptr)
			prepared = SDL_MapRGBA(format, r, g, b, a);
		//std::cout << prepared << std::endl;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	//
	//Metody klasy brush
	//
	//-----------------------------------------------------------------------------------------------------------------------------------------
	Brush::Brush(Color fore, Color fill, Type type, int size ) {
		color = fore;
		fillColor = fill;
		this->type = type;
		this->size = size;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Brush::operator=(Brush & el)
	{
		this->color = el.color;
		this->fillColor = el.fillColor;
		this->size = el.size;
		this->type = el.type;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	bool Brush::lineDraw(int i)
	{
		switch (type) {
		case Type::Solid: return true;
		case Type::Dotted: return ((i / __max(size,dotLenght)) % 2 == 1);
		case Type::Dashed: return ((i % (lineLenght + dotLenght)) <= lineLenght);
		}
		return true;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	//
	//Metody klasy Timer
	//
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Timer::operator()()
	{
		if (lastExecute<0 || !active) return;//zakoñcz jeœli timer jest wstrzymany (a lastExecute zawiera informacje o odstêpie czasowym)
		//jeœli min¹³ po¿¹dany czas od ostatniego wykonania wykonaj instrukcje
		int tmp = SDL_GetTicks();
		if (tmp - lastExecute > interval) {
			lastExecute = tmp;
			if (function != nullptr) {
				function();
			}
			if (method.object != nullptr && method.method != nullptr) {
				Events::EventArgs args;
				args.sender = this;
				args.time = tmp;
				method(&args);
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Timer::Pause()
	{
		if (lastExecute < 0) {
			lastExecute += SDL_GetTicks();
		}
		else {
			lastExecute -= SDL_GetTicks();
		}
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	bool Timer::operator==(Timer &t)
	{
		if (method == t.method && function == t.function) return true;
		else return false;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	//
	//Metody klasy Timer
	//
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Timers::operator()() { for (int i = 0; i < timers.size(); i++)(*timers[i])(); }
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Timers::Pause()
	{
		for (int i = 0; i < timers.size(); i++)timers[i]->Pause();
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Timers::Start()
	{
		for (int i = 0; i < timers.size(); i++)timers[i]->Start();
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Timers::Stop()
	{
		for (int i = 0; i < timers.size(); i++)timers[i]->Stop();
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	int Timers::search(Timer *t) {
		for (unsigned int i = 0; i < timers.size(); i++) {
			if (timers[i] == t) return i;
		}return -1;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	//
	//Metody klasy FpsHelper
	//
	//-----------------------------------------------------------------------------------------------------------------------------------------
	double FpsHelper::operator()()
	{
		int tmp = SDL_GetTicks();
		if (tmp - lastRend > interval) {
			lastFps = frames *(1000 / (double)interval);
			frames = 0;
			lastRend = tmp;
		}
		frames++;
		return lastFps;
	}

}