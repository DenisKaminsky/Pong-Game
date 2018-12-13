#include "Gameplay.h"

#define TIMER_ID 666
#define EXIT_COMMAND 9000

Graphics* graphics;
HWND hWnd;

int posX = 0;
int posY = 0;

VOID CALLBACK TimerProc(HWND hWnd, UINT uMessage, UINT_PTR uEventId, DWORD dwTime)
{
	graphics->BeginDraw();
	graphics->ClearScreen(0, 0, 0);
	graphics->DrawRectangle(posX, posY, 20, 100, 0, 0, 1, 1);
	graphics->EndDraw();
	posX++;
}

void StartGame(HWND hwnd,bool isWithBotMode,int difficulty)
{
	hWnd = hwnd;
	graphics = new Graphics();
	graphics->Init(hWnd);
	SetTimer(hWnd, TIMER_ID, 20, TimerProc);
}

void DeleteGameParams()
{
	delete graphics;
	KillTimer(hWnd, TIMER_ID);
}

void StopGame()
{
	posX = 0;
	posY = 0;
	DeleteGameParams();
	SendMessage(hWnd, WM_COMMAND, EXIT_COMMAND, NULL);
}

void Pause()
{

}