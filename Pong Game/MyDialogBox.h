#pragma once
#include <Windows.h>

HWND CreateDialogBox(HINSTANCE hInstance);
void ShowDialogExitProgram(HWND hWndDialog, HWND hWndParent);
void ShowHelpDialog(HWND hWndDialog, HWND hWndParent);
void ShowPauseDialog(HWND hWndDialog, HWND hWndParent);