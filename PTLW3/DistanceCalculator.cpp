﻿#include <Windows.h>


// stupid define
#define OnEnterButtonClick	1

// stupid init
int nodes = 3;

HWND** adj_matrix;


int GetAdjVal(HWND hWndEdit) {
	char buffer[10]; // Max memory for my computer
	GetWindowTextA(hWndEdit, buffer, sizeof(buffer));
	return atoi(buffer); // Less Bullshit for Djikstra
}

WNDCLASS BaseWindow(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure)
{
	WNDCLASS BWC = { 0 };

	BWC.hIcon = Icon;
	BWC.hCursor = Cursor;
	BWC.hInstance = hInst;
	BWC.hIcon = Icon;
	BWC.lpszClassName = Name;
	BWC.hbrBackground = BGColor;
	BWC.lpfnWndProc = Procedure;

	return BWC;
}

void AdjBuilder(HWND hwnd)
{
	// Poopy grid
	adj_matrix = new HWND * [nodes];
	for (int i = 0; i < nodes; i ++)
	{
		adj_matrix[i] = new HWND[nodes];
		for (int j = 0; j < nodes; j ++)
		{
			adj_matrix[i][j] = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_CENTER, 200 + 40 * i , 200 + 40 * j, 30, 30, hwnd, NULL, NULL, NULL);
		}
	}

	CreateWindowA("button", "Enter", WS_VISIBLE | WS_CHILD, 5, 50, 100, 25, hwnd, (HMENU) OnEnterButtonClick, NULL, NULL);
}


LRESULT CALLBACK BaseProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	// All bullshit handler
	
	switch (msg)
	{
	case WM_CREATE:
		AdjBuilder(hWnd);
		break;
	case WM_COMMAND:
		switch (wp) 
		{
		case OnEnterButtonClick:
			
			break;
		default: break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:return DefWindowProc(hWnd, msg, wp, lp);
	}
}

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR args, int ncmdshow)
{
	WNDCLASS ParentWindow = BaseWindow((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(NULL, IDI_WINLOGO),
		L"ParentWindow", BaseProcedure);

	
	if (!RegisterClassW(&ParentWindow)) { return -1; }

	MSG msg = { 0 };
	

	CreateWindow(L"ParentWindow",L"ADJ Matrix",  WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;

}
