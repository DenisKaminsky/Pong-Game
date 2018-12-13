#pragma once
#include <Windows.h>
#include "Graphics.h"

void StartGame(HWND hWnd, bool isWithBotMode, int difficulty);
void StopGame();
void DeleteGameParams();