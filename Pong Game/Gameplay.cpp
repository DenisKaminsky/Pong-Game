#include "Gameplay.h"

#define TIMER_ID 666
#define EXIT_COMMAND 9000

Graphics* graphics;
HWND hWnd;

//ball
POINT ballPosition;
POINT ballSpeed;
int ballRadius;
//left board
POINT lBoardPosition;
int lBoardSpeed;
int lBoardHeight;
//right board
POINT rBoardPosition;
int rBoardSpeed;
int rBoardHeight;
//boards width
int boardWidth;

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

void BoardLimit(POINT &boardPosition,int boardHeight)
{
	if (boardPosition.y <= 0)
		boardPosition.y = 0;
	if (boardPosition.y + boardHeight >= windowHeight)
		boardPosition.y = windowHeight - boardHeight;
}

VOID CALLBACK TimerProc(HWND hWnd, UINT uMessage, UINT_PTR uEventId, DWORD dwTime)
{	
	graphics->BeginDraw();
	graphics->ClearScreen(0, 0, 0);
	//drawBall
	graphics->DrawCircle(ballPosition.x, ballPosition.y, ballRadius, 0, 0, 1, 1);
	//drawLBoard
	graphics->DrawRectangle(lBoardPosition.x, lBoardPosition.y, boardWidth, lBoardHeight, 255, 0, 0, 1);
	//drawRBoard
	graphics->DrawRectangle(rBoardPosition.x, rBoardPosition.y, boardWidth, rBoardHeight, 255, 0, 0, 1);

	BallBounceFromBoundary();
	//here boards and ball contact physics
	BoardLimit(lBoardPosition, lBoardHeight);
	BoardLimit(rBoardPosition, rBoardHeight);
	BallOutOfField();
	ballPosition.x += ballSpeed.x;
	ballPosition.y += ballSpeed.y;

	graphics->EndDraw();
}

void SetGameParameters(int difficulty,int bRadius,int bWidth,int lbHeight,int rbHeight,int rbSpeed)
{
	RECT rect;

	GetClientRect(hWnd, &rect);
	windowHeight = rect.bottom - rect.top;
	windowWidth = rect.right - rect.left;
	//ball
	ballRadius = bRadius;
	ballPosition.x = (rect.right - rect.left) / 2;
	ballPosition.y = (rect.bottom - rect.top) / 2;
	//board
	boardWidth = bWidth;
	lBoardHeight = lbHeight;
	lBoardPosition.x = 8;
	lBoardPosition.y = windowHeight / 2 - lBoardHeight / 2;
	rBoardHeight = rbHeight;
	rBoardSpeed = rbSpeed;
	rBoardPosition.x = windowWidth - boardWidth - 8;
	rBoardPosition.y = windowHeight / 2 - rBoardHeight / 2;
	//easy
	if (difficulty = 1)
	{
		ballSpeed.y = 10;
		ballSpeed.x = 1;
		lBoardSpeed = rbSpeed;
	}
}

void StartGame(HWND hwnd,bool isWithBotMode,int difficulty)
{
	hWnd = hwnd;
	graphics = new Graphics();
	graphics->Init(hWnd);
	SetGameParameters(difficulty,20,30,180,180,10);
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