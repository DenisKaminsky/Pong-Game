#pragma once
#include <Windows.h>

HWND CreateDialogBox(HINSTANCE hInstance);
void ShowExitDialog(HWND hWndDialog, HWND hWndParent,bool isExitProgram);
void ShowHelpDialog(HWND hWndDialog, HWND hWndParent);