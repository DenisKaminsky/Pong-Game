#include "Gameplay.h"

#define TIMER_ID 666
#define EXIT_COMMAND 9000

Graphics* graphics;
HWND hWnd;

//ball
POINT ballPosition;
POINT ballSpeed;
int ballRadius = 50;

int windowHeight = 0;
int windowWidth = 0;

void BallBounceFromBoundary()
{
	if (ballPosition.y + ballRadius >= windowHeight)
		ballSpeed.y = -ballSpeed.y;
	if (ballPosition.y - ballRadius <= 0)
		ballSpeed.y = -ballSpeed.y;
}

void BallOutOfField()
{
	if ((ballPosition.x + ballRadius >= windowWidth) || (ballPosition.x - ballRadius <= 0))
	{		
		ballPosition.x = windowWidth / 2;
		ballPosition.y = windowHeight / 2;
		Sleep(1000);
	}
}

VOID CALLBACK TimerProc(HWND hWnd, UINT uMessage, UINT_PTR uEventId, DWORD dwTime)
{	
	graphics->BeginDraw();
	graphics->ClearScreen(0, 0, 0);
	graphics->DrawCircle(ballPosition.x, ballPosition.y, ballRadius, 0, 0, 1, 1);
	//graphics->DrawRectangle(posX, posY, 20, 100, 0, 0, 1, 1);
	graphics->EndDraw();
	ballPosition.x += ballSpeed.x;
	ballPosition.y += ballSpeed.y;

	BallBounceFromBoundary();
	BallOutOfField();
}

void StartGame(HWND hwnd,bool isWithBotMode,int difficulty)
{
	RECT rect;
	hWnd = hwnd;
	graphics = new Graphics();
	graphics->Init(hWnd);
	//GetWindowRect(hWnd, &rect);
	GetClientRect(hWnd, &rect);
	windowHeight = rect.bottom - rect.top;
	windowWidth = rect.right - rect.left;
	ballSpeed.y = 10;
	ballSpeed.x = 1;
	ballPosition.x = (rect.right - rect.left) / 2;
	ballPosition.y = (rect.bottom - rect.top) / 2;
	SetTimer(hWnd, TIMER_ID, 20, TimerProc);
}

void DeleteGameParams()
{
	delete graphics;
	KillTimer(hWnd, TIMER_ID);
}

void StopGame()
{
	DeleteGameParams();
	SendMessage(hWnd, WM_COMMAND, EXIT_COMMAND, NULL);
}

void Pause()
{

}