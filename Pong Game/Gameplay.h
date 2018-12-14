#pragma once
#include <Windows.h>
#include "Graphics.h"

void StartGame(HWND hWnd, bool isWithBotMode, int difficulty);
void StopGame(LPCSTR message);
void Pause();
void DeleteGame();
void InitializeGame(HWND hwnd);
void RBoardMoveDown();
void RBoardMoveUp();
void LBoardMoveDown();
void LBoardMoveUp();