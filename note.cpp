// note.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "note.h"
#include "tchar.h"
#include <iostream>
#define MAX_LOADSTRING 100000
#include <fstream>
#include <tchar.h>
#include <commdlg.h>
#define MAX_TEXT_LENGTH 10000
#include<string>
#include<string.h>
#include <locale>
// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING] = _T("NotePad");					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING] = _T("NewWin");			// �⺻ â Ŭ���� �̸��Դϴ�.
int xPointer = 0;
int yPointer = 0;
int CapsLock = 1;
int langu = 1;
int ime =1;
HWND hWnd;
wchar_t Cstr[10];
int ime_Counter = 0;
int scrollH = 0;
int scrollV = 0;
RECT RectErase;

struct TextData {
    wchar_t* text;
    int count;
};
TextData* texts; // �ؽ�Ʈ ������ �迭

int textCount = 0; // ���� �ؽ�Ʈ�� ����
int pointerCount =0;
int focusingFlag =1;
int InsertFlag = 1;
int MAX_yPointer =0;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void SaveDataToFile(HWND hWnd, HDC hdc);
void LoadDataFromFile(HWND hWnd,HDC hdc);
void ResetNote(HDC hdc);
void IME_RESULT(HWND hWnd, HDC hdc);
void IME_COMP(HWND hWnd,HDC hdc);
void LeftClick(HDC hdc);
void Return(HDC hdc);
void Back(HDC hdc);
void BKTAB(HDC hdc);
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

	RECT tempRect;
	GetWindowRect(hWnd, &tempRect); // hWnd�� �������� �ڵ�

				// �������� ���� ���̸� ���
	int Widthh = tempRect.right - tempRect.left -50;
	int Heightt = tempRect.bottom - tempRect.top-50;

	
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	int len;
	hdc = GetDC(hWnd);
	if(scrollH<0){
	scrollH =0;
	}
	{
	
	
	}
	if(textCount == 0){
	xPointer = 0;}


	
	if(pointerCount<0) {
		pointerCount =0;
	}
	
	switch (message)
	{
		case WM_CREATE:{
			CreateCaret(hWnd,NULL,1,17);
			ShowCaret(hWnd);
			break;
				   }
		case WM_IME_COMPOSITION:{
			HideCaret(hWnd);
			HIMC Himc = ImmGetContext(hWnd);
			hdc = GetDC(hWnd);
			
			if(lParam & GCS_RESULTSTR){
				IME_RESULT(hWnd, hdc);
			}
			else if(lParam & GCS_COMPSTR){
				IME_COMP(hWnd, hdc);
			}
			ShowCaret(hWnd);
			break;
		}
		
		case WM_LBUTTONDOWN:{
				LeftClick(hdc);
					break;		
							}
		case WM_CHAR:
		{
		SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);	
		HDC hdc = GetDC(hWnd); // �������� Ŭ���̾�Ʈ ������ ���� ��� ��ġ ���ؽ�Ʈ �ڵ��� �����ɴϴ�.
		 HideCaret(hWnd);

	
		if (message != WM_IME_COMPOSITION){
			switch (wParam)
			{

			case VK_RETURN:{
				Return(hdc);
						   }
					SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
			ShowCaret(hWnd);
            break;

			


			case VK_BACK:{
			
            Back(hdc);
							 }





        
			case VK_HANGUL:
			langu = langu *(-1);
			break;


			case VK_TAB:{
					BKTAB(hdc);


					break;
							}
			 default:{
			
            wchar_t ch = (wchar_t)wParam;          //���� Ű ����
            wchar_t str[2] = { ch, L'\0' };        //����Ű + \0 (���ڿ��� ���� ǥ��) �ϴ� ������ �ٲ�

            if (CapsLock < 0) {
                CharUpperW(str);
            }
            else {
                CharLowerW(str);
            }

            TextOut(hdc,xPointer- (scrollV) * (-15), yPointer - (scrollH)*15, str, 1);
            xPointer += 10;


			


			if(InsertFlag ==1 && pointerCount < textCount){
				for(int i=textCount; i>pointerCount; i--){
					if(i ==0)break;
					texts[i].text = texts[i-1].text;
					texts[i].count = texts[i-1].count;
				}
			}
			if(InsertFlag == -1){
				if(pointerCount != textCount){}
				else{texts[pointerCount].text = new wchar_t[2];}
				
			}
			else{
				texts[pointerCount].text = new wchar_t[2];}
            wcscpy(texts[pointerCount].text, str);
            texts[pointerCount].count = 10;
			if(InsertFlag == -1 && textCount != pointerCount){}
			else{
				textCount++;}
			pointerCount++;
			
			

			//Ŀ�� ���� ��ũ�� �̵�
	if(xPointer- (scrollV) * (-15)>Widthh-50)
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos += 10;
	scrollV -=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
	if(xPointer- (scrollV) * (-15)<0 )
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos -= 10;
	scrollV +=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}


			if(pointerCount == textCount){
				SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
				ShowCaret(hWnd);
				break;
			}
			{


				RECT clientRect;
				GetWindowRect(hWnd, &clientRect); // hWnd�� �������� �ڵ�

				// �������� ���� ���̸� ���
				int windowWidth = clientRect.right - clientRect.left;
				int windowHeight = clientRect.bottom - clientRect.top;

				// RECT ����ü �����
				RectErase.left = 0; // ���� x ��ǥ
				RectErase.top = yPointer; // ���� y ��ǥ
				RectErase.right = RectErase.left + windowWidth; // ������ x ��ǥ (�������� ���� ���)
				RectErase.bottom = RectErase.top + 17; // �Ʒ��� y ��ǥ (������ ���̸� ���)


				InvalidateRect(hWnd,NULL,FALSE);
				UpdateWindow(hWnd);

			
				


			}
					 }  
		}
		SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
		ShowCaret(hWnd);

	
		}
		break; 
}


		case WM_KEYDOWN:{
			
			switch(wParam){

			case VK_DELETE:{
						   
				if(textCount == pointerCount) break;
				
				HideCaret(hWnd);
				if (textCount > 0 && texts[pointerCount].count >= 15) { //�����Է��� �����϶�
			
				
				
				
				delete[] (texts[pointerCount ].text);
				
				texts[pointerCount ].count =0;
                textCount--;
				
                
                TextOut(hdc,xPointer- (scrollV) * (-15), yPointer, L"    ", 4);

				if(pointerCount < textCount){
				for(int i=pointerCount; i<textCount;i++){
					if(!texts[i+1].text)break;
					texts[i].text = texts[i+1].text;
					texts[i].count = texts[i+1].count;
				}
				texts[textCount+1].text = NULL;
			}



				//Ŀ�� ���� ��ũ�� �̵�
	if(xPointer- (scrollV) * (-15)>Widthh-50)
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos += 10;
	scrollV -=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
	if(xPointer- (scrollV) * (-15)<0 )
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos -= 10;
	scrollV +=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}





				{
					RECT clientRect;
					GetWindowRect(hWnd, &clientRect); // hWnd�� �������� �ڵ�

				// �������� ���� ���̸� ���
					int windowWidth = clientRect.right - clientRect.left;
					int windowHeight = clientRect.bottom - clientRect.top;

				// RECT ����ü �����
					RectErase.left = 0; // ���� x ��ǥ
					RectErase.top = yPointer; // ���� y ��ǥ
					RectErase.right = RectErase.left + windowWidth; // ������ x ��ǥ (�������� ���� ���)
					RectErase.bottom = RectErase.top + 17; // �Ʒ��� y ��ǥ (������ ���̸� ���)


					
					

					InvalidateRect(hWnd,NULL,FALSE);
					UpdateWindow(hWnd);

					int x= 0;
					int y=0;
					HDC hdc = GetDC(hWnd);
					for(int i =0; i< textCount; i++) {
					
					if(textCount > 0 &&texts[i].count == 0 && texts[i].text[0] == '\0'){
					x= 0;
					y += 17;
					continue;
					}
					if(textCount >0 && texts[i].count >= 15){
				
						
						TextOut(hdc,x- (scrollV) * (-15), y - (scrollH)*15, texts[i].text, 1);
						
					
					
					x += texts[i].count;
					
					continue;
					}
					
						
						TextOut(hdc,x- (scrollV) * (-15), y - (scrollH)*15, texts[i].text, 1);
						
					
					
					
					x += 10;
					
					continue;
					}
				

				
				}
				SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
			ShowCaret(hWnd);
				break;
            }
				else if (textCount > 0 && texts[pointerCount].count == 0) { //���� �Է��� �����϶�
                
                delete[] (texts[pointerCount].text);
                textCount--;
				
				
				
				if(pointerCount < textCount){
				for(int i=pointerCount; i<textCount;i++){
					if(!texts[i+1].text)break;
					texts[i].text = texts[i+1].text;
					texts[i].count = texts[i+1].count;
				}
				texts[textCount+1].text = NULL;
			}
				
				{
				
					RECT clientRect;
					GetWindowRect(hWnd, &clientRect); // hWnd�� �������� �ڵ�

				// �������� ���� ���̸� ���
					int windowWidth = clientRect.right - clientRect.left;
					int windowHeight = clientRect.bottom - clientRect.top;

				// RECT ����ü �����
					RectErase.left = 0; // ���� x ��ǥ
					RectErase.top = yPointer; // ���� y ��ǥ
					RectErase.right = RectErase.left + windowWidth; // ������ x ��ǥ (�������� ���� ���)
					RectErase.bottom = RectErase.top + windowHeight; // �Ʒ��� y ��ǥ (������ ���̸� ���)


					
					//Ŀ�� ���� ��ũ�� �̵�
	if(xPointer- (scrollV) * (-15)>Widthh-50)
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos += 10;
	scrollV -=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
	if(xPointer- (scrollV) * (-15)<0 )
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos -= 10;
	scrollV +=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}

					InvalidateRect(hWnd,NULL,FALSE);
					UpdateWindow(hWnd);

					int x= 0;
					int y=0;
					HDC hdc = GetDC(hWnd);
					for(int i =0; i< textCount; i++) {
					
					if(textCount > 0 &&texts[i].count == 0 && texts[i].text[0] == '\0'){
					x= 0;
					y += 17;
					continue;
					}
					if(textCount >0 && texts[i].count >= 15){
				
						
						TextOut(hdc,x- (scrollV) * (-15), y - (scrollH)*15, texts[i].text, 1);
						
					
					
					x += texts[i].count;
					
					continue;
					}
					
						
						TextOut(hdc,x- (scrollV) * (-15), y - (scrollH)*15, texts[i].text, 1);
						
					
					
					
					x += 10;
					
					continue;
					}
				


				}

				SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
			ShowCaret(hWnd);

				break;
            }
				else if(textCount >0 && texts[pointerCount].count == 10){
				delete[] (texts[pointerCount].text);
                textCount--;
				
                
				TextOut(hdc,xPointer- (scrollV) * (-15), yPointer, L"   ", 3);
				
				
				if(pointerCount < textCount){
				for(int i=pointerCount; i<textCount;i++){
					if(!texts[i+1].text)break;
					texts[i].text = texts[i+1].text;
					texts[i].count = texts[i+1].count;
				}
				texts[textCount+1].text = NULL;
			}
				
				
				{
				
					RECT clientRect;
					GetWindowRect(hWnd, &clientRect); // hWnd�� �������� �ڵ�

				// �������� ���� ���̸� ���
					int windowWidth = clientRect.right - clientRect.left;
					int windowHeight = clientRect.bottom - clientRect.top;

				// RECT ����ü �����
					RectErase.left = 0; // ���� x ��ǥ
					RectErase.top = yPointer; // ���� y ��ǥ
					RectErase.right = RectErase.left + windowWidth; // ������ x ��ǥ (�������� ���� ���)
					RectErase.bottom = RectErase.top + windowHeight; // �Ʒ��� y ��ǥ (������ ���̸� ���)


					//Ŀ�� ���� ��ũ�� �̵�
	if(xPointer- (scrollV) * (-15)>Widthh-50)
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos += 10;
	scrollV -=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
	if(xPointer- (scrollV) * (-15)<0 )
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos -= 10;
	scrollV +=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
					

					InvalidateRect(hWnd,NULL,FALSE);
					UpdateWindow(hWnd);

					int x= 0;
					int y=0;
					HDC hdc = GetDC(hWnd);
					for(int i =0; i< textCount; i++) {
					
					if(textCount > 0 &&texts[i].count == 0 && texts[i].text[0] == '\0'){
					x= 0;
					y += 17;
					continue;
					}
					if(textCount >0 && texts[i].count >= 15){
				
						
						TextOut(hdc,x- (scrollV) * (-15), y - (scrollH)*15, texts[i].text, 1);
						
					
					
					x += texts[i].count;
					
					continue;
					}
					
						
						TextOut(hdc,x- (scrollV) * (-15), y - (scrollH)*15, texts[i].text, 1);
						
					
					
					
					x += 10;
					
					continue;
					}
				

				}
				SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
			ShowCaret(hWnd);
				break;
			}
               
						   
						   }

			case VK_CAPITAL:
					CapsLock = CapsLock * (-1);
					break;
			case VK_LEFT:{
					if(pointerCount ==0){break;}
					if(texts[pointerCount-1].count == 0 && texts[pointerCount-1].text[0] == '\0'){break;}
					HideCaret(hWnd);
					pointerCount--; 
					if(textCount > 0 &&texts[pointerCount].count == 0 && texts[pointerCount].text[0] == '\0'){
				
					}
					else{
						xPointer -= texts[pointerCount].count;
						
					}
					SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
					ShowCaret(hWnd);
					break;

					//Ŀ�� ���� ��ũ�� �̵�
	if(xPointer- (scrollV) * (-15)>Widthh-50)
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos += 10;
	scrollV -=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
	if(xPointer- (scrollV) * (-15)<0 )
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos -= 10;
	scrollV +=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
					



				}
			

				 break;


				


			case VK_RIGHT:{
					if(pointerCount ==textCount){break;}
					if(texts[pointerCount].count == 0 && texts[pointerCount].text[0] == '\0'){break;}
					HideCaret(hWnd);
					
					if(textCount > 0 &&texts[pointerCount+1].count == 0 && texts[pointerCount+1].text[0] == '\0'){
						pointerCount++;
						xPointer += texts[pointerCount-1].count;
					}
					else{
						pointerCount++;
						xPointer += texts[pointerCount-1].count;
						
					}
					
					//Ŀ�� ���� ��ũ�� �̵�
	if(xPointer- (scrollV) * (-15)>Widthh-50)
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos += 10;
	scrollV -=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
	if(xPointer- (scrollV) * (-15)<0 )
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos -= 10;
	scrollV +=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
					SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
					ShowCaret(hWnd);
					break;
				}
			case VK_UP:{
				if(textCount == 0)break;
				if(yPointer == 0){break;}
				
				HideCaret(hWnd);
				yPointer -= 17;
				int x=0;
				int y=0;

				for(int i=0;i<textCount;i++){
					if(textCount > 0 &&texts[i].count == 0 && texts[i].text[0] == '\0'){
					x =0;
					y += 17;
					}
					else{
					x += texts[i].count;
					}
					if(xPointer - x<6 && yPointer == y){
						xPointer = x;
						pointerCount = i+1;
						SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
						ShowCaret(hWnd);
						break;
					}
				}
				if(xPointer == x && yPointer == y){
				SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
				ShowCaret(hWnd);
				break;
				}
				else{
					x=0;
					y=0;
					for(int i=0;i<textCount;i++){
					if(textCount > 0 &&texts[i].count == 0 && texts[i].text[0] == '\0'){
					x =0;
					y += 17;
					}
					else{
					x += texts[i].count;
					}
					if(texts[i+1].count == 0 && texts[i+1].text[0] == '\0' && yPointer == y){
						pointerCount = i+1;
						xPointer += texts[pointerCount].count;
						xPointer = x;

						SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
						ShowCaret(hWnd);

						break;
					}
					}
				}
				

				//Ŀ�� ���� ��ũ�� �̵�
	if(xPointer- (scrollV) * (-15)>Widthh-50)
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos += 10;
	scrollV -=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
	if(xPointer- (scrollV) * (-15)<0 )
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos -= 10;
	scrollV +=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
				


				
				}
				break;

				case VK_DOWN:{
					if(textCount == 0)break;
					if(yPointer == MAX_yPointer){break;}
					int x=0;
					int y=0;
					int FFF = 0;
					yPointer +=17;
					for(int i=0;i<textCount;i++){
					if(textCount > 0 &&texts[i].count == 0 && texts[i].text[0] == '\0'){
						if(yPointer == y){//�Ʒ��� �� ����.
						FFF = 1;
						 xPointer = x;
						 pointerCount = i;
						 break;
						}
					x =0;
					y += 17;
					}
					else{
					x += texts[i].count;
					if(xPointer - x <6&& yPointer == y){//�Ʒ��� ũ�ų� ��
						xPointer = x;
						FFF = -1;
						pointerCount = i+1;
						break;
					
					}
					}
					
					}
					if(FFF ==1){//�Ʒ��� �� ��
					
					}
					if(FFF == -1){//�Ʒ� �� ũ�ų� ��

					}
					if(FFF == 0){
						pointerCount = textCount;
						xPointer = x;}
					
				


				SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
				ShowCaret(hWnd);



				//Ŀ�� ���� ��ũ�� �̵�
	if(xPointer- (scrollV) * (-15)>Widthh-50)
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos += 10;
	scrollV -=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
	if(xPointer- (scrollV) * (-15)<0 )
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos -= 10;
	scrollV +=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}



				break;
				}
							 
				case VK_HOME:{
					if(textCount == 0)break;
					int x=0;
					int y=0;
					for(int i=0;i<textCount;i++){
						if(yPointer == 0){
							pointerCount = 0;
							xPointer = 0;
							break;}
					if(textCount > 0 &&texts[i].count == 0 && texts[i].text[0] == '\0'){
					
					x =0;
					y += 17;
					if(y == yPointer){
						pointerCount = i+1;
						xPointer = 0;
					}
					}
					else{
					x += texts[i].count;										
					}

					}
					//Ŀ�� ���� ��ũ�� �̵�
	if(xPointer- (scrollV) * (-15)>Widthh-50)
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos += 10;
	scrollV -=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
	if(xPointer- (scrollV) * (-15)<0 )
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos -= 10;
	scrollV +=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
					SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
				ShowCaret(hWnd);
					
							 }
							 break;
				case VK_END:{
						if(textCount == 0)break;
						int x = 0;
						int y = 0;
						int fff = 0;
						for(int i=0;i<textCount;i++){
						
					if(textCount > 0 &&texts[i].count == 0 && texts[i].text[0] == '\0'){
					if(y == yPointer){
						fff = 1;
						pointerCount = i;
						xPointer = x;
						break;
					}
					x =0;
					y += 17;
					
					}
					else{
					x += texts[i].count;
					
					}

					}
				
						if(fff ==0){
						pointerCount = textCount;
						xPointer = x;
						}
						//Ŀ�� ���� ��ũ�� �̵�
	if(xPointer- (scrollV) * (-15)>Widthh-50)
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos += 10;
	scrollV -=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
	if(xPointer- (scrollV) * (-15)<0 )
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos -= 10;
	scrollV +=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}

				SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
				ShowCaret(hWnd);
				break;
							}
				
				case VK_INSERT:{
							InsertFlag *= -1;
							break;
							   }
				default:{}
						}
						
						}
case WM_VSCROLL:
    // ���� ��ũ�ѹ� �޽��� ó��
{
    SCROLLINFO si;
    int yPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ��ũ�� ������ �����ɴϴ�.
    ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_VERT, &si);

    // ��ũ�� �̺�Ʈ�� ó���մϴ�.
    nScrollCode = LOWORD(wParam);
    switch (LOWORD(wParam))
    {
        case SB_LINEUP:
			if(scrollH ==0) break;
            si.nPos -= 1;
			scrollH--;
			
            break;

        case SB_LINEDOWN:
            si.nPos += 1;
			scrollH++;
			
            break;
		case SB_PAGEUP:{
			if(scrollH ==0) break;

			if(scrollH <5){
			scrollH = 0;
			si.nPos = 0;
			}
			else{
				si.nPos -= 5;
				scrollH -= 5;}
            
			break;
					   }
		case SB_PAGEDOWN:{
			
            si.nPos += 5;
			
			scrollH += 5;
			break;
					   }
		
		
        

        default:
            break;
    }

    // ��ũ�� ��ġ�� �ּҰ����� �۰ų� �ִ밪���� ũ�� �����մϴ�.
    si.nPos = max(0, min(si.nPos, si.nMax - si.nPage));

    // ���ο� ��ũ�� ��ġ�� �����մϴ�.
    SetScrollPos(hWnd, SB_VERT, si.nPos, TRUE);

    // ȭ���� �ٽ� �׸��ϴ�.
    
	{
				RECT clientRect;
				GetWindowRect(hWnd, &clientRect); // hWnd�� �������� �ڵ�

				// �������� ���� ���̸� ���
				int windowWidth = clientRect.right - clientRect.left;
				int windowHeight = clientRect.bottom - clientRect.top;

				// RECT ����ü �����
				RectErase.left = 0; // ���� x ��ǥ
				RectErase.top = 0; // ���� y ��ǥ
				RectErase.right = RectErase.left + windowWidth; // ������ x ��ǥ (�������� ���� ���)
				RectErase.bottom = RectErase.top + windowHeight; // �Ʒ��� y ��ǥ (������ ���̸� ���)	
				InvalidateRect(hWnd,NULL,FALSE);
				UpdateWindow(hWnd);

				
				


			}
	SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
						ShowCaret(hWnd);

    break;
}


case WM_HSCROLL:
{
    SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
    ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

    // ��ũ�� �̺�Ʈ�� ó���մϴ�.
    nScrollCode = LOWORD(wParam);
    switch (LOWORD(wParam))
    {
        case SB_LINELEFT:
			if(scrollV ==0) break;
            si.nPos -= 1;
			scrollV++;
			
            break;

        case SB_LINERIGHT:
            si.nPos += 1;
			scrollV--;
			
            break;

       case SB_PAGELEFT:{
			if(scrollV ==0) break;
			if(scrollV >-5){
			scrollV = 0;
			si.nPos = 0;
			}
			else{
				scrollV += 5;
			si.nPos -= 5;
			}
			
			break;
					   }
            
			
		case SB_PAGERIGHT:{
			
            si.nPos += 5;
			
			scrollV -= 5;
			break;
					   }

        default:
            break;
    }

    // ��ũ�� ��ġ�� �ּҰ����� �۰ų� �ִ밪���� ũ�� �����մϴ�.
    si.nPos = max(0, min(si.nPos, si.nMax - si.nPage));

    // ���ο� ��ũ�� ��ġ�� �����մϴ�.
    SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);

    // ȭ���� �ٽ� �׸��ϴ�.
    

	{
				RECT clientRect;
				GetWindowRect(hWnd, &clientRect); // hWnd�� �������� �ڵ�

				// �������� ���� ���̸� ���
				int windowWidth = clientRect.right - clientRect.left;
				int windowHeight = clientRect.bottom - clientRect.top;

				// RECT ����ü �����
				RectErase.left = 0; // ���� x ��ǥ
				RectErase.top = 0; // ���� y ��ǥ
				RectErase.right = RectErase.left + windowWidth; // ������ x ��ǥ (�������� ���� ���)
				RectErase.bottom = RectErase.top + windowHeight; // �Ʒ��� y ��ǥ (������ ���̸� ���)	
				InvalidateRect(hWnd,NULL,FALSE);
				UpdateWindow(hWnd);

				
				


			}
	SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
						ShowCaret(hWnd);

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
		case ID_32771:{
					  SaveDataToFile(hWnd,hdc);
					  break;
					  }
					  break;
		case ID_32772:{
					  ResetNote(hdc);
					  LoadDataFromFile(hWnd,hdc);
					  break;
					  }
					  break;

		case ID_32773:{
					  ResetNote(hdc);
					  
					  break;
					  }
					  break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_SIZE:{
		{
				RECT clientRect;
				GetWindowRect(hWnd, &clientRect); // hWnd�� �������� �ڵ�

				// �������� ���� ���̸� ���
				int windowWidth = clientRect.right - clientRect.left;
				int windowHeight = clientRect.bottom - clientRect.top;

				// RECT ����ü �����
				RectErase.left = 0; // ���� x ��ǥ
				RectErase.top = 0; // ���� y ��ǥ
				RectErase.right = RectErase.left + windowWidth; // ������ x ��ǥ (�������� ���� ���)
				RectErase.bottom = RectErase.top + windowHeight; // �Ʒ��� y ��ǥ (������ ���̸� ���)	
				InvalidateRect(hWnd,NULL,FALSE);
				UpdateWindow(hWnd);

	
				

				
				}
        ReleaseDC(hWnd, hdc);
				 }
		break;
	case WM_SETFOCUS:
		{
			hdc = GetDC(hWnd);
			CreateCaret(hWnd,NULL,1, 17);
			ShowCaret(hWnd);
			SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
		}
		break;

	case WM_KILLFOCUS:
		{
			hdc = GetDC(hWnd);
			HideCaret(hWnd);
			DestroyCaret();
		}
		break;
	case WM_PAINT:
	{
			
		/** ������۸� ����ó���Դϴ�. **/
        HDC hdc, MemDC, tmpDC;
        HBITMAP BackBit, oldBackBit;
        RECT bufferRT;
        hdc = BeginPaint(hWnd, &ps);
       
        GetClientRect(hWnd, &bufferRT);
        MemDC = CreateCompatibleDC(hdc);
        BackBit = CreateCompatibleBitmap(hdc, bufferRT.right, bufferRT.bottom);
        oldBackBit = (HBITMAP)SelectObject(MemDC, BackBit);
        PatBlt(MemDC,0,0,bufferRT.right, bufferRT.bottom, WHITENESS);
       

        // TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
		RECT rc;
        GetClientRect(hWnd, &rc);
        FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW+1));
        int x= 0;
		int y=0;
		hdc = GetDC(hWnd);
		for(int i =0; i< textCount; i++) {
					
			if(textCount > 0 &&texts[i].count == 0 && texts[i].text[0] == '\0'){
				x= 0;
				y += 17;
				continue;
			}
			if(textCount >0 && texts[i].count >= 15){
				
						
				TextOut(hdc,x- (scrollV) * (-15), y - (scrollH)*15, texts[i].text, 1);
						
					
					
				x += texts[i].count;
					
			continue;
		}
					
						
				TextOut(hdc,x- (scrollV) * (-15), y - (scrollH)*15, texts[i].text, 1);
						
					
					
					
				x += 10;
					
				continue;
		}

        /** ������۸� ��ó�� �Դϴ�. **/
        tmpDC=hdc;
        hdc=MemDC;
        MemDC=tmpDC;
        GetClientRect(hWnd, &bufferRT);
        BitBlt(hdc, 0, 0, bufferRT.right, bufferRT.bottom, MemDC, 0, 0, SRCCOPY);
        SelectObject(MemDC, oldBackBit);
        DeleteObject(BackBit);
        DeleteDC(MemDC);
        EndPaint(hWnd, &ps);
        break;


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



void SaveDataToFile(HWND hWnd, HDC hdc) {
    OPENFILENAME ofn;
    TCHAR szFile[MAX_PATH] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn) == TRUE) {
        std::wofstream file(ofn.lpstrFile);
		file.imbue(std::locale(std::locale("kor")));
        if (file.is_open()) {
            for (int i = 0; i < textCount; ++i) {
                // ���ڿ��� ������ ���Ͽ� ����
				int length = wcslen(texts[i].text);
				wchar_t* tmp = new wchar_t[length+1];
				wcscpy(tmp, texts[i].text);
				//TextOut(hdc,i*15,17,tmp,1);
                file << tmp << std::endl;
                file << texts[i].count << std::endl;
				delete[] tmp;
            }
            file.close();
            MessageBox(hWnd, _T("����ƽ��ϴ�."), _T("����ƴ�?"), MB_OK | MB_ICONINFORMATION);
        } else {
            MessageBox(hWnd, _T("�������"), _T("����ƴ�?"), MB_OK | MB_ICONERROR);
        }
    }
}




void LoadDataFromFile(HWND hWnd,HDC hdc) {
    OPENFILENAME ofn;
    TCHAR szFile[MAX_PATH] = { 0 };
	
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	
    if (GetOpenFileName(&ofn) == TRUE) {
        std::wifstream file(ofn.lpstrFile);
		file.imbue(std::locale(std::locale("kor")));
        if (file.is_open()) {
            int i = 0;
            while (!file.eof()) {

				texts[i].text = new wchar_t[2];
				wchar_t* tmp = new wchar_t[3];
				file.getline(tmp,sizeof(wchar_t)*2);
				std::wstring line(tmp);
				if(line.empty()){
					file.getline(tmp,sizeof(wchar_t)*2);
					std::wstring line(tmp);
					if(line.empty()){
						delete[] texts[i].text;
						delete[] tmp;
						break;}
					else{
						texts[i].text[0] = L'\0';
						texts[i].text[1] = L'\0';
						texts[i].count = 0;
						delete[] tmp;
						i++;
						continue;
					}
				}
				wcscpy(texts[i].text,tmp);
				TextOut(hdc,i*17,i*17,texts[i].text,1);

				
				file.getline(tmp,sizeof(wchar_t)*2);
				std::wstring wstr(tmp);
				
				int numtmp = std::stoi(wstr);
				texts[i].count = numtmp;
				delete[] tmp;
				
				
				i++;
            }
            textCount = i;
            file.close();


			{
				
				InvalidateRect(hWnd,NULL,FALSE);
				UpdateWindow(hWnd);

				int x= 0;
				int y=0;
					HDC hdc = GetDC(hWnd);
					for(int i =0; i< textCount; i++) {
					
					if(textCount > 0 &&texts[i].count == 0 && texts[i].text[0] == '\0'){
					x= 0;
					y += 17;
					MAX_yPointer = y;
					continue;
					}
					if(textCount >0 && texts[i].count >= 15){
				
						
						TextOut(hdc,x- (scrollV) * (-15), y - (scrollH)*15, texts[i].text, 1);
						
					
					
					x += texts[i].count;
					
					continue;
					}
					
						
						TextOut(hdc,x- (scrollV) * (-15), y - (scrollH)*15, texts[i].text, 1);
						
					
					
					
					x += 10;
					
					continue;
					}
				

				
				}


            MessageBox(hWnd, _T("File loaded successfully."), _T("Success"), MB_OK | MB_ICONINFORMATION);
        } else {
            MessageBox(hWnd, _T("Failed to open file for reading."), _T("Error"), MB_OK | MB_ICONERROR);
        }
    }
}

void ResetNote(HDC hdc){
	pointerCount = textCount;
	for(int i=textCount;i>0;i--){
	{
		delete[] (texts[i-1].text);		
		texts[i-1].count = 0;
	}  
	xPointer = 0;
yPointer = 0;
CapsLock = 1;
langu = 1;
ime =1;


ime_Counter = 0;
scrollH = 0;
scrollV = 0;



textCount = 0; // ���� �ؽ�Ʈ�� ����
pointerCount =0;
focusingFlag =1;
InsertFlag = 1;
MAX_yPointer =0;
	InvalidateRect(hWnd,NULL,FALSE);
            UpdateWindow(hWnd);
							 }
	
	
}

void IME_RESULT(HWND hWnd, HDC hdc){

	RECT tempRect;
	GetWindowRect(hWnd, &tempRect); // hWnd�� �������� �ڵ�

				// �������� ���� ���̸� ���
	int Widthh = tempRect.right - tempRect.left -50;
	int Heightt = tempRect.bottom - tempRect.top-50;

	int len;
	HIMC Himc = ImmGetContext(hWnd);
	hdc = GetDC(hWnd);
	if((len = ImmGetCompositionString(Himc,GCS_RESULTSTR,NULL,0))>0){
					ime_Counter = 0;
					ImmGetCompositionString(Himc,GCS_RESULTSTR,Cstr,len);
					Cstr[len] = L'\0';
					TextOut(hdc,xPointer- (scrollV) * (-15), yPointer - (scrollH)*15, Cstr, 1);
					



					int xPt = xPointer + 15;
					SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
					xPointer += 15;
					ime = 1;

					if(InsertFlag == 1 && pointerCount != textCount){
						for(int i = textCount; i> pointerCount;i--){
							if(i ==0) break;
							texts[i].text = texts[i-1].text;
							texts[i].count = texts[i-1].count;
						}
					}

					if(InsertFlag == -1){
					if(pointerCount != textCount){}
					else{texts[pointerCount].text = new wchar_t[2];}
				
					}
					else{					
						texts[pointerCount].text = new wchar_t[2];}

					wcscpy(texts[pointerCount].text, Cstr);
					texts[pointerCount].count = 15;
					if(InsertFlag == -1 && textCount != pointerCount){}
					else{
						textCount++;}
					pointerCount++;
					if(InsertFlag  ==1&& pointerCount < textCount){

					RECT clientRect;
					GetWindowRect(hWnd, &clientRect); // hWnd�� �������� �ڵ�

				// �������� ���� ���̸� ���
					int windowWidth = clientRect.right - clientRect.left;
					int windowHeight = clientRect.bottom - clientRect.top;

				// RECT ����ü �����
					RectErase.left = 0; // ���� x ��ǥ
					RectErase.top = yPointer; // ���� y ��ǥ
					RectErase.right = RectErase.left + windowWidth; // ������ x ��ǥ (�������� ���� ���)
					RectErase.bottom = RectErase.top + 17; // �Ʒ��� y ��ǥ (������ ���̸� ���)


					//Ŀ�� ���� ��ũ�� �̵�
	if(xPointer- (scrollV) * (-15)>Widthh-50)
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos += 10;
	scrollV -=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
	if(xPointer- (scrollV) * (-15)<0 )
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos -= 10;
	scrollV +=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}


					InvalidateRect(hWnd,NULL,FALSE);
					UpdateWindow(hWnd);

					
					
					}
				}

}

void IME_COMP(HWND hWnd,HDC hdc){
	int len;
	ime_Counter++;
	HIMC Himc = ImmGetContext(hWnd);
	hdc = GetDC(hWnd);
	if(ime == 1) 
			ime =  (-1);
	len =ImmGetCompositionString(Himc,GCS_COMPSTR,NULL,0);
	ImmGetCompositionString(Himc,GCS_COMPSTR,Cstr,len);
	Cstr[len] = L'\0';
	int xPt = xPointer + 15;
	if(Cstr[0] != L'\0'){
			TextOut(hdc,xPointer- (scrollV) * (-15),  yPointer - (scrollH)*15, Cstr, 1);
	}
	else {
			TextOut(hdc,xPointer- (scrollV) * (-15),  yPointer - (scrollH)*15, L"    ", 4);
			xPt -= 15;
	}
				


				
	xPointer +=15;
	ShowCaret(hWnd);
	SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
	xPointer -=15;
	
}

void LeftClick(HDC hdc){
	HideCaret(hWnd);
				POINT cur;
				int xCur=0;
			
				
				int yCur=0;
			

				int x = 0;
				int y = 0;

				int Found =0;
				if(GetCursorPos(&cur)){
					ScreenToClient(hWnd, &cur);
					xCur = cur.x- (scrollV) * (15);
					yCur = cur.y - (scrollH)*(-15);
					
					if(yCur > MAX_yPointer){
						Found = -1;
					}
					for(int i=0;i<textCount;i++){
						if(yCur - y >=0 && yCur - y <= 17){
							if(xCur-x >=0 && xCur - x <= texts[i].count){
								Found = 1;
								xPointer = x;
								yPointer = y;
								pointerCount = i;
								break;
							}
						}
					if(textCount > 0 &&texts[i].count == 0 && texts[i].text[0] == '\0'){
						
					x =0;
					y += 17;
					}
					else{
					x += texts[i].count;
					
						}
					
					}



				}
				else{
				ShowCaret(hWnd);
				return;
				}
				if(Found !=1){
					xPointer = x;
					yPointer = y;
					pointerCount = textCount;
				}
				
				SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
				ShowCaret(hWnd);

}

void Return(HDC hdc){
	HideCaret(hWnd);

	RECT tempRect;
	GetWindowRect(hWnd, &tempRect); // hWnd�� �������� �ڵ�

				// �������� ���� ���̸� ���
	int Widthh = tempRect.right - tempRect.left -50;
	int Heightt = tempRect.bottom - tempRect.top-50;


            yPointer += 17;
            xPointer = 0;
			if(MAX_yPointer <yPointer){
				MAX_yPointer = yPointer;}

			if(InsertFlag == 1 && pointerCount < textCount){
				for(int i=textCount; i>pointerCount;i--){
					
					texts[i].text = texts[i-1].text;
					texts[i].count = texts[i-1].count;
				}
			}
            texts[pointerCount].text = new wchar_t[2];
            texts[pointerCount].text[0] = L'\0';
            texts[pointerCount].text[1] = L'\0';
            texts[pointerCount].count = 0; 
            textCount++;
			pointerCount++;


			RECT clientRect;
			GetWindowRect(hWnd, &clientRect); // hWnd�� �������� �ڵ�

				// �������� ���� ���̸� ���
			int windowWidth = clientRect.right - clientRect.left;
			int windowHeight = clientRect.bottom - clientRect.top;

				// RECT ����ü �����
			RectErase.left = 0; // ���� x ��ǥ
			RectErase.top = yPointer-17; // ���� y ��ǥ
			RectErase.right = RectErase.left + windowWidth; // ������ x ��ǥ (�������� ���� ���)
			RectErase.bottom = RectErase.top + windowHeight; // �Ʒ��� y ��ǥ (������ ���̸� ���)



			//Ŀ�� ���� ��ũ�� �̵�
	if(yPointer - (scrollH)*15>Heightt-50)
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_VERT, &si);

	si.nPos += 2;
	scrollH +=2;
	 SetScrollPos(hWnd, SB_VERT, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
	if(yPointer - (scrollH)*15<40 )
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_VERT, &si);

	si.nPos -= 2;
	scrollH -=2;
	 SetScrollPos(hWnd, SB_VERT, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}



			InvalidateRect(hWnd,NULL,FALSE);
			/*
            UpdateWindow(hWnd);

			int x= 0;
				int y=0;
					hdc = GetDC(hWnd);
					for(int i =0; i< textCount; i++) {
					
					if(textCount > 0 &&texts[i].count == 0 && texts[i].text[0] == '\0'){
					x= 0;
					y += 17;
					continue;
					}
					if(textCount >0 && texts[i].count >= 15){
				
						
						TextOut(hdc,x- (scrollV) * (-15), y - (scrollH)*15, texts[i].text, 1);
						
					
					
					x += texts[i].count;
					
					continue;
					}
					
						
						TextOut(hdc,x- (scrollV) * (-15), y - (scrollH)*15, texts[i].text, 1);
						
					
					
					
					x += 10;
					
					continue;
			
					}*/


}

void Back(HDC hdc){
				
				if(textCount ==0) return;
				if(pointerCount == 0){
					return;
				ShowCaret(hWnd);
				}
				RECT tempRect;
	GetWindowRect(hWnd, &tempRect); // hWnd�� �������� �ڵ�

				// �������� ���� ���̸� ���
	int Widthh = tempRect.right - tempRect.left -50;
	int Heightt = tempRect.bottom - tempRect.top-50;
				
				HideCaret(hWnd);
				if (textCount > 0 && texts[pointerCount - 1].count >= 15) { //�����Է��� �����϶�
			
				
				
				
				delete[] (texts[pointerCount - 1].text);
				xPointer -= texts[pointerCount - 1].count;
				texts[pointerCount - 1].count =0;
                textCount--;
				pointerCount--;
                
                TextOut(hdc,xPointer- (scrollV) * (-15), yPointer, L"    ", 4);

				if(pointerCount < textCount){
				for(int i=pointerCount; i<textCount;i++){
					if(!texts[i+1].text)break;
					texts[i].text = texts[i+1].text;
					texts[i].count = texts[i+1].count;
				}
				texts[textCount+1].text = NULL;
			}

				if(pointerCount == textCount){
					SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
					ShowCaret(hWnd);
					return;
				}
				{
				
				RECT clientRect;
				GetWindowRect(hWnd, &clientRect); // hWnd�� �������� �ڵ�

				// �������� ���� ���̸� ���
				int windowWidth = clientRect.right - clientRect.left;
				int windowHeight = clientRect.bottom - clientRect.top;

				// RECT ����ü �����
				RectErase.left = 0; // ���� x ��ǥ
				RectErase.top = yPointer; // ���� y ��ǥ
				RectErase.right = RectErase.left + windowWidth; // ������ x ��ǥ (�������� ���� ���)
				RectErase.bottom = RectErase.top + 17; // �Ʒ��� y ��ǥ (������ ���̸� ���)



				//Ŀ�� ���� ��ũ�� �̵�
	if(xPointer- (scrollV) * (-15)>Widthh-50)
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos += 10;
	scrollV -=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
	if(xPointer- (scrollV) * (-15)<0 )
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos -= 10;
	scrollV +=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}



				InvalidateRect(hWnd,NULL,FALSE);	
				UpdateWindow(hWnd);
				
				

				
				}
				SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
			ShowCaret(hWnd);
				return;
            }
				else if (textCount > 0 && texts[pointerCount - 1].count == 0) { //���� �Է��� �����϶�
                yPointer -= 17;
                xPointer = 0; // ����� �κ�
                delete[] (texts[pointerCount - 1].text);
                textCount--;
				pointerCount--;
				
				
				if(pointerCount < textCount){
				for(int i=pointerCount; i<textCount;i++){
					if(!texts[i+1].text)break;
					texts[i].text = texts[i+1].text;
					texts[i].count = texts[i+1].count;
				}
				texts[textCount+1].text = NULL;
			}
				int x=0;
				int y=0;
				for(int i =0; i< pointerCount; i++) {
					
					if(textCount > 0 &&texts[i].count == 0 && texts[i].text[0] == '\0'){
					x= 0;
					y += 17;
					continue;
					}
					if(textCount >0 && texts[i].count >= 15){
				
						if(y >= yPointer)
						TextOut(hdc,x- (scrollV) * (-15), y - (scrollH)*15, texts[i].text, 1);
						
					
					
					x += texts[i].count;
					
					continue;
					}
					
						if(y >= yPointer)
						TextOut(hdc,x- (scrollV) * (-15), y - (scrollH)*15, texts[i].text, 1);
						
					
					
					
					x += 10;
					
					continue;
					}
				xPointer = x;

				if(pointerCount == textCount){
					TextOut(hdc,x- (scrollV) * (-15), y - (scrollH)*15, L"   ", 3);
					if(yPointer - (scrollH)*15>Heightt-50)
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_VERT, &si);

	si.nPos += 2;
	scrollH +=2;
	 SetScrollPos(hWnd, SB_VERT, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);
	SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
				ShowCaret(hWnd);
	}
	if(yPointer - (scrollH)*15<17 )
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_VERT, &si);

	si.nPos -= 2;
	scrollH -= 2;
	 SetScrollPos(hWnd, SB_VERT, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);
	SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
				ShowCaret(hWnd);
	}
					SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
					ShowCaret(hWnd);
					return;
				}
				{
				RECT clientRect;
				GetClientRect(hWnd, &clientRect); // hWnd�� �������� �ڵ�

				// �������� ���� ���̸� ���
				int windowWidth = clientRect.right - clientRect.left;
				int windowHeight = clientRect.bottom - clientRect.top;

				// RECT ����ü �����
				
				RectErase.left = 0; // ���� x ��ǥ
				RectErase.top = yPointer; // ���� y ��ǥ
				RectErase.right = RectErase.left + windowWidth; // ������ x ��ǥ (�������� ���� ���)
				RectErase.bottom = RectErase.top + windowHeight; // �Ʒ��� y ��ǥ (������ ���̸� ���)


							//Ŀ�� ���� ��ũ�� �̵�
	if(yPointer - (scrollH)*15>Heightt-50)
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_VERT, &si);

	si.nPos += 2;
	scrollH +=2;
	 SetScrollPos(hWnd, SB_VERT, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);
	SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
				ShowCaret(hWnd);
	}
	if(yPointer - (scrollH)*15<17 )
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_VERT, &si);

	si.nPos -= 2;
	scrollH -= 2;
	 SetScrollPos(hWnd, SB_VERT, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);
	SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
				ShowCaret(hWnd);
	}



				InvalidateRect(hWnd,NULL,FALSE);
				UpdateWindow(hWnd);			
				

				
				

				}

				SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
				ShowCaret(hWnd);

				return;
            }
				else if(textCount >0 && texts[pointerCount-1].count == 10){
				delete[] (texts[pointerCount - 1].text);
                textCount--;
				pointerCount--;
                xPointer -= 10;
				TextOut(hdc,xPointer- (scrollV) * (-15), yPointer, L"   ", 3);
				
				
				if(pointerCount < textCount){
				for(int i=pointerCount; i<textCount;i++){
					if(!texts[i+1].text)break;
					texts[i].text = texts[i+1].text;
					texts[i].count = texts[i+1].count;
				}
				texts[textCount+1].text = NULL;
				}
				
				if(pointerCount == textCount){
					SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
					ShowCaret(hWnd);

					return;
				}
				{
				RECT clientRect;
				GetClientRect(hWnd, &clientRect); // hWnd�� �������� �ڵ�

				// �������� ���� ���̸� ���
				int windowWidth = clientRect.right - clientRect.left;
				int windowHeight = clientRect.bottom - clientRect.top;

				// RECT ����ü �����
				
				RectErase.left = 0; // ���� x ��ǥ
				RectErase.top = yPointer; // ���� y ��ǥ
				RectErase.right = RectErase.left + windowWidth; // ������ x ��ǥ (�������� ���� ���)
				RectErase.bottom = RectErase.top + 17; // �Ʒ��� y ��ǥ (������ ���̸� ���)

			
				//Ŀ�� ���� ��ũ�� �̵�
	if(xPointer- (scrollV) * (-15)>Widthh-50)
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos += 10;
	scrollV -=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
	if(xPointer- (scrollV) * (-15)<0 )
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos -= 10;
	scrollV +=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}


				InvalidateRect(hWnd,NULL,FALSE);
				UpdateWindow(hWnd);
				
				
				
				
				}
				SetCaretPos(xPointer- (scrollV) * (-15),yPointer - (scrollH)*15);
			ShowCaret(hWnd);
				return;
			}
}

void BKTAB(HDC hdc){

	RECT tempRect;
	GetWindowRect(hWnd, &tempRect); // hWnd�� �������� �ڵ�

				// �������� ���� ���̸� ���
	int Widthh = tempRect.right - tempRect.left -50;
	int Heightt = tempRect.bottom - tempRect.top-50;


			wchar_t str[2] = { L' ', L'\0' };
			TextOut(hdc,xPointer- (scrollV) * (-15), yPointer, str, 4);
			xPointer += 40;


			if(InsertFlag ==1 && pointerCount < textCount){
				for(int i=textCount; i>pointerCount; i--){
					if(i ==0)break;
					texts[i].text = texts[i-1].text;
					texts[i].count = texts[i-1].count;
				}
			}
			if(InsertFlag == -1){
				if(pointerCount != textCount){}
				else{texts[pointerCount].text = new wchar_t[2];}
				
			}
			else{
				texts[pointerCount].text = new wchar_t[2];}
				wcscpy(texts[pointerCount].text, str);
				texts[pointerCount].count = 40;
			if(InsertFlag == -1 && textCount != pointerCount){}
			else{
				textCount++;}
				pointerCount++;
			
			
			
			{
				RECT clientRect;
				GetWindowRect(hWnd, &clientRect); // hWnd�� �������� �ڵ�

				// �������� ���� ���̸� ���
				int windowWidth = clientRect.right - clientRect.left;
				int windowHeight = clientRect.bottom - clientRect.top;

				// RECT ����ü �����
				RectErase.left = 0; // ���� x ��ǥ
				RectErase.top = yPointer; // ���� y ��ǥ
				RectErase.right = RectErase.left + windowWidth; // ������ x ��ǥ (�������� ���� ���)
				RectErase.bottom = RectErase.top + 17; // �Ʒ��� y ��ǥ (������ ���̸� ���)


				//Ŀ�� ���� ��ũ�� �̵�
	if(xPointer- (scrollV) * (-15)>Widthh-50)
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos += 10;
	scrollV -=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}
	if(xPointer- (scrollV) * (-15)<0 )
	{
	SCROLLINFO si;
    int xPos, nScrollCode;
	HideCaret(hWnd);
    // ���� ���� ��ũ�� ������ �����ɴϴ�.
	ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

	si.nPos -= 10;
	scrollV +=10;
	 SetScrollPos(hWnd, SB_HORZ, si.nPos, TRUE);
	InvalidateRect(hWnd,NULL,FALSE);

	}



				InvalidateRect(hWnd,NULL,FALSE);
				UpdateWindow(hWnd);

		
				


			}

}