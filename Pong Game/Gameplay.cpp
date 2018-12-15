#include "Gameplay.h"
#include <string>

#define TIMER_ID 666
#define EXIT_COMMAND 9000
#define GO_TO_MAIN_MENU 10009

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
int BottomBorderHeight = 25;
int TopBorderHeight = 48;

int lPlayerPoints = 0;
int rPlayerPoints = 0;
int windowHeight = 0;
int windowWidth = 0;
bool withBot;

void BallBounceFromBoundary()
{
	if (ballPosition.y + ballRadius >= windowHeight-BottomBorderHeight)
		ballSpeed.y = -ballSpeed.y;
	if (ballPosition.y - ballRadius <= TopBorderHeight)
		ballSpeed.y = -ballSpeed.y;
}

void CheckGameIsOver()
{
	if (lPlayerPoints == 6)
		if (withBot)
			StopGame("YOU LOSE");
		else
			StopGame("PLAYER 1 WIN");
	if (rPlayerPoints == 6)
		StopGame("PLAYER 2 WIN");
}

void BallOutOfField()
{
	if (ballPosition.x + ballRadius >= windowWidth)
	{		
		ballPosition.x = windowWidth / 2;
		ballPosition.y = windowHeight / 2;
		lPlayerPoints++;
		Sleep(1000);
		CheckGameIsOver();
	}
	if (ballPosition.x - ballRadius <= 0)
	{
		ballPosition.x = windowWidth / 2;
		ballPosition.y = windowHeight / 2;
		rPlayerPoints++;
		Sleep(1000);
		CheckGameIsOver();
	}
}

void BoardLimit(POINT &boardPosition,int boardHeight)
{
	if (boardPosition.y <= TopBorderHeight)
		boardPosition.y = TopBorderHeight;
	if (boardPosition.y + boardHeight >= windowHeight-BottomBorderHeight)
		boardPosition.y = windowHeight - boardHeight - BottomBorderHeight;
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
	BoardLimit(lBoardPosition, lBoardHeight);
}

void LBoardMoveUp()
{
	lBoardPosition.y -= lBoardSpeed;
	BoardLimit(lBoardPosition, lBoardHeight);
}

void SetGameParameters(bool isWithBotMode,int difficulty, int bRadius, int bWidth, int lbHeight, int rbHeight, int rbSpeed)
{
	RECT rect;
	
	withBot = isWithBotMode;
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
		ballSpeed.y = 20;
		ballSpeed.x = 15;
		lBoardSpeed = rbSpeed;
	}
}

VOID CALLBACK TimerProc(HWND hWnd, UINT uMessage, UINT_PTR uEventId, DWORD dwTime)
{	
	graphics->BeginDraw();
	graphics->ClearScreen(0, 0, 0);
	//draw background
	graphics->DrawImage(graphics->GetBitmapBackground(), 0, 0, windowWidth, windowHeight);
	//drawBall
	graphics->DrawCircle(ballPosition.x, ballPosition.y, ballRadius, 255, 0, 0, 1);
	//drawLBoard
	graphics->DrawImage(graphics->GetBitmapLBoard(), lBoardPosition.x, lBoardPosition.y, boardWidth, lBoardHeight);
	//graphics->DrawRoundRectangle(lBoardPosition.x, lBoardPosition.y, boardWidth, lBoardHeight, 255, 0, 0, 1);
	//drawRBoard
	graphics->DrawImage(graphics->GetBitmapRBoard(), rBoardPosition.x, rBoardPosition.y, boardWidth, rBoardHeight);
	//graphics->DrawRoundRectangle(rBoardPosition.x, rBoardPosition.y, boardWidth, rBoardHeight, 255, 0, 0, 1);
	//draw score
	graphics->DrawString(std::to_wstring(lPlayerPoints).c_str(), 1, (float)(windowWidth / 2 - 30), 1, 20, 20, 40, 255, 0, 0, 1);
	graphics->DrawString(L":", 1, (float)(windowWidth / 2 + 2), 1, 1, 20, 40, 255, 0, 0, 1);
	graphics->DrawString(std::to_wstring(rPlayerPoints).c_str(), 1, (float)(windowWidth / 2 + 20), 1, 20, 20, 40, 255, 0, 0, 1);
	graphics->EndDraw();
	ballPosition.x += ballSpeed.x;
	ballPosition.y += ballSpeed.y;
	BallBounceFromBoundary();
	//here boards and ball contact physics
	BoardLimit(lBoardPosition, lBoardHeight);
	//BoardLimit(rBoardPosition, rBoardHeight);
	BallOutOfField();
}

void InitializeGame(HWND hwnd)
{
	hWnd = hwnd;
	graphics = new Graphics();
	graphics->Init(hWnd);
	graphics->SetBitmapBackground(graphics->LoadImageFromFile(L"..\\ResourceFiles\\font.jpg"));
	graphics->SetBitmapLBoard(graphics->LoadImageFromFile(L"..\\ResourceFiles\\LBoard2.png"));
	graphics->SetBitmapRBoard(graphics->LoadImageFromFile(L"..\\ResourceFiles\\RBoard2.png"));
}

void StopTimer()
{
	KillTimer(hWnd, TIMER_ID);
}

void DeleteGame()
{
	StopTimer();
	delete graphics;
}

void StartGame(HWND hwnd,bool isWithBotMode,int difficulty)
{
	SetGameParameters(isWithBotMode,difficulty,20,30,180,180,10);
	SetTimer(hWnd, TIMER_ID, 20, TimerProc);
}

//finish game and show result
void StopGame(LPCSTR message)
{
	StopTimer();
	graphics->BeginDraw();
	graphics->ClearScreen(0, 0, 0);
	graphics->EndDraw();
	SendMessage(hWnd, WM_COMMAND, EXIT_COMMAND, (LPARAM)message);
}

//continue game
void Continue()
{
	SetTimer(hWnd, TIMER_ID, 20, TimerProc);
}

//pause game
void Pause()
{
	StopTimer();
	graphics->BeginDraw();
	graphics->DrawString(L"PAUSE", 5, (float)(windowWidth / 2 - 80), windowHeight/2, 170, 60, 60, 255, 0, 0, 1);
	graphics->EndDraw();
}

//close game
void ExitGame()
{
	StopTimer();
	int MB_RESULT = MessageBox(hWnd, "Do you really want to exit ?", "PAUSE", MB_YESNO);
	if (MB_RESULT == 6)
		SendMessage(hWnd, WM_COMMAND, GO_TO_MAIN_MENU, NULL);
	else
		SetTimer(hWnd, TIMER_ID, 20, TimerProc);
}