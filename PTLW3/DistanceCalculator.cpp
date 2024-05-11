#include <Windows.h>
#include <iostream>
#include <vector>
#include <sstream>
// stupid define
#define OnButtonClick	1



INT INF = 9999;

using namespace std;
 
// stupid init
int nodes;

HWND** adj_matrix;
HWND vertexCount;
HWND startVertex;

WNDCLASS ParentWindow;
WNDCLASS ChildWindow;
WNDCLASS PreWindow;

HWND WindowToLock;
HWND ResLabel;

LPCWSTR distToString(vector<int>& vecInt) {
	wstringstream ss;

	// Convert each integer to a string and append to stringstream
	ss << "( ";
	for (int i : vecInt) {
		ss << i << ", ";
	}
	ss << " )";

	// Retrieve the resulting string from the stringstream
	wstring result = ss.str();

	// Allocate memory for the new string
	wchar_t* buffer = new wchar_t[result.size() + 1];

	// Copy the string contents to the allocated buffer
	wcscpy_s(buffer, result.size() + 1, result.c_str());

	// Return the pointer to the buffer
	return buffer;
}


int GetVal(HWND hWndEdit) {
	char buffer[10];
	int adjValue = 0; 

	if (GetWindowTextA(hWndEdit, buffer, sizeof(buffer)) > 0) {
		
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
			adj_val = GetVal(adj_matrix[u][v]);
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
				adj_matrix[i][j] = CreateWindowA("edit", "0", WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_CENTER | ES_READONLY, 200 + 40 * j, 200 + 40 * i, 30, 30, hwnd, NULL, NULL, NULL);
			}
			else 
			{
				adj_matrix[i][j] = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_CENTER, 200 + 40 * j, 200 + 40 * i, 30, 30, hwnd, NULL, NULL, NULL);
			}
		}
	}
	CreateWindowA("button", "Enter", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 200, 400, 90, 30, hwnd, (HMENU)OnButtonClick, NULL, NULL);
}
void DjikstraBuilder(HWND hwnd)
{
	
	// info labels 4 users
	CreateWindowA("static", "Start point:", WS_VISIBLE | WS_CHILD | ES_CENTER, 200, 200 , 45, 30, hwnd, NULL, NULL, NULL);
	// edit labels 
	startVertex =  CreateWindowA("edit", "0", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 260, 200, 30, 30, hwnd, NULL, NULL, NULL);
	CreateWindowA("button", "Calculate", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 200, 240, 90, 30, hwnd, (HMENU)OnButtonClick, NULL, NULL);
}


void PreBuilder(HWND hwnd)
{

	// info labels 4 users
	
	CreateWindowA("static", "Vertex count:", WS_VISIBLE | WS_CHILD | ES_CENTER, 200, 200, 45, 30, hwnd, NULL, NULL, NULL);
	// edit labels 
	vertexCount = CreateWindowA("edit", "3", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 260, 200, 30, 30, hwnd, NULL, NULL, NULL);
	CreateWindowA("button", "Enter", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 200, 240, 90, 30, hwnd, (HMENU)OnButtonClick, NULL, NULL);
	ResLabel = CreateWindowA("static", "", WS_VISIBLE | WS_CHILD | ES_CENTER, 200, 280, 120, 30, hwnd, NULL, NULL, NULL);
}


LRESULT CALLBACK PreProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:
		PreBuilder(hWnd);
		break;
	case WM_COMMAND:
		switch (wp)
		{
		case OnButtonClick:
			nodes = GetVal(vertexCount);
			if (nodes > 0 && nodes <= 5) {
				CreateWindow(L"ParentWindow", L"ADJ Matrix", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 600, 100, 500, 500, NULL, NULL, NULL, NULL);
				WindowToLock = hWnd;
				EnableWindow(WindowToLock, false);
			}
			else
			{
				SetWindowText(ResLabel, L"Vertex valid range [1,5]");
				 
			}
			break;
		default: break;
		}
		break;
	case WM_DESTROY:
		
		break;
	default:return DefWindowProc(hWnd, msg, wp, lp);
	}
}


LRESULT CALLBACK ParentProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
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
		case OnButtonClick:
			
			CreateWindow(L"ChildWindow", L"Distance Calculator", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 600, 100, 500, 500, NULL, NULL, NULL, NULL);
			ShowWindow(hWnd, SW_HIDE);
			
			
		default: break;
		}
	 case WM_DESTROY: break;

	
	default:return DefWindowProc(hWnd, msg, wp, lp);
	}
}
LRESULT CALLBACK ChildProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	LPCWSTR message;
	string test_msg;
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
		case OnButtonClick:
			start_node = GetVal(startVertex);
			
			
			if (start_node < nodes)
			{
				dist = dijkstra(adj_matrix, start_node);
				message = distToString(dist);
				SetWindowText(ResLabel,message);
			}
			else 
			{
				SetWindowText(ResLabel, L"Invalid vertex");
				
			}
			EnableWindow(WindowToLock, true);
			DestroyWindow(hWnd);
			break;
		default: break;
		}
		
	case WM_DESTROY:
		// PostQuitMessage(0);
		break;
	default:return DefWindowProc(hWnd, msg, wp, lp);
	}
}



int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR args, int ncmdshow)
{
	 ParentWindow = BaseWindow((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(NULL, IDI_WINLOGO),
	L"ParentWindow", ParentProcedure);

	 PreWindow = BaseWindow((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(NULL, IDI_WINLOGO),
		L"PreWindow", PreProcedure);

	 ChildWindow = BaseWindow((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(NULL, IDI_WINLOGO),
	L"ChildWindow",ChildProcedure);

	
	if (!RegisterClassW(&ParentWindow)) { return -1; }
	if (!RegisterClassW(&ChildWindow)) { return -1; }
	if (!RegisterClassW(&PreWindow)) { return -1; }

	MSG msg = { 0 };
	
	CreateWindow(L"PreWindow", L"Vertex count window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);
	
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;

}

