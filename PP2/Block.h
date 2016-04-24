/* struktura pomocnicza zawieraj� informacje o aktywnym klocku
*/
#pragma once
#include "SdlBitmap.h"

namespace TetrisGame {
	struct Block
	{
		Block() {
			pos.x = pos.y = rendPos.x = rendPos.y = 0;
		};
		Block(unsigned color, unsigned elsCount, ...);
	public:
		static const unsigned size = 4;
		SdlLibrary::Point rendPos;// = { 0,0 };//wzgl�dna pozycja na potrzeby rysowania (w pikselach)
		SdlLibrary::Point pos;// = { 0,0 };//bezwzgl�dna pozycja na siatce planszy
		int rot = 0;//obr�t w stopniach
		unsigned color;//numer heksadecymalny koloru w systemie ARGB
		bool shape[size][size];//siatka u�o�enia
	};

	Block* ReadyShapes();
	unsigned ReadyShapesCount();
}