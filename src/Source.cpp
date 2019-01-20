#include"TCP.h"
#include"Http.h"
 
 

#define IDC_URL	102
#define IDC_HTML 103
const char* className = "BASE";
std::string url, html;
HWND urlbox, htmlbox, hwnd;
WNDCLASSEX baseClass, htmlboxClass;
MSG Msg;
WNDPROC editBaseProc;

WSADATA wsaData;
serverCom main("tabMain");

website tab1;

void registerClasses(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK urlboxProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK htmlboxProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	 
	 
	 
	 
	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, className, "Browser", WS_OVERLAPPEDWINDOW, 200, 200, 300, 300, NULL, NULL, hInstance, NULL);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	WSACleanup();
	return Msg.wParam;
}

void registerClasses(HINSTANCE hInstance)
{
	baseClass.cbSize = sizeof(WNDCLASSEX);
	baseClass.style = 0;
	baseClass.lpfnWndProc = WndProc;
	baseClass.cbClsExtra = 0;
	baseClass.cbWndExtra = 0;
	baseClass.hInstance = hInstance;
	baseClass.hIcon = LoadIcon(NULL, IDI_SHIELD);
	baseClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	baseClass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	baseClass.lpszMenuName = NULL;
	baseClass.lpszClassName = className;
	baseClass.hIconSm = LoadIcon(NULL, IDI_SHIELD);
	if (!RegisterClassEx(&baseClass))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		exit(-1);
	}
	/*
	htmlboxClass.cbSize = sizeof(WNDCLASSEX);
	htmlboxClass.style = CS_HREDRAW;
	htmlboxClass.lpfnWndProc = htmlboxProc;
	htmlboxClass.cbClsExtra = 0;
	htmlboxClass.cbWndExtra = 0;
	htmlboxClass.hInstance = hInstance;
	htmlboxClass.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	htmlboxClass.lpszMenuName = NULL;
	htmlboxClass.lpszClassName = "HTML";
	if (!RegisterClassEx(&htmlboxClass))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		exit(-1);
	}
	*/

}

// Declare Callback Procedures
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		urlbox = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 0, 0, 280, 23, hwnd, (HMENU)IDC_URL, GetModuleHandle(NULL), NULL);
		editBaseProc = (WNDPROC)SetWindowLongPtr(urlbox, GWLP_WNDPROC, (LONG_PTR)urlboxProc);
		htmlbox = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL, 0, 23, 280, 280, hwnd, (HMENU)IDC_HTML, GetModuleHandle(NULL), NULL);
		break;
	case WM_SIZE:
		RECT newSize;
		GetClientRect(hwnd, &newSize);
		SetWindowPos(urlbox, NULL, 0, 0, newSize.right, newSize.top + 23, 0);
		SetWindowPos(htmlbox, NULL, 0, 23, newSize.right, newSize.bottom, 0);
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK urlboxProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (msg)
	{
		/*
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		OnPaint(hdc);
		EndPaint(hwnd, &ps);
		return 0;
	*/
	case WM_KEYDOWN:
		if (wParam == VK_RETURN)
		{
			if (tab1.getSite(urlbox) != 0)
				break;
			tab1.drawSite(htmlbox);
		}
		break;
	default:
		return CallWindowProc(editBaseProc, hwnd, msg, wParam, lParam);
	}
}
LRESULT CALLBACK htmlboxProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT box;
	HFONT font;
	switch(msg)
	{
	case WM_PAINT:
		 
		GetClientRect(hwnd, &box);
		hdc = BeginPaint(hwnd, &ps);
		
		font = CreateFont(-MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72), 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_MODERN, "Courier New");
		SelectObject(hdc, font);
		DrawText(hdc, "TEst", -1, &box, DT_LEFT);
		 
		DeleteObject(font);
		EndPaint(hwnd, &ps);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}