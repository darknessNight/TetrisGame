#include "Test4.h"

int Test4() {
	MainClass mc;
	mc.start();
	return 0;
};

MainClass::MainClass() 
{
	sdl = new SdlLibrary::Window(SCREEN_WIDTH, SCREEN_HEIGHT, false, false);
	preparePicts();

	timer->Start();
}

void MainClass::start()
{
	sdl->RenderStart += Events::MethodHandler(this, (Events::Object::memberFunction)&MainClass::Sdl_Render);
	sdl->KeyDown += Events::MethodHandler(this, (Events::Object::memberFunction)&MainClass::Sdl_KeyDown);
	sdl->fpsHelper.interval = 500;
	while (active) {
		sdl->DoAll();
	}
}

void MainClass::Sdl_Render(Events::EventArgs * e)
{
	sdl->Clear();
	sdl->DrawBitmap(*background, { 0,0 });
	sdl->DrawBitmap(*block, { blockPosX, blockPosY });
	char buff[100];
	sprintf(buff, "FPS: %f", sdl->GetSettings().fps);
	sdl->DrawString({  (int)(sdl->GetClientRect().right - rightColW + (margin/2)),(int)(margin*1.5) }, buff);
}

void MainClass::Tick(Events::EventArgs * e)
{
	if (blockPosY < 600)
		blockPosY+=1;
	else blockPosY = 20;
}

void MainClass::Sdl_KeyDown(Events::KeyboardEventArgs * e)
{
	switch (e->vKeyCode) {
	case Events::KeyboardEventArgs::VKeyCode::ESCAPE:active = false; break;
	case Events::KeyboardEventArgs::VKeyCode::LEFT: if (blockPosX>margin)blockPosX-=4; 
													else blockPosX = sdl->GetClientRect().right - rightColW - (margin * 2); 
													break;
	case Events::KeyboardEventArgs::VKeyCode::RIGHT: if (blockPosX<sdl->GetClientRect().right - rightColW - (margin * 2))blockPosX+=4; 
													 else blockPosX = margin; 
													 break;
	}
}

void MainClass::preparePicts()
{
	background = new SdlLibrary::Bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
	block = new SdlLibrary::Bitmap(50, 50);
	timer = new SdlLibrary::Timer(Events::MethodHandler(this, (Events::Object::memberFunction)&MainClass::Tick));
	timer->interval = 10;
	sdl->timers += *timer;

	background->SetBrush(SdlLibrary::Brush(SdlLibrary::Color::Blue, SdlLibrary::Color::Black));
	background->FillRect(background->GetClientRect(), true);
	background->SetBrush(SdlLibrary::Brush(SdlLibrary::Color::Red, SdlLibrary::Color::Black, SdlLibrary::Brush::Dotted, 2));
	//lewa kolumna
	SdlLibrary::Rect rect = background->GetClientRect();
	rect.top += margin;
	rect.bottom -= margin;
	rect.left += margin;
	rect.right -= rightColW + margin;
	background->FillRect(rect);
	background->DrawRect(rect);
	//prawa kolumna
	rect.left = rect.right + margin;
	rect.right += rightColW;
	rect.bottom = margin + rightColH;
	background->FillRect(rect);
	background->DrawRect(rect);

	block->SetBrush(SdlLibrary::Brush(SdlLibrary::Color::Red, SdlLibrary::Color::Green));
	//block->FillRect({ 0,0,100,100 }, true);
	block->FillCircle({ 25,25 }, 25);
}
