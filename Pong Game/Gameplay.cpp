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
//border
int borderHeight = 38;

int lPlayerPoints = 0;
int rPlayerPoints = 0;
int windowHeight = 0;
int windowWidth = 0;

LPWSTR IntToLPWSTR(int number)
{
	wchar_t buffer[5];
	ZeroMemory(buffer, 5);
	wsprintfW(buffer, L"%d", number);
	return buffer;
}

void BallBounceFromBoundary()
{
	if (ballPosition.y + ballRadius >= windowHeight)
		ballSpeed.y = -ballSpeed.y;
	if (ballPosition.y - ballRadius <= 0)
		ballSpeed.y = -ballSpeed.y;
}

void BallOutOfField()
{
	if (ballPosition.x + ballRadius >= windowWidth)
	{		
		ballPosition.x = windowWidth / 2;
		ballPosition.y = windowHeight / 2;
		lPlayerPoints++;
		Sleep(1000);
	}
	if (ballPosition.x - ballRadius <= 0)
	{
		ballPosition.x = windowWidth / 2;
		ballPosition.y = windowHeight / 2;
		rPlayerPoints++;
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

void RBoardMoveDown()
{
	rBoardPosition.y += rBoardSpeed;
	BoardLimit(rBoardPosition, rBoardHeight);
}

void RBoardMoveUp()
{
	rBoardPosition.y -= rBoardSpeed;
	BoardLimit(rBoardPosition, rBoardHeight);
}

void LBoardMoveDown()
{
	lBoardPosition.y += lBoardSpeed;
}

void LBoardMoveUp()
{
	lBoardPosition.y -= lBoardSpeed;
}

void SetGameParameters(int difficulty, int bRadius, int bWidth, int lbHeight, int rbHeight, int rbSpeed)
{
	RECT rect;

	lPlayerPoints = 0;
	rPlayerPoints = 0;
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

VOID CALLBACK TimerProc(HWND hWnd, UINT uMessage, UINT_PTR uEventId, DWORD dwTime)
{	
	graphics->BeginDraw();
	graphics->ClearScreen(0, 0, 0);
	//drawBall
	graphics->DrawCircle(ballPosition.x, ballPosition.y, ballRadius, 0, 0, 1, 1);
	//drawLBoard
	graphics->DrawRoundRectangle(lBoardPosition.x, lBoardPosition.y, boardWidth, lBoardHeight, 255, 0, 0, 1);
	//drawRBoard
	graphics->DrawRoundRectangle(rBoardPosition.x, rBoardPosition.y, boardWidth, rBoardHeight, 255, 0, 0, 1);
	//drawBorders
	graphics->DrawRectangle(0,0, windowWidth, borderHeight, 255, 255, 155, 1);
	graphics->DrawRectangle(0,windowHeight-borderHeight, windowWidth, borderHeight,255, 255, 155, 1);
	graphics->DrawString(L"0", 1, windowWidth/2-25, 1, 20, 20,40, 255, 0, 0, 1);
    graphics->DrawString(L":", 1, windowWidth/2+2,1, 1, 20,40, 255, 0, 0, 1);
	graphics->DrawString(L"0", 1, windowWidth/2+15, 1, 20, 20,40, 255, 0, 0, 1);
	ballPosition.x += ballSpeed.x;
	ballPosition.y += ballSpeed.y;
	BallBounceFromBoundary();
	//here boards and ball contact physics
	BoardLimit(lBoardPosition, lBoardHeight);
	//BoardLimit(rBoardPosition, rBoardHeight);
	BallOutOfField();

	graphics->EndDraw();
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