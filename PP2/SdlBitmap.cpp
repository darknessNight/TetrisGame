#include "SdlException.h"
#include "SdlBitmap.h"


namespace SdlLibrary {
	SDL_Surface* Bitmap::staticCharset;
	//-----------------------------------------------------------------------------------------------------------------------------------------
	Bitmap::Bitmap(NonSTL::string file):brush(Color::White,Color::Black) {
		if (!Load(file)) {
			throw Exception("Cannot load bitmap");
		}
		charset = staticCharset;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	Bitmap::Bitmap() :brush(Color::White, Color::Black){
		pict = nullptr;
		charset = staticCharset;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	Bitmap::Bitmap(unsigned width, unsigned height) : brush(Color::White, Color::Black)
	{
		pict = SDL_CreateRGBSurface(0, width, height, 32,
			0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
		charset = staticCharset;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------

	Bitmap::~Bitmap()
	{
		if(charset!=nullptr)
			SDL_FreeSurface(charset);
		if (pict != nullptr && pict->format != nullptr)
			SDL_FreeSurface(pict);
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	bool Bitmap::Load(NonSTL::string file){
		pict = SDL_LoadBMP(file.c_str());
		if (pict == nullptr) {
			return false;
		}
		else {
			return true;
		}
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	bool Bitmap::LoadCharset(NonSTL::string file)
	{
		charset = SDL_LoadBMP(file.c_str());
		if (charset!=nullptr) {
			
			SDL_SetColorKey(charset, true, 0x000000);
			return true;
		}
		charset = nullptr;
		return false;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	bool Bitmap::DrawString(Point p, const char *text) {
		if (charset == nullptr || pict == nullptr) 
			return false;
		int px, py, c, x, y;
		SDL_Rect s, d;
		s.w = 8;s.h = 8;d.w = 8;d.h = 8;
		x = p.x; y = p.y;
		while (*text) {
			if (*text == '\n') {
				x = p.x;
				y += 10;
			}
			else {
				c = *text & 255;
				px = (c % 16) * 8;
				py = (c / 16) * 8;
				s.x = px; s.y = py; d.x = x; d.y = y;
				if (SDL_BlitSurface(charset, &s, pict, &d) != 0)
					return false;
				x += 8;
			}
			text++;
		}
		return true;
	};

	//-----------------------------------------------------------------------------------------------------------------------------------------
	bool Bitmap::DrawBitmap(Bitmap &sprite, Point p) {
		SDL_Rect dest;
		dest.x = p.x;
		dest.y = p.y;
		dest.w = sprite.pict->w;
		dest.h = sprite.pict->h;
		int ret = SDL_BlitSurface(sprite.pict, NULL, pict, &dest);
		if (ret==0)
			return true;
		else return false;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Bitmap::DrawPixel(Point p)
	{
		DrawPixel(p, brush.color);
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Bitmap::DrawPixel(Point p, Color color)
	{
		if (p.x < pict->clip_rect.w && p.y < pict->clip_rect.h && p.x > 0 && p.y > 0) {
			int bpp = pict->format->BytesPerPixel;
			Uint8 *pix = (Uint8 *)pict->pixels + p.y * pict->pitch + p.x * bpp;
			*(unsigned *)pix = color;
		}
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Bitmap::DrawLine(Point p1, Point p2, int size, bool foreColor)
	{
		if (abs(p1.x - p2.x) > pict->clip_rect.w || abs(p1.y - p2.y) > pict->clip_rect.h)return;
		Color c = (foreColor ? brush.color : brush.fillColor);
		if (p1.x == p2.x && p1.y == p2.y) { DrawPixel(p1, c); return; }
		if (size <= 0) size = brush.size;
		double a, b; bool inv = false;
		
		//obliczanie wzoru linii
		if (abs(p2.y-p1.y)>abs(p2.x-p1.x)) {
			if (p1.y > p2.y) {Point tmp = p2;p2 = p1; p1 = tmp;}
			a = (p2.x - p1.x) / (p2.y - (double)p1.y);b = p2.x - a*p2.y;
			inv = true;
		}
		else {
			if (p1.x > p2.x) {Point tmp = p2;p2 = p1; p1 = tmp;}
			a = (p2.y - p1.y) / (p2.x - (double)p1.x);b = p2.y - a*p2.x;
		}
		//w³aœciwe rysowanie
		for (int i = 0; i < size; i++) {//¿eby uzyskaæ odpowiedni¹ gruboœæ rysuje linie kilkukrotnie
			for (int j = (inv ? p1.y : p1.x) - size + 1; j < (inv ? p2.y : p2.x) + 1; j++) {
				if (brush.lineDraw(j)) {//rysuje w zale¿noœci od wybranego typu linii
					if (inv) DrawPixel({ (int)(j*a + b), j }, c);
					else DrawPixel({ j,(int)(j*a + b) }, c);
				}
			}
			b -= 1;
		}
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Bitmap::DrawPoint(Point p1)
	{
		FillCircle(p1, brush.size, true);
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Bitmap::DrawRect(Rect rect)
	{
		DrawLine({ rect.left,rect.top }, { rect.right + brush.size - 1,rect.top });
		DrawLine({ rect.left,rect.bottom+brush.size-1 }, { rect.right,rect.bottom + brush.size - 1 });
		DrawLine({ rect.left,rect.top }, { rect.left,rect.bottom + brush.size - 1 });
		DrawLine({ rect.right + brush.size - 1,rect.top }, { rect.right + brush.size - 1,rect.bottom + brush.size - 1 });
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Bitmap::FillRect(Rect rect1, bool foreColor)
	{
		SDL_Rect rect;
		rect.x = rect1.left;rect.y = rect1.top;
		rect.w = rect1.right - rect1.left;
		rect.h = rect1.bottom - rect1.top;
		int result=SDL_FillRect(pict, &rect, (foreColor?brush.color:brush.fillColor));
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Bitmap::DrawCircle(Point p, int radius)
	{
		//licznikiem jest k¹t. ³¹czone s¹ punkty: poprzedni punkt rysowania do nowy punkt rysowania, wiêc ko³o jest trochê kanciaste
		//za to liczba obrotów pêtli jest sta³a
		for (int i = 0; i < brush.size; i++) {
			int y,x, lastX,lastY;
			lastX = sin(0)*radius + p.x;
			lastY = cos(0)*radius + p.y;
			for (double j = 0; j < 2*M_PI; j+=circleDrawStep) {
				x = sin(j)*radius+p.x; y = cos(j)*radius+p.y;
				DrawLine({ lastX,lastY }, {x,y}, 1, brush.color);
				lastX = x; lastY = y;
			}
			radius++;
		}
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Bitmap::FillCircle(Point p, int radius, bool foreColor)
	{
		double y1, r = radius*radius;
		for (int i = p.x - radius; i < p.x + radius; i++) {
			y1 = sqrt(r - pow(i - p.x, 2));
			DrawLine({ (int)i,p.y - (int)y1 }, { (int)i,p.y + (int)y1 }, 1, foreColor);
		}
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Bitmap::Clear()
	{
		SDL_FillRect(pict, nullptr, 0);
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	void Bitmap::SetBrush(Brush brush)
	{
		this->brush = brush;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------
	Rect Bitmap::GetClientRect()
	{
		return{ pict->clip_rect.x,pict->clip_rect.y, pict->clip_rect.x+ pict->clip_rect.w, pict->clip_rect.y + pict->clip_rect.h };
	}
}