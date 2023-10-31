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
		return 0;
	case WM_MOUSEWHEEL:
		xPos = rect.left;
		yPos = rect.top;
		wheelParam = GET_WHEEL_DELTA_WPARAM(wParam);
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
