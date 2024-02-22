// note.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "note.h"
#include "tchar.h"
#include "iostream"
#define MAX_LOADSTRING 100000

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING] = _T("NotePad");					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING] = _T("NewWin");			// �⺻ â Ŭ���� �̸��Դϴ�.
int xPointer = 0;
int yPointer = 0;
int CapsLock = 1;


struct TextData {
    wchar_t* text;
    int count;
};
TextData* texts; // �ؽ�Ʈ ������ �迭

int textCount = 0; // ���� �ؽ�Ʈ�� ����





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
	LoadString(hInstance, IDC_NOTE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NOTE));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NOTE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_NOTE);
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

  // long WinStyle; // ��Ÿ�� ������ ����

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   texts = new TextData[MAX_LOADSTRING];

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,CW_USEDEFAULT , NULL, NULL, hInstance, NULL);

  // WinStyle = GetWindowLong(hWnd,GWL_STYLE); // ��Ÿ�� �޾ƿ���.
   //WinStyle = WinStyle | WS_HSCROLL; // ��ũ�� �߰�
   //SetWindowLong(hWnd, GWL_STYLE,WinStyle); // ��Ÿ�� ����

   if (!hWnd)
   {
      return FALSE;
   }



   if (!SetWindowTextA (hWnd, "�޸���")) // Ÿ��Ʋ ����----------------------------------------------------------
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
	HDC hdc;

	switch (message)
	{
		case WM_KEYDOWN:{
			HDC hdc = GetDC(hWnd); // �������� Ŭ���̾�Ʈ ������ ���� ��� ��ġ ���ؽ�Ʈ �ڵ��� �����ɴϴ�.
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
			if(textCount > 0 && texts[textCount - 1].count == 1 && texts[textCount-1].text[0] != '\0'){ //�����Է��� �����϶�
			 free(texts[textCount -1].text);
			 textCount--;
			 xPointer -=10;
			 TextOut(hdc, xPointer, yPointer, L"   ",3);
			}
			if(textCount > 0 && texts[textCount - 1].count >= 1 && texts[textCount-1].text[0] == '\0'){ //���� �Է��� �����϶�
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
			
			wchar_t ch = (wchar_t)wParam;          //���� Ű ����
			
			wchar_t str[2] = {ch,L'\0'};			//����Ű + \0 (���ڿ��� ���� ǥ��) �ϴ� ������ �ٲ�

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
		// �޴� ������ ���� �м��մϴ�.
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
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
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
