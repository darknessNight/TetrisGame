#include "Block.h"
#include <cstdarg>

namespace TetrisGame {

	Block readyShapes[] = {//zestaw gotowych klocków
		Block(SdlLibrary::Color::Red, 16, true, false, false, false,/**/ true, false, false, false,/**/ true, false, false, false,/**/true, false, false, false),// I
		Block(SdlLibrary::Color::Green, 6, true, true, false, false,/**/ true,true),//O
		Block(SdlLibrary::Color::Aqua, 6, true, true, true, false,/**/ false,true),//T
		Block(SdlLibrary::Color::Fuchsia, 10, true, false ,false, false,/**/ true,false, false, false,/**/ true, true),// L
		Block(SdlLibrary::Color::Gray, 10, false, true,false,false,/**/ false, true,false, false,/**/ true, true),// j
		Block(SdlLibrary::Color::Maroon, 6, false, true, true, false,/**/ true,true),//S
		Block(SdlLibrary::Color::Yellow, 7, true, true, false, false,/**/ false,true, true),//Z
	};

	unsigned readyShapesCount = 7;

	Block::Block(unsigned c, unsigned elC, ...)
	{
		color = c;
		bool el;
		va_list list;
		va_start(list, elC);
		for (int i = 0; i < elC; i++) {
			el = va_arg(list, bool);
			shape[i / size][i%size] = el;
		}
		for (int i = elC; i < 16; i++)
			shape[i / size][i%size] = false;
		va_end(list);
	}

	Block* ReadyShapes()
	{
		return readyShapes;
	}

	unsigned ReadyShapesCount()
	{
		return readyShapesCount;
	}
}