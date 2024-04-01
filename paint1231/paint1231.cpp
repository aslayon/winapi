// paint1231.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "paint1231.h"
#include <windowsx.h>
#define MAX_LOADSTRING 100
#include <iostream>
#include <string.h>
HDC hdc;
void CreateButton(const WCHAR* name, LONG x, LONG y, LONG width, LONG height, HMENU id, HWND hWnd, HINSTANCE hInst);

void CreateRGBTable(const WCHAR* name, LONG left, LONG top, LONG right, LONG bottom, HMENU id, HWND hWnd, HINSTANCE hInst);

void SetMode(WPARAM wParam, LPARAM lParam,HWND hWnd);
void SetScrollFunction(WPARAM wParam, LPARAM lParam);
void SetColor(HWND hWnd,HDC hdc);
void CreateBackPage(HWND hWnd, HINSTANCE hInst, HDC* memDC , HBITMAP* memBitmap);
POINT Draw(HWND hWnd, HDC hdc, WPARAM wParam, LPARAM lParam,POINT stPos) ;
POINT Eraser(HWND hWnd, HDC hdc, WPARAM wParam, LPARAM lParam, POINT stPos);
bool pen = FALSE;
bool eraser = FALSE;
void CreateThicknessTable(const WCHAR* name, LONG left, LONG top, LONG right, LONG bottom, HMENU id, HWND hWnd, HINSTANCE hInst);
void SetThickness(HWND hWnd,HDC hdc);
POINT Drawsquare(HWND hWnd, HDC hdc, WPARAM wParam, LPARAM lParam,POINT stPos) ;
POINT  DrawRectangle(HWND hWnd, HDC hdc, WPARAM wParam, LPARAM lParam);
POINT  DrawCircle(HWND hWnd, HDC hdc, WPARAM wParam, LPARAM lParam);


// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.
HDC memDC;
HBITMAP memBitmap;
POINT stPos;
int thickness = 5;
BOOL bImageChanged = TRUE;
bool circle = FALSE;
bool square = FALSE;
BOOL isDrawing = FALSE;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PAINT1231, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAINT1231));

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
//  ����:
//
//    Windows 95���� �߰��� 'RegisterClassEx' �Լ����� ����
//    �ش� �ڵ尡 Win32 �ý��۰� ȣȯ�ǵ���
//    �Ϸ��� ��쿡�� �� �Լ��� ����մϴ�. �� �Լ��� ȣ���ؾ�
//    �ش� ���� ���α׷��� �����
//    '�ùٸ� ������' ���� �������� ������ �� �ֽ��ϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAINT1231));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PAINT1231);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   hWnd = CreateWindow(szWindowClass, szTitle,  WS_MAXIMIZE| WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
      CW_USEDEFAULT, 0, 1100, 700, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	ReleaseDC(hWnd, hdc);
	hdc = GetDC(hWnd);
	ReleaseDC(hWnd, hdc);
	switch (message)
	{
	case WM_CREATE:{
		CreateButton(L"Pen",800,20,50,50,(HMENU)1, hWnd, hInst); 
        CreateButton(L"Erase", 800, 75, 50, 50, (HMENU)2, hWnd, hInst);
		
		CreateButton(L"1",489,75,25,25,(HMENU)15, hWnd, hInst); 
        CreateButton(L"3", 519, 75, 25, 25, (HMENU)16, hWnd, hInst);
		CreateButton(L"5",549,75,25,25,(HMENU)17, hWnd, hInst); 
        CreateButton(L"7", 579, 75, 25, 25, (HMENU)18, hWnd, hInst);
		CreateButton(L"9",609,75,25,25,(HMENU)19, hWnd, hInst); 
		CreateButton(L"��",855,20,50,50,(HMENU)3, hWnd, hInst); 
        CreateButton(L"��",855,75,50,50,(HMENU)4, hWnd, hInst); 


        //��ư ���� �� �̹��� �����

        //���� ���̺� ����
        CreateRGBTable(L"RGBtable", 20, 30, 0, 120, (HMENU)10, hWnd, hInst);

		//�β� ���̺� ����
		CreateThicknessTable(L"Thickness", 80, 30, 160, 120, (HMENU)30, hWnd, hInst);


		CreateBackPage(hWnd, hInst, &memDC , &memBitmap);
		
		break;
					}
	case WM_COMMAND:
		
	
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case 1:
			pen = TRUE;
			eraser = FALSE;
			circle = FALSE;
			square = FALSE;
			//TextOut(hdc,100,100,L"test1",5);
			break;
		case 2:
			pen = FALSE;
			eraser = TRUE;
			circle = FALSE;
			square = FALSE;
			//TextOut(hdc,100,100,L"test2",5);
			break;
		case 3:
			pen = FALSE;
			eraser = FALSE;
			circle = TRUE;
			square = FALSE;
			//TextOut(hdc,100,100,L"test1",5);
			break;
		case 4:
			pen = FALSE;
			eraser = TRUE;
			circle = FALSE;
			square = TRUE;
			//TextOut(hdc,100,100,L"test2",5);
			break;
		case 15:
			
			SetScrollPos(FindWindowExW(hWnd,NULL,L"scrollbar", L"Thickness"),SB_CTL,1,TRUE);
			thickness = 1;
			SetThickness(hWnd,hdc);
			SetThickness(hWnd,memDC);
			
			break;
		case 16:
			
			SetScrollPos(FindWindowExW(hWnd,NULL,L"scrollbar", L"Thickness"),SB_CTL,3,TRUE);
			thickness = 3;
			InvalidateRect(hWnd, NULL, FALSE);
			UpdateWindow(hWnd);
			
			break;
		case 17:
			
			SetScrollPos(FindWindowExW(hWnd,NULL,L"scrollbar", L"Thickness"),SB_CTL,5,TRUE);
			thickness = 5;
			InvalidateRect(hWnd, NULL, FALSE);
			UpdateWindow(hWnd);
			ReleaseDC(hWnd, hdc);
			break;
		case 18:
		
			SetScrollPos(FindWindowExW(hWnd,NULL,L"scrollbar", L"Thickness"),SB_CTL,7,TRUE);
			thickness = 7;
			InvalidateRect(hWnd, NULL, FALSE);
			UpdateWindow(hWnd);
			ReleaseDC(hWnd, hdc);
			break;
		case 19:
			
			SetScrollPos(FindWindowExW(hWnd,NULL,L"scrollbar", L"Thickness"),SB_CTL,9,TRUE);
			thickness = 9;
			InvalidateRect(hWnd, NULL, FALSE);
			UpdateWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_MOUSEMOVE:{
		hdc = GetDC(hWnd);
	
		RECT rect;
		GetClientRect(hWnd,&rect);
		if (wParam == MK_LBUTTON && stPos.y >=150 && stPos.y <= rect.bottom - rect.top && stPos.x > rect.left && stPos.x < rect.right - rect.left){
			if(pen == TRUE){
				stPos = Draw(hWnd,memDC,wParam,lParam,stPos);
				
				
				BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
				ReleaseDC(hWnd, hdc);
			}
			else if(eraser == TRUE){
				stPos = Eraser(hWnd, memDC, wParam, lParam, stPos);
                
				
				BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
				ReleaseDC(hWnd, hdc);
			}
			else if(square == TRUE){
				
				if(isDrawing == FALSE){
					isDrawing = TRUE;
				}
				HDC hdcBuffer;
				HBITMAP hbmBuffer, hbmOld;
				

    
				hdcBuffer = CreateCompatibleDC(hdc);
				hbmBuffer = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
				hbmOld = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

				BitBlt(hdcBuffer, 0, 0, rect.right, rect.bottom, hdc, 0, 0, SRCCOPY);


				DrawRectangle( hWnd,  hdcBuffer,  wParam,  lParam);


				BitBlt(memDC, 0, 0, rect.right, rect.bottom, hdc, 0, 0, SRCCOPY);
				
				BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcBuffer, 0, 0, SRCCOPY);

				BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
				SelectObject(hdcBuffer, hbmOld);
				DeleteObject(hbmBuffer);
				DeleteDC(hdcBuffer);
			}
			else if(circle == TRUE){
				if(isDrawing == FALSE){
					isDrawing = TRUE;
				}
				HDC hdcBuffer;
				HBITMAP hbmBuffer, hbmOld;
				

    
				hdcBuffer = CreateCompatibleDC(hdc);
				hbmBuffer = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
				hbmOld = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

				BitBlt(hdcBuffer, 0, 0, rect.right, rect.bottom, hdc, 0, 0, SRCCOPY);


				DrawCircle( hWnd,  hdcBuffer,  wParam,  lParam);


				BitBlt(memDC, 0, 0, rect.right, rect.bottom, hdc, 0, 0, SRCCOPY);
				
				BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcBuffer, 0, 0, SRCCOPY);

				BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
				SelectObject(hdcBuffer, hbmOld);
				DeleteObject(hbmBuffer);
				DeleteDC(hdcBuffer);
				
			}
			
		
		}
		break;
					  }

	 case WM_LBUTTONDOWN:
    
        //�� Ŭ���� �ʱ� ��ǥ �� ����
        stPos.x = GET_X_LPARAM(lParam);
        stPos.y = GET_Y_LPARAM(lParam);
        break;
    
    case WM_LBUTTONUP:
		if(isDrawing == TRUE && square == TRUE){
			//TextOut(memDC,100,100,L"test1",5);
			hdc = GetDC(hWnd);
	
			RECT rect;
			GetClientRect(hWnd,&rect);

			DrawCircle( hWnd,  memDC,  wParam,  lParam);
			BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
			isDrawing = FALSE;
			ReleaseDC(hWnd,hdc);
		}
		if(isDrawing == TRUE && circle == TRUE){
			//TextOut(memDC,100,100,L"test1",5);
			hdc = GetDC(hWnd);
	
			RECT rect;
			GetClientRect(hWnd,&rect);

			DrawCircle( hWnd,  memDC,  wParam,  lParam);
			BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
			isDrawing = FALSE;
			ReleaseDC(hWnd,hdc);
		}
        break;
    
    case WM_RBUTTONDOWN:
    
        break;
    
    case WM_RBUTTONUP:
    
        break;
    
	case WM_PAINT:
		PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);
		SetColor(hWnd,hdc);
        SetColor(hWnd,memDC);
		SetThickness(hWnd,hdc);
		SetThickness(hWnd,memDC);


		


        EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		DeleteObject(memBitmap);
        DeleteDC(memDC);
        PostQuitMessage(0);
		break;
	 case WM_HSCROLL:{
		hdc = GetDC(hWnd);
		HWND hScrollBar = (HWND)lParam;
        int scrollBarId = LOWORD(wParam);
        SetScrollFunction(wParam, lParam);
		if(scrollBarId >=10 && scrollBarId <=12)
			SetColor(hWnd,hdc);
		else{
			thickness = GetScrollPos(FindWindowExW(hWnd, NULL, L"scrollbar", L"Thickness"),SB_CTL);
			wchar_t temp[10];
			wsprintf(temp,L"%d",thickness);
			TextOut(hdc,440,100,temp,lstrlenW(temp));
			SetThickness(hWnd,hdc);
		}
        InvalidateRect(hWnd, NULL, FALSE);
        UpdateWindow(hWnd);
		ReleaseDC(hWnd, hdc);
        break;
					 }
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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

void CreateButton(const WCHAR* name,LONG x,LONG y,LONG width ,LONG height, HMENU id, HWND hWnd, HINSTANCE hInst)
{
    CreateWindowW(L"button", name, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x,y,width,height, hWnd, id, hInst,NULL);
}

void CreateRGBTable(const WCHAR* name, LONG left, LONG top, LONG right, LONG bottom, HMENU id, HWND hWnd, HINSTANCE hInst)
{
    
    //RGB�� ��ũ�� �� ���� �� ���� ����
    CreateWindowW(L"scrollbar", L"R", WS_CHILD | WS_VISIBLE | SBS_HORZ, right+20, top+10, 180, 15, hWnd, id, hInst, NULL);
    SetScrollRange(FindWindowExW(hWnd,NULL,L"scrollbar", L"R"), SB_CTL, 0, 255, TRUE);
    CreateWindowW(L"scrollbar", L"G", WS_CHILD | WS_VISIBLE | SBS_HORZ, right+20, top+35, 180, 15, hWnd, id+1, hInst, NULL);
    SetScrollRange(FindWindowExW(hWnd,NULL,L"scrollbar", L"G"), SB_CTL, 0, 255, TRUE);
    CreateWindowW(L"scrollbar", L"B", WS_CHILD | WS_VISIBLE | SBS_HORZ, right+20, top+60, 180, 15, hWnd, id+2, hInst, NULL);
    SetScrollRange(FindWindowExW(hWnd,NULL,L"scrollbar", L"B"), SB_CTL, 0, 255, TRUE);
    SetColor(hWnd,GetDC(hWnd));
}


void CreateThicknessTable(const WCHAR* name, LONG left, LONG top, LONG right, LONG bottom, HMENU id, HWND hWnd, HINSTANCE hInst)
{
    
    hdc = GetDC(hWnd);
    HWND ID = CreateWindowW(L"scrollbar", L"Thickness", WS_CHILD | WS_VISIBLE | SBS_HORZ, right+330, top+15, right + 80, 15, hWnd, id, hInst, NULL);
    SetScrollRange(FindWindowExW(hWnd,NULL,L"scrollbar", L"Thickness"), SB_CTL, 0, 99, TRUE);
	SetScrollPos(ID,SB_CTL,5,TRUE);
    SetThickness(hWnd,hdc);
	ReleaseDC(hWnd, hdc);
	return;
}


void SetScrollFunction(WPARAM wParam, LPARAM lParam)
{
	int scrollBarId = LOWORD(wParam);
    switch (LOWORD(wParam))
    {
    case SB_LINELEFT:
        SetScrollPos((HWND)lParam, SB_CTL, max(0,GetScrollPos((HWND)lParam,SB_CTL)-1), TRUE);
		//if(scrollBarId == 13) thickness--;
		//if(thickness <0) thickness = 0;
        break;
    case SB_PAGELEFT:
        SetScrollPos((HWND)lParam, SB_CTL, max(0, GetScrollPos((HWND)lParam, SB_CTL) - 5), TRUE);
		//if(scrollBarId == 13) thickness -=5;
		//if(thickness <0) thickness = 0;
        break;
    case SB_LINERIGHT:
        SetScrollPos((HWND)lParam, SB_CTL, min(255, GetScrollPos((HWND)lParam, SB_CTL) + 1), TRUE);
		//if(scrollBarId == 13) thickness++;
        break;
    case SB_PAGERIGHT:
        SetScrollPos((HWND)lParam, SB_CTL, min(255, GetScrollPos((HWND)lParam, SB_CTL) + 5), TRUE);
		//if(scrollBarId == 13) thickness+=5;
        break;
    case SB_THUMBTRACK:
        SetScrollPos((HWND)lParam, SB_CTL, HIWORD(wParam), TRUE);
		//if(scrollBarId == 13) thickness = HIWORD(wParam);
        break;
    }
}

void SetColor(HWND hWnd,HDC hdc)
{
    int status = 0;
    if (hdc == NULL) {
        hdc = GetDC(hWnd);
        status = 1;
    }


    //��ũ�� Bar�� �� �޾ƿ���
    int R = GetScrollPos(FindWindowExW(hWnd, NULL, L"scrollbar", L"R"), SB_CTL);
    int G = GetScrollPos(FindWindowExW(hWnd, NULL, L"scrollbar", L"G"), SB_CTL);
    int B = GetScrollPos(FindWindowExW(hWnd, NULL, L"scrollbar", L"B"), SB_CTL);

    HBRUSH newBrush = CreateSolidBrush(RGB(R, G, B));
    HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, newBrush);
    //���õ� ���� �����ֱ�
	
    Rectangle(hdc, 280, 40, 320, 100);

    //��ũ�� Bar�� �� ǥ�����ֱ�
    WCHAR text[10];
    wsprintf(text, L"R : %d", R );
	TextOutW(hdc, 210, 40, L"R :         ", 12);
    TextOutW(hdc, 210, 40, text, lstrlenW(text));
    wsprintf(text, L"G : %d", G );
	TextOutW(hdc, 210, 65, L"R :         ", 12);
    TextOutW(hdc, 210, 65, text, lstrlenW(text));
    wsprintf(text, L"B : %d", B );
	TextOutW(hdc, 210, 90, L"R :         ", 12);
    TextOutW(hdc, 210, 90, text, lstrlenW(text));
    SelectObject(hdc, OldBrush);
    DeleteObject(newBrush);
    
    if (status == 1)
    {
        ReleaseDC(hWnd, hdc);
    }
}

void SetThickness(HWND hWnd,HDC hdc)
{
    int status = 0;
    if (hdc == NULL) {
        hdc = GetDC(hWnd);
        status = 1;
    }

	HPEN newPen = CreatePen(PS_SOLID, 100, RGB(255, 255, 255));
    HPEN OldPen = (HPEN)SelectObject(hdc, newPen);
    
	

    MoveToEx(hdc, 430, 70, NULL);
    LineTo(hdc, 430, 70);
    
	SelectObject(hdc, OldPen);
    DeleteObject(newPen);

    newPen = CreatePen(PS_SOLID, thickness, RGB(0, 0, 0));
    OldPen = (HPEN)SelectObject(hdc, newPen);
    

    MoveToEx(hdc, 430, 70, NULL);
    LineTo(hdc, 430, 70);
    
	
    

    //��ũ�� Bar�� �� ǥ�����ֱ�
    WCHAR text[10];
    wsprintf(text, L"%d", thickness );
	TextOutW(hdc, 640, 75, L"               ", 12);
    TextOutW(hdc, 640, 75, text, lstrlenW(text));
    
    SelectObject(hdc, OldPen);
    DeleteObject(newPen);
    
    if (status == 1)
    {
        ReleaseDC(hWnd, hdc);
    }
}

void CreateBackPage(HWND hWnd, HINSTANCE hInst, HDC* memDC , HBITMAP* memBitmap)
{
    hdc = GetDC(hWnd);
    *memDC = CreateCompatibleDC(hdc);
    RECT rect;
    GetClientRect(hWnd, &rect);
    *memBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
    (HBITMAP)SelectObject(*memDC, *memBitmap);
    FillRect(*memDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
    ReleaseDC(hWnd, hdc);

}

POINT Draw(HWND hWnd, HDC hdc, WPARAM wParam, LPARAM lParam,POINT stPos) 
{

    //��ũ�� Bar�� �� �޾ƿ���
    int R = GetScrollPos(FindWindowExW(hWnd, NULL, L"scrollbar", L"R"), SB_CTL);
    int G = GetScrollPos(FindWindowExW(hWnd, NULL, L"scrollbar", L"G"), SB_CTL);
    int B = GetScrollPos(FindWindowExW(hWnd, NULL, L"scrollbar", L"B"), SB_CTL);

    HPEN newPen = CreatePen(PS_SOLID, thickness, RGB(R, G, B));
    HPEN oldPen = (HPEN)SelectObject(hdc, newPen);

    POINT pos;
    pos.x = GET_X_LPARAM(lParam);
    pos.y = GET_Y_LPARAM(lParam);

    MoveToEx(hdc, stPos.x, stPos.y, NULL);
    LineTo(hdc, pos.x, pos.y);

    SelectObject(hdc, oldPen);
    DeleteObject(newPen);

    stPos.x = pos.x;
    stPos.y = pos.y;
	bImageChanged = TRUE;

	


    return stPos;
}





POINT Eraser(HWND hWnd, HDC hdc, WPARAM wParam, LPARAM lParam, POINT stPos)
{
    HPEN newPen = CreatePen(PS_SOLID, thickness, RGB(255, 255, 255));
    HPEN oldPen = (HPEN)SelectObject(hdc, newPen);

    POINT pos;
    pos.x = GET_X_LPARAM(lParam);
    pos.y = GET_Y_LPARAM(lParam);

    MoveToEx(hdc, stPos.x, stPos.y, NULL);
    LineTo(hdc, pos.x, pos.y);

    SelectObject(hdc, oldPen);
    DeleteObject(newPen);

    stPos.x = pos.x;
    stPos.y = pos.y;
	bImageChanged = TRUE;

	


    return stPos;
}

POINT DrawCircle(HWND hWnd, HDC hdc, WPARAM wParam, LPARAM lParam) {
		
        int R = GetScrollPos(FindWindowExW(hWnd, NULL, L"scrollbar", L"R"), SB_CTL);
        int G = GetScrollPos(FindWindowExW(hWnd, NULL, L"scrollbar", L"G"), SB_CTL);
        int B = GetScrollPos(FindWindowExW(hWnd, NULL, L"scrollbar", L"B"), SB_CTL);

        HPEN newPen = CreatePen(PS_SOLID, thickness, RGB(R, G, B));
		HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

        HPEN oldPen = (HPEN)SelectObject(hdc, newPen);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        POINT pos;
        pos.x = GET_X_LPARAM(lParam);
        pos.y = GET_Y_LPARAM(lParam);

       
		
		//TextOut(hdc,100,100,L"test1",5);
		
		
        Ellipse(hdc, stPos.x, stPos.y, pos.x, pos.y);

        SelectObject(hdc, oldPen);
		SelectObject(hdc, hOldBrush);
        DeleteObject(newPen);
		
		
		
		
		
		bImageChanged = TRUE;
		
		return stPos;
}

// ���簢���� �׸��� �Լ�
POINT  DrawRectangle(HWND hWnd, HDC hdc, WPARAM wParam, LPARAM lParam) {
    
        int R = GetScrollPos(FindWindowExW(hWnd, NULL, L"scrollbar", L"R"), SB_CTL);
        int G = GetScrollPos(FindWindowExW(hWnd, NULL, L"scrollbar", L"G"), SB_CTL);
        int B = GetScrollPos(FindWindowExW(hWnd, NULL, L"scrollbar", L"B"), SB_CTL);

        HPEN newPen = CreatePen(PS_SOLID, thickness, RGB(R, G, B));
		HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

        HPEN oldPen = (HPEN)SelectObject(hdc, newPen);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        POINT pos;
        pos.x = GET_X_LPARAM(lParam);
        pos.y = GET_Y_LPARAM(lParam);

       
		
		//TextOut(hdc,100,100,L"test1",5);
		
		
        Rectangle(hdc, stPos.x, stPos.y, pos.x, pos.y);

        SelectObject(hdc, oldPen);
		SelectObject(hdc, hOldBrush);
        DeleteObject(newPen);
		
		
		
		
		
		bImageChanged = TRUE;
		
		return stPos;
}