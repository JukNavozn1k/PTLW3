#include <Windows.h>
#include <cstdlib> // for atoi
#include <string> // for std::string
#include <iostream>

#include <vector>
#include <queue>
#include <sstream>
// stupid define
#define OnEnterButtonClick	1
# define OnCalculateButtonClick	2


INT INF = 9999;

using namespace std;
 
// stupid init
int nodes = 3;

HWND** adj_matrix;


string convertToCString(const vector<int>& dist) {
	stringstream ss;
	for (int i = 0; i < dist.size(); ++i) {
		ss << dist[i];
		if (i != dist.size() - 1) {
			ss << ", ";
		}
	}
	return ss.str();
}


int GetAdjVal(HWND hWndEdit) {
	char buffer[10]; // My PC can't handle more ((( 
	int adjValue = 0; 

	if (GetWindowTextA(hWndEdit, buffer, sizeof(buffer)) > 0) {
		// Bullshit check
		adjValue = atoi(buffer);
	}

	return adjValue;
}

vector<int> dijkstra(HWND** adj_matrix, int source) {
    int n = nodes;
	int adj_val;
    vector<int> dist(n, INF);
    vector<bool> visited(n, false);
    dist[source] = 0;

    for (int count = 0; count < n - 1; ++count) {
        int minDist = INF;
        int u = -1;

        for (int v = 0; v < n; ++v) {
            if (!visited[v] && dist[v] < minDist) {
                minDist = dist[v];
                u = v;
            }
        }

        if (u == -1) break; // If there is no path from source to remaining vertices

        visited[u] = true;

        for (int v = 0; v < n; ++v) {
			adj_val = GetAdjVal(adj_matrix[u][v]);
			if (adj_val == 0) { adj_val = INF; }
            if (!visited[v] && adj_val && dist[u] != INF && dist[u] + adj_val < dist[v]) {
				dist[v] = dist[u] + adj_val;
            }
        }
    }
    return dist;
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
	string str_num;
	

	adj_matrix = new HWND * [nodes];
	for (int i = 0; i < nodes; i ++)
	{
		adj_matrix[i] = new HWND[nodes];
		str_num = to_string(i);
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
	string message;
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
			adjValue = GetAdjVal(adj_matrix[0][1]);
			message =  "Adjusted Value: " + to_string(adjValue);
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
	string message;
	int adjValue;
	int start_node;// Example start node
	vector<int> dist;
	switch (msg)
	{
	case WM_CREATE:
		DjikstraBuilder(hWnd);
		break;
	case WM_COMMAND:
		switch (wp)
		{
		case OnCalculateButtonClick:
			start_node = 0;
			dist = dijkstra(adj_matrix, 0);
			message = convertToCString(dist);
			MessageBoxA(NULL, message.c_str(), "Distances", MB_OK | MB_ICONINFORMATION);
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
	

	CreateWindow(L"ParentWindow",L"ADJ Matrix",  WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);
	CreateWindow(L"ChildWindow", L"Distance Calculator", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 600, 100, 500, 500, NULL, NULL, NULL, NULL);

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;

}

