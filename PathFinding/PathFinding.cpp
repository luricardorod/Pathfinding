// PathFinding.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PathFinding.h"
#include <windows.h>
#include <vector>
#include "CGraph.h"
#include "CDFS.h"
#include "CBFS.h"
#include "CBestFS.h"
#include "CDijkstra.h"
#include "CAStar.h"
#define MAX_LOADSTRING 100
#define IDC_BUTTON_CREATE_GRAPH	(WM_USER+1)
#define ID_PATH_BFS (WM_USER+2)
#define ID_PATH_BESTFS (WM_USER+3)
#define ID_PATH_DFS (WM_USER+4)
#define ID_PATH_ASTAR (WM_USER+5)
#define ID_PATH_DIJKSTRA (WM_USER+6)
#define ID_EDIT_COLUMNS (WM_USER+7)
#define ID_EDIT_ROWS (WM_USER+8)
#define ID_HEURISTIC_EUCLIDIAN (WM_USER+9)
#define ID_HEURISTIC_QUADRATIC (WM_USER+10)
#define ID_HEURISTIC_MANHATTAN (WM_USER+11)
#define ID_CONTROLS_START (WM_USER+12)
#define ID_CONTROLS_END (WM_USER+13)
#define ID_CONTROLS_DISCONNECT (WM_USER+14)
#define ID_CONTROLS_CHANGE_VALUE (WM_USER+15)
#define ID_CONTROLS_EDIT_CHANGE_VALUE (WM_USER+16)
#define IDC_BUTTON_CREATE_PATH (WM_USER+17)

// Global Variables:
CGraph graph;
int iSelectedHeuristic = ID_HEURISTIC_EUCLIDIAN;
int iSelectedPathfinding = ID_PATH_DFS;
int iSelectedControl = ID_CONTROLS_START;

CWalker* walker;
CDFS cdfs;
CBFS cbfs;
CBestFS cbestfs;
CDijkstra cdijkstra;
CAStar castar;

int maxColor = 10;
HWND hWndButton2;
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

												// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PATHFINDING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PATHFINDING));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PATHFINDING));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PATHFINDING);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void FindPath() {
	HEURISTIC_TYPE::E tempHeuristic = HEURISTIC_TYPE::kLINEAL;
	switch (iSelectedHeuristic)
	{
	case ID_HEURISTIC_EUCLIDIAN:
		tempHeuristic = HEURISTIC_TYPE::kLINEAL;
		break;
	case ID_HEURISTIC_MANHATTAN:
		tempHeuristic = HEURISTIC_TYPE::kMANHATTAN;
		break;
	case ID_HEURISTIC_QUADRATIC:
		tempHeuristic = HEURISTIC_TYPE::kQUADRATIC;
		break;
	default:
		break;
	}
	switch (iSelectedPathfinding)
	{
	case ID_PATH_DFS:
		walker = &cdfs;
		break;
	case ID_PATH_BFS:
		walker = &cbfs;
		break;
	case ID_PATH_BESTFS:
		walker = &cbestfs;
		break;
	case ID_PATH_DIJKSTRA:
		walker = &cdijkstra;
		break;
	case ID_PATH_ASTAR:
		walker = &castar;
		break;
	default:
		break;
	}
	if (graph.m_start != NULL && graph.m_end != NULL)
	{
		walker->SetStart(graph.m_start);
		walker->SetEnd(graph.m_end);
		walker->SetHeuristic(tempHeuristic);
		walker->RunPathFinding();
		walker->PathFinding();
	}
}
void PaintNodes(HDC hdc, int rows, int columns, HWND window, float sizeX, float sizeY) {
	static HBRUSH startBrush = CreateSolidBrush(RGB(5, 255, 5));
	static HBRUSH endBrush = CreateSolidBrush(RGB(5, 5, 255));
	static HBRUSH disconectBrush = CreateSolidBrush(RGB(0, 0, 0));
	static HBRUSH oldBrush;
	HBRUSH tempBrush;
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < columns; x++)
		{
			if (graph.m_start == &graph.m_graph[x + y*columns])
			{
				oldBrush = HBRUSH(SelectObject(hdc, startBrush));
				Rectangle(hdc, 240 + x*sizeX, y*sizeY, 240 + (x + 1)*sizeX, (y + 1)*sizeY);
				SelectObject(hdc, oldBrush);
			}
			else if (graph.m_end == &graph.m_graph[x + y*columns])
			{
				oldBrush = HBRUSH(SelectObject(hdc, endBrush));
				Rectangle(hdc, 240 + x*sizeX, y*sizeY, 240 + (x + 1)*sizeX, (y + 1)*sizeY);
				SelectObject(hdc, oldBrush);
			}
			else if (graph.m_graph[x + y*columns].m_conections.size() == 0)
			{
				oldBrush = HBRUSH(SelectObject(hdc, disconectBrush));
				Rectangle(hdc, 240 + x*sizeX, y*sizeY, 240 + (x + 1)*sizeX, (y + 1)*sizeY);
				SelectObject(hdc, oldBrush);
			}
			else
			{
				float color = graph.m_graph[x + y*columns].m_conectionValue * 220 /maxColor;
				color = 255 - color;
				tempBrush = CreateSolidBrush(RGB(color, color, color));
				oldBrush = HBRUSH(SelectObject(hdc, tempBrush));
				Rectangle(hdc, 240 + x*sizeX, y*sizeY, 240 + (x + 1)*sizeX, (y + 1)*sizeY);
				DeleteObject(SelectObject(hdc, oldBrush));
			}
		}
	}
}

void DrawPath(HDC hdc) {
	if (walker == NULL)
	{
		return;
	}
	std::vector<node*> nodes = walker->PathFinding();
	static HPEN pathColor = CreatePen(0, 2, RGB(255, 5, 5));
	static HPEN oldPen;

	oldPen = HPEN(SelectObject(hdc, pathColor));
	for (auto node = nodes.begin(); node != nodes.end(); node++)
	{
		MoveToEx(hdc, (*node)->m_position.x, (*node)->m_position.y, NULL);
		node++;
		if (node == nodes.end())
		{
			break;
		}
		LineTo(hdc, (*node)->m_position.x, (*node)->m_position.y);
		node--;
	}
	SelectObject(hdc, oldPen);
}
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int columns, rows;
	static float sizeX = 0, sizeY = 0;
	int iCordX, iCordY;
	HWND filedit;
	TCHAR buff[5];
	RECT rc;
	switch (message)
	{
	case WM_CREATE:
	{
		RECT rect;
		GetWindowRect(hWnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

		CreateWindowW(L"Static", L"",
			WS_CHILD | WS_VISIBLE | SS_LEFT,
			0, 0, 240, height,
			hWnd, (HMENU)1, NULL, NULL);

		CreateWindowW(L"static", L"Grafo",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			5, 20, 100, 20,
			hWnd, (HMENU)(501),
			hInstance, NULL);

		CreateWindowW(L"static", L"Columnas:",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			10, 40, 120, 20,
			hWnd, (HMENU)(501),
			hInstance, NULL);

		CreateWindowW(L"Edit", NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			80, 40, 50, 20,
			hWnd, (HMENU)ID_EDIT_COLUMNS,
			NULL, NULL);

		CreateWindowW(L"static", L"Filas:",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			140, 40, 50, 20,
			hWnd, (HMENU)(501),
			hInstance, NULL);

		CreateWindowW(L"Edit", NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			180, 40, 50, 20,
			hWnd, (HMENU)ID_EDIT_ROWS,
			NULL, NULL);

		CreateWindowW(L"Button", L"Generar",
			WS_VISIBLE | WS_CHILD,
			130, 70, 80, 25,
			hWnd, (HMENU)IDC_BUTTON_CREATE_GRAPH, NULL, NULL);

		CreateWindowW(L"static", L"Pathfinding",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			5, 110, 100, 20,
			hWnd, (HMENU)(501),
			hInstance, NULL);

		CreateWindowW(L"BUTTON", L"DFS",
			WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			10, 130, 100, 20,
			hWnd, (HMENU)ID_PATH_DFS,
			hInstance, NULL);

		CreateWindowW(L"BUTTON", L"BFS",
			WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			10, 150, 100, 20,
			hWnd, (HMENU)ID_PATH_BFS,
			hInstance, NULL);

		CreateWindowW(L"BUTTON", L"BestFS",
			WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			10, 170, 100, 20,
			hWnd, (HMENU)ID_PATH_BESTFS,
			hInstance, NULL);

		CreateWindowW(L"BUTTON", L"DIJKSTRA",
			WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			10, 190, 100, 20,
			hWnd, (HMENU)ID_PATH_DIJKSTRA,
			hInstance, NULL);

		CreateWindowW(L"BUTTON", L"A*",
			WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			10, 210, 100, 20,
			hWnd, (HMENU)ID_PATH_ASTAR,
			hInstance, NULL);


		CreateWindowW(L"static", L"Heurística",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			5, 250, 100, 20,
			hWnd, (HMENU)(501),
			hInstance, NULL);

		CreateWindowW(L"BUTTON", L"Euclidiana",
			WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			10, 270, 100, 20,
			hWnd, (HMENU)ID_HEURISTIC_EUCLIDIAN,
			hInstance, NULL);

		CreateWindowW(L"BUTTON", L"Cuadratica",
			WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			10, 290, 100, 20,
			hWnd, (HMENU)ID_HEURISTIC_QUADRATIC,
			hInstance, NULL);

		CreateWindowW(L"BUTTON", L"Manhattan",
			WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			10, 310, 100, 20,
			hWnd, (HMENU)ID_HEURISTIC_MANHATTAN,
			hInstance, NULL);

		CreateWindowW(L"static", L"Controls",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			5, 350, 100, 20,
			hWnd, (HMENU)(501),
			hInstance, NULL);

		CreateWindowW(L"BUTTON", L"Nodo de inicio",
			WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			10, 370, 120, 20,
			hWnd, (HMENU)ID_CONTROLS_START,
			hInstance, NULL);

		CreateWindowW(L"BUTTON", L"Nodo final",
			WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			10, 390, 100, 20,
			hWnd, (HMENU)ID_CONTROLS_END,
			hInstance, NULL);

		CreateWindowW(L"BUTTON", L"Cambiar valor:",
			WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			10, 410, 120, 20,
			hWnd, (HMENU)ID_CONTROLS_CHANGE_VALUE,
			hInstance, NULL);

		CreateWindowW(L"Edit", NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			140, 410, 90, 20,
			hWnd, (HMENU)ID_CONTROLS_EDIT_CHANGE_VALUE,
			NULL, NULL);

		CreateWindowW(L"BUTTON", L"Deconectar",
			WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			10, 430, 120, 20,
			hWnd, (HMENU)ID_CONTROLS_DISCONNECT,
			hInstance, NULL);

		CreateWindowW(L"Button", L"Ver Ruta",
			WS_VISIBLE | WS_CHILD,
			130, 460, 80, 25,
			hWnd, (HMENU)IDC_BUTTON_CREATE_PATH, NULL, NULL);
		SendDlgItemMessage(hWnd, ID_PATH_DFS, BM_SETCHECK, 1, 0);
		SendDlgItemMessage(hWnd, ID_HEURISTIC_EUCLIDIAN, BM_SETCHECK, 1, 0);
		SendDlgItemMessage(hWnd, ID_CONTROLS_START, BM_SETCHECK, 1, 0);

		SetDlgItemText(hWnd, ID_EDIT_COLUMNS, TEXT("10"));
		SetDlgItemText(hWnd, ID_EDIT_ROWS, TEXT("10"));
		SetDlgItemText(hWnd, ID_CONTROLS_EDIT_CHANGE_VALUE, TEXT("10"));
		columns = 10;
		rows = 10;

		sizeX = (width - 240.0f) / columns;
		sizeY = (height - 50.0f) / rows;
		graph.CreateGraph(rows, columns, sizeX / 2, sizeY / 2);
		graph.SetStart(&graph.m_graph[0]);
		graph.SetEnd(&graph.m_graph[graph.m_graph.size()-1]);

		break;
	}
	case WM_LBUTTONUP:
		iCordX = (LOWORD(lParam) - 240) / sizeX;
		iCordY = (HIWORD(lParam)) / sizeY;
		switch (iSelectedControl)
		{
		case ID_CONTROLS_START:
			graph.SetStart(&graph.m_graph[iCordX+columns*iCordY]);
			break;
		case ID_CONTROLS_END:
			graph.SetEnd(&graph.m_graph[iCordX + columns*iCordY]);
			break;
		case ID_CONTROLS_DISCONNECT:
			graph.DisconectNode(&graph.m_graph[iCordX + columns*iCordY]);
			break;
		case ID_CONTROLS_CHANGE_VALUE:
			filedit = GetDlgItem(hWnd, ID_CONTROLS_EDIT_CHANGE_VALUE); // I tried with and without this
			GetWindowText(filedit, buff, 5);
			if (maxColor <  _tstof(buff))
			{
				maxColor = _tstof(buff);
			}
			graph.ChangeValueConnections(&graph.m_graph[iCordX + columns*iCordY], _tstof(buff));
			break;
		default:
			break;
		}
		GetWindowRect(hWnd, &rc);
		rc.left =240;
		rc.top = 0;
		InvalidateRect(hWnd, &rc, true);
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		if (wmId >= ID_PATH_BFS && wmId <= ID_PATH_DIJKSTRA)
		{
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				SendDlgItemMessage(hWnd, ID_PATH_BFS, BM_SETCHECK, 0, 0);
				SendDlgItemMessage(hWnd, ID_PATH_DFS, BM_SETCHECK, 0, 0);
				SendDlgItemMessage(hWnd, ID_PATH_BESTFS, BM_SETCHECK, 0, 0);
				SendDlgItemMessage(hWnd, ID_PATH_DIJKSTRA, BM_SETCHECK, 0, 0);
				SendDlgItemMessage(hWnd, ID_PATH_ASTAR, BM_SETCHECK, 0, 0);
				SendDlgItemMessage(hWnd, wmId, BM_SETCHECK, 1, 0);
				iSelectedPathfinding = wmId;
				break;
			}
		}

		if (wmId >= ID_HEURISTIC_EUCLIDIAN && wmId <= ID_HEURISTIC_MANHATTAN)
		{
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				SendDlgItemMessage(hWnd, ID_HEURISTIC_EUCLIDIAN, BM_SETCHECK, 0, 0);
				SendDlgItemMessage(hWnd, ID_HEURISTIC_QUADRATIC, BM_SETCHECK, 0, 0);
				SendDlgItemMessage(hWnd, ID_HEURISTIC_MANHATTAN, BM_SETCHECK, 0, 0);
				SendDlgItemMessage(hWnd, wmId, BM_SETCHECK, 1, 0);
				iSelectedHeuristic = wmId;
				break;
			}
		}
		if (wmId >= ID_CONTROLS_START && wmId <= ID_CONTROLS_EDIT_CHANGE_VALUE)
		{
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				SendDlgItemMessage(hWnd, ID_CONTROLS_START, BM_SETCHECK, 0, 0);
				SendDlgItemMessage(hWnd, ID_CONTROLS_END, BM_SETCHECK, 0, 0);
				SendDlgItemMessage(hWnd, ID_CONTROLS_DISCONNECT, BM_SETCHECK, 0, 0);
				SendDlgItemMessage(hWnd, ID_CONTROLS_CHANGE_VALUE, BM_SETCHECK, 0, 0);
				SendDlgItemMessage(hWnd, wmId, BM_SETCHECK, 1, 0);
				iSelectedControl = wmId;
				break;
			}
		}
		
		switch (wmId)
		{
		case IDC_BUTTON_CREATE_PATH:
			FindPath();
			GetWindowRect(hWnd, &rc);
			rc.left = 240;
			rc.top = 0;
			InvalidateRect(hWnd, &rc, true);
			break;
		case IDC_BUTTON_CREATE_GRAPH:
			RECT rect;

			filedit = GetDlgItem(hWnd, ID_EDIT_COLUMNS); // I tried with and without this
			GetWindowText(filedit, buff, 5);
			columns = _tstoi(buff);

			filedit = GetDlgItem(hWnd, ID_EDIT_ROWS); // I tried with and without this
			GetWindowText(filedit, buff, 5);
			rows = _tstoi(buff);

			GetWindowRect(hWnd, &rect);
			sizeX = ((rect.right - rect.left - 240.0f) / columns)*.95f;
			sizeY = ((rect.bottom - rect.top - 50.0f) / rows)*.95f;
			graph.CreateGraph(rows, columns, sizeX / 2, sizeY / 2);

			graph.SetStart(&graph.m_graph[0]);
			graph.SetEnd(&graph.m_graph[graph.m_graph.size() - 1]);
			walker = NULL;
			GetWindowRect(hWnd, &rc);
			rc.left = 240;
			rc.top = 0;
			InvalidateRect(hWnd, &rc, true);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		PaintNodes(hdc, rows, columns, hWnd, sizeX, sizeY);
		DrawPath(hdc);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
