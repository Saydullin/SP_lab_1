#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	const wchar_t CLASS_NAME[] = L"MyWindowClass";

	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Mouse Wheel",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (hwnd == nullptr)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

BOOL IsInWnd(HWND hwnd, POINT point) {
	RECT rect;
	GetClientRect(hwnd, &rect);
	return PtInRect(&rect, point);
}

boolean shouldDraw = 1;

void DrawImageInRect(HWND hwnd, const RECT& rect)
{
	PAINTSTRUCT ps;
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, L"sample.bmp", IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
	HDC hdc = BeginPaint(hwnd, &ps);
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	FillRect(hdc, &clientRect, (HBRUSH)(COLOR_WINDOW));

	if (shouldDraw)
	{
		if (hBitmap)
		{

			HDC memDC = CreateCompatibleDC(hdc);
			HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);
			BITMAP bmp;
			GetObject(hBitmap, sizeof(BITMAP), &bmp);
			int imageWidth = bmp.bmWidth;
			int imageHeight = bmp.bmHeight;
			int destX = rect.left + (rect.right - rect.left - imageWidth) / 2;
			int destY = rect.top + (rect.bottom - rect.top - imageHeight) / 2;
			BitBlt(hdc, destX, destY, imageWidth, imageHeight, memDC, 0, 0, SRCCOPY);
			SelectObject(memDC, hOldBitmap);
			DeleteDC(memDC);
			DeleteObject(hBitmap);
		}
		else
		{
			MessageBox(hwnd, L"Не удалось загрузить изображение.", L"Ошибка", MB_OK | MB_ICONERROR);
		}
	}
	else {
		FillRect(hdc, &rect, NULL);
	}

	EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	int wheelParam = 0;
	static RECT rect = { 50, 50, 100, 100 };
	int step = 5;
	int xPos = 0;
	int yPos = 0;
	switch (uMsg) {
	case WM_CREATE:
		return 0;
	case WM_PAINT:
		DrawImageInRect(hwnd, rect);
		return 0;
	case WM_MOUSEWHEEL:
		xPos = rect.left;
		yPos = rect.top;
		wheelParam = GET_WHEEL_DELTA_WPARAM(wParam);
		if (GetKeyState(VK_SHIFT) & 0x8000) {
			if (wheelParam > 0) {
				POINT point = { xPos - step * 2, yPos };
				if (IsInWnd(hwnd, point)) {
					xPos -= step;
				}
				else {
					xPos += step * 3;
				}
			}
			else {
				POINT point = { xPos + 50 + step * 2, yPos };
				if (IsInWnd(hwnd, point)) {
					xPos += step;
				}
				else {
					xPos -= step * 3;
				}
			}
		}
		else {
			if (wheelParam > 0) {
				POINT point = { xPos, yPos - step * 2 };
				if (IsInWnd(hwnd, point)) {
					yPos -= step;
				}
				else {
					yPos += step * 3;
				}
			}
			else {
				POINT point = { xPos, yPos + 50 + step * 2 };
				if (IsInWnd(hwnd, point)) {
					yPos += step;
				}
				else {
					yPos -= step * 3;
				}
			}
		}
		SetRect(&rect, xPos, yPos, xPos + 50, yPos + 50);
		InvalidateRect(hwnd, nullptr, TRUE);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}


