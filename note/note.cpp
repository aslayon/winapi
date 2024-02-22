// note.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "note.h"
#include "tchar.h"
#include "iostream"
#define MAX_LOADSTRING 100000

// 전역 변수:
HINSTANCE hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING] = _T("NotePad");					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING] = _T("NewWin");			// 기본 창 클래스 이름입니다.
int xPointer = 0;
int yPointer = 0;
int CapsLock = 1;


struct TextData {
    wchar_t* text;
    int count;
};
TextData* texts; // 텍스트 데이터 배열

int textCount = 0; // 현재 텍스트의 개수





// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
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

 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_NOTE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NOTE));

	// 기본 메시지 루프입니다.
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
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
//  설명:
//
//    Windows 95에서 추가된 'RegisterClassEx' 함수보다 먼저
//    해당 코드가 Win32 시스템과 호환되도록
//    하려는 경우에만 이 함수를 사용합니다. 이 함수를 호출해야
//    해당 응용 프로그램에 연결된
//    '올바른 형식의' 작은 아이콘을 가져올 수 있습니다.
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NOTE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_NOTE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

  // long WinStyle; // 스타일 저장할 목적

   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   texts = new TextData[MAX_LOADSTRING];

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,CW_USEDEFAULT , NULL, NULL, hInstance, NULL);

  // WinStyle = GetWindowLong(hWnd,GWL_STYLE); // 스타일 받아오기.
   //WinStyle = WinStyle | WS_HSCROLL; // 스크롤 추가
   //SetWindowLong(hWnd, GWL_STYLE,WinStyle); // 스타일 적용

   if (!hWnd)
   {
      return FALSE;
   }



   if (!SetWindowTextA (hWnd, "메모장")) // 타이틀 제목----------------------------------------------------------
	{
		return FALSE;
	}

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
		case WM_KEYDOWN:{
			HDC hdc = GetDC(hWnd); // 윈도우의 클라이언트 영역에 대한 출력 장치 컨텍스트 핸들을 가져옵니다.
		switch (wParam)
    {
		case VK_RETURN:
			yPointer += 17;
			xPointer =0;
			
			texts[textCount].text = new wchar_t[2];
            texts[textCount].text[0] = '\0';
            texts[textCount].text[1] = L'\0';
            texts[textCount].count = textCount;
            textCount++;

			break;
		case VK_BACK:
			if(textCount > 0 && texts[textCount - 1].count == 1 && texts[textCount-1].text[0] != '\0'){ //이전입력이 글자일때
			 free(texts[textCount -1].text);
			 textCount--;
			 xPointer -=10;
			 TextOut(hdc, xPointer, yPointer, L"   ",3);
			}
			if(textCount > 0 && texts[textCount - 1].count >= 1 && texts[textCount-1].text[0] == '\0'){ //이전 입력이 엔터일때
			 yPointer -= 17;
			 xPointer = texts[textCount - 1].count;
			 free(texts[textCount -1].text);
			 TextOut(hdc, xPointer, yPointer, L"   ",3);
			}
			break;
		case VK_CAPITAL:
			CapsLock = CapsLock * (-1);
			break;
		default:
			
			wchar_t ch = (wchar_t)wParam;          //눌린 키 정보
			
			wchar_t str[2] = {ch,L'\0'};			//눌린키 + \0 (문자열의 끝을 표시) 하는 정보로 바꿈

			if(CapsLock<0){
			 CharUpperW(str);
			 TextOut(hdc, xPointer, yPointer, str,1);
			 xPointer += 10;
			 
			 texts[textCount].text = new wchar_t[2];
             wcscpy(texts[textCount].text, str);
             texts[textCount].count = 1;
             textCount++;
			 
			 ReleaseDC(hWnd,hdc);

			break;
			}
			else{
			 CharLowerW(str);
			 TextOut(hdc, xPointer, yPointer, str,1);
			 xPointer += 10;
			 
			 texts[textCount].text = new wchar_t[2];
             wcscpy(texts[textCount].text, str);
             texts[textCount].count = 1;
             textCount++;

			 ReleaseDC(hWnd,hdc);

			break;
			}

			
		}
	}
	break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_SIZE:{
		int x= 0;
		int y=0;
            HDC hdc = GetDC(hWnd);
        for (int i = 0; i < textCount; i++) {
			if(textCount > 0 && texts[i].count >= 1 && texts[i].text[0] == '\0'){
			 x= 0;
			 y += 17;
			 continue;
			}
            TextOut(hdc, x, y, texts[i].text, texts[i].count);
			x += 10;
        }
        ReleaseDC(hWnd, hdc);
				 }
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 그리기 코드를 추가합니다.
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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
