#include <Windows.h>
#include <cstdlib> // for atoi
#include <string> // for std::string
#include <iostream>


// stupid define
#define OnEnterButtonClick	1
# define OnCalculateButtonClick	2

// stupid init
int nodes = 3;

HWND** adj_matrix;


int GetAdjVal(HWND hWndEdit) {
	char buffer[10]; // My PC can't handle more ((( 
	int adjValue = 0; 

	if (GetWindowTextA(hWndEdit, buffer, sizeof(buffer)) > 0) {
		// Bullshit check
		adjValue = atoi(buffer);
	}

	return adjValue;
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
	std::string str_num;
	

	adj_matrix = new HWND * [nodes];
	for (int i = 0; i < nodes; i ++)
	{
		adj_matrix[i] = new HWND[nodes];
		str_num = std::to_string(i);
		CreateWindowA("static", str_num.c_str(), WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_CENTER, 200 - 40, 200 + i * 40, 30, 30, hwnd, NULL, NULL, NULL);

		CreateWindowA("static", str_num.c_str(), WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_CENTER, 200 + 40*i, 200 - 40, 30, 30, hwnd, NULL, NULL, NULL);
		for (int j = 0; j < nodes; j ++)
		{
			if (i == j)
			{
				adj_matrix[i][j] = CreateWindowA("edit", "0", WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_CENTER | ES_READONLY, 200 + 40 * i, 200 + 40 * j, 30, 30, hwnd, NULL, NULL, NULL);
			}
			else 
			{
				adj_matrix[i][j] = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_CENTER, 200 + 40 * i, 200 + 40 * j, 30, 30, hwnd, NULL, NULL, NULL);
			}
		}
	}

	CreateWindowA("button", "Enter", WS_VISIBLE | WS_CHILD, 200, 200 + 40*nodes, 40*nodes, 25, hwnd, (HMENU) OnEnterButtonClick, NULL, NULL);
}
void DjikstraBuilder(HWND hwnd)
{
	// info labels 4 users
	CreateWindowA("static", "Start point:", WS_VISIBLE | WS_CHILD | ES_CENTER, 200, 200 , 45, 30, hwnd, NULL, NULL, NULL);
	// edit labels 
	CreateWindowA("edit", "0", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 260, 200, 30, 30, hwnd, NULL, NULL, NULL);
	CreateWindowA("button", "Calculate", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 200, 240, 90, 30, hwnd, (HMENU) OnCalculateButtonClick, NULL, NULL);
}


LRESULT CALLBACK ParentProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	// All bullshit handler
	std::string message;
	int adjValue;

	switch (msg)
	{
	case WM_CREATE:
		AdjBuilder(hWnd);
		break;
	case WM_COMMAND:
		switch (wp) 
		{
		case OnEnterButtonClick:
			adjValue = GetAdjVal(adj_matrix[0][0]);
			message =  "Adjusted Value: " + std::to_string(adjValue);
			MessageBoxA(NULL, message.c_str(), "Adjusted Value", MB_OK | MB_ICONINFORMATION);
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
LRESULT CALLBACK ChildProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	// All bullshit handler
	std::string message;
	int adjValue;

	switch (msg)
	{
	case WM_CREATE:
		DjikstraBuilder(hWnd);
		break;
	case WM_COMMAND:
		switch (wp)
		{
		case OnCalculateButtonClick:
			
			MessageBoxA(NULL, "Some data...", "Distances", MB_OK | MB_ICONINFORMATION);
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
		L"ParentWindow", ParentProcedure);

	WNDCLASS ChildWindow = BaseWindow((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(NULL, IDI_WINLOGO),
		L"ChildWindow",ChildProcedure);

	
	if (!RegisterClassW(&ParentWindow)) { return -1; }
	if (!RegisterClassW(&ChildWindow)) { return -1; }

	MSG msg = { 0 };
	

	HWND pw = CreateWindow(L"ParentWindow",L"ADJ Matrix",  WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);
	CreateWindow(L"ChildWindow", L"Distance Calculator", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 600, 100, 500, 500, pw, NULL, NULL, NULL);

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;

}

